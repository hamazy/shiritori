#ifndef SHIRITORI_SERVER_HPP
#define SHIRITORI_SERVER_HPP

#include <iostream>
#include <algorithm>
#include <queue>
#include <string>

#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "game.hpp"
#include "player.hpp"

namespace shiritori {

class session
	: public boost::enable_shared_from_this<session>,
	  public player
{
	boost::asio::ip::tcp::socket socket_;
	game &game_;
	boost::asio::streambuf buffer_;
	std::queue<std::string> message_queue_;

public:
	typedef boost::shared_ptr<session> pointer;

	virtual ~session() {}

	boost::asio::ip::tcp::socket& socket() { return socket_; }

	void start()
	{
		game_.join(shared_from_this());
		read_message();
	}

	void on_read(boost::system::error_code const &error, std::size_t bytes_transferred)
	{
		if (error)
		{
			leave_from_game();
			return;
		}

		std::string line;
		std::istream stream(&buffer_);
		std::getline(stream, line);
		
		game_.respond_to(line);
		read_message();
	}

	void on_write(boost::system::error_code const &error)
	{
		if (error)
		{
			leave_from_game();
			return;
		}
		message_queue_.pop();

		if (message_queue_.empty()) return;
		write_front_message();
	}

	void deliver(std::string const &message_to_deliver)
	{
		const bool write_in_progress(!message_queue_.empty());
		message_queue_.push(message_to_deliver);
		if (write_in_progress) return;
		write_front_message();
	}

	static pointer create(boost::asio::io_service& io_service, game &game)
	{
		return pointer(new session(io_service, game));
	}

private:
	session(boost::asio::io_service& io_service, game &game)
		: socket_(io_service)
		, game_(game)
		, buffer_()
		, message_queue_()
	{}

	void read_message()
	{
		boost::asio::async_read_until(
			socket_, buffer_, "\r\n",
			boost::bind(
				&session::on_read, shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}
	void write_front_message()
	{
		std::string const message_delivering(message_queue_.front());
		boost::asio::async_write(
			socket_,
			boost::asio::buffer(message_delivering),
			boost::bind(
				&session::on_write, shared_from_this(),
				boost::asio::placeholders::error));
	}
	void leave_from_game()
	{
		game_.leave(shared_from_this());
	}

	session();
	session(session const &src);
	session &operator=(session const &src);

};

class server
{
public:
	unsigned const port_;
	boost::asio::io_service &io_service_;
	boost::asio::ip::tcp::acceptor acceptor_;
	bool terminated_;
	boost::condition_variable condition_;
	boost::mutex mutex_;
	game &game_;
public:
	explicit server(unsigned port, game &game, boost::asio::io_service &io_service)
		: port_(port)
		, io_service_(io_service)
		, acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port_))
		, terminated_(false)
		, condition_()
		, mutex_()
		, game_(game)
	{
		start_accept();
	}

	virtual ~server() {}

	void start_accept()
	{
		session::pointer session(
			session::create(
				acceptor_.io_service(), game_));

		acceptor_.async_accept(
			session->socket(),
			boost::bind(
				&server::handle_accept, this, session,
				boost::asio::placeholders::error));
	}

	void handle_accept(session::pointer session, boost::system::error_code const &error)
	{
		if (error) return;

		session->start();
		start_accept();
	}

	boost::thread start()
	{
		return boost::thread(boost::ref(*this));
	}

	void operator()()
	{
		boost::unique_lock<boost::mutex> lock(mutex_);
		while (!terminated_)
		{
			io_service_.run();
			condition_.wait(lock);
		}
	}

	void stop()
	{
		io_service_.stop();
		{
			boost::lock_guard<boost::mutex> lock(mutex_);
			terminated_ = true;
		}
		condition_.notify_all();
	}

private:
	server();
	server(server const &src);
	server& operator=(const server &src);

};

} // namespace shiritori

#endif	// SHIRITORI_SERVER_HPP
