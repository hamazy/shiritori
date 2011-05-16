#ifndef SHIRITORI_SERVER_HPP
#define SHIRITORI_SERVER_HPP

#include <iostream>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace shiritori {

class tcp_connection: public boost::enable_shared_from_this<tcp_connection> {
	boost::asio::ip::tcp::socket m_socket;
	std::string m_message;

public:
	typedef boost::shared_ptr<tcp_connection> pointer;

	tcp_connection(boost::asio::io_service& io_service)
		: m_socket(io_service)
		, m_message() {}

	virtual ~tcp_connection() {}

	boost::asio::ip::tcp::socket& socket() { return m_socket; }

	void start()
	{
		m_message = "hoge\n";

		boost::asio::async_write(m_socket, boost::asio::buffer(m_message),
								 boost::bind(&tcp_connection::handle_write, shared_from_this(),
											 boost::asio::placeholders::error,
											 boost::asio::placeholders::bytes_transferred));
	}

	void handle_write(const boost::system::error_code& /*error*/,
					  size_t /*bytes_transferred*/) {}


	static pointer create(boost::asio::io_service& io_service)
	{
		return pointer(new tcp_connection(io_service));
	}

private:
	tcp_connection();
	tcp_connection(const tcp_connection &src);
	tcp_connection &operator=(const tcp_connection &src);

};

class server {
public:
	const unsigned m_port;
	boost::asio::io_service &m_io_service;
	boost::asio::ip::tcp::acceptor m_acceptor;
	bool m_terminated;
	boost::condition_variable m_condition;
	boost::mutex m_mutex;
public:
	explicit server(unsigned port, boost::asio::io_service &io_service)
		: m_port(port)
		, m_io_service(io_service)
		, m_acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), m_port))
		, m_terminated(false)
		, m_condition()
		, m_mutex()
	{
		start_accept();
	}

	virtual ~server() {}

	void start_accept()
	{
		tcp_connection::pointer new_connection =
			tcp_connection::create(m_acceptor.io_service());

		m_acceptor.async_accept(new_connection->socket(),
							   boost::bind(&server::handle_accept, this, new_connection,
										   boost::asio::placeholders::error));
	}

	void handle_accept(tcp_connection::pointer new_connection,
					   boost::system::error_code const &error)
	{
		if (error) return;

		new_connection->start();
		start_accept();
	}

	boost::thread start()
	{
		return boost::thread(boost::ref(*this));
	}

	void operator()()
	{
		boost::unique_lock<boost::mutex> lock(m_mutex);
		while (!m_terminated)
		{
			m_io_service.run();
			m_condition.wait(lock);
		}
	}

	void stop()
	{
		m_io_service.stop();
		{
			boost::lock_guard<boost::mutex> lock(m_mutex);
			m_terminated = true;
		}
		m_condition.notify_all();
	}

private:
	server();
	server(const server &src);
	server& operator=(const server &src);

};

}

#endif	// SHIRITORI_SERVER_HPP
