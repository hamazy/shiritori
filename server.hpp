#ifndef SHIRITORI_SERVER_HPP
#define SHIRITORI_SERVER_HPP

#include <iostream>
#include <set>
#include <algorithm>
#include <queue>

#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace shiritori {

class message
{
	enum { message_length = 8 };
	char data_[message_length];
public:
	message() {}
	virtual ~message() {}
	char *data()
	{
		return data_;
	}
	char const *data() const
	{
		return data_;
	}
	std::size_t size() const
	{
		return message_length;
	}
};

class player {
public:
	typedef boost::shared_ptr<player> pointer;
	virtual ~player() {}
	virtual void deliver(message const &message) = 0;
};

class game
{
	std::set<player::pointer> players_;
public:
	game()
		: players_() {}

	virtual ~game() {}

	void join(player::pointer player)
	{
		players_.insert(player);
	}

	void leave(player::pointer player)
	{
		players_.erase(player);
	}
	void deliver(message const &message)
	{
		std::for_each(
			players_.begin(), players_.end(),
			boost::bind(&player::deliver, _1, boost::ref(message)));
	}

private:
	game(game const &src);
	game &operator=(game const &src);

};

class session
	: public boost::enable_shared_from_this<session>,
	  public player
{
	boost::asio::ip::tcp::socket socket_;
	game &game_;
	message message_;
	std::queue<message> message_queue_;

public:
	typedef boost::shared_ptr<session> pointer;

	session(boost::asio::io_service& io_service, game &game)
		: socket_(io_service)
		, game_(game)
		, message_()
		, message_queue_()
	{}

	virtual ~session() {}

	boost::asio::ip::tcp::socket& socket() { return socket_; }

	void start()
	{
		game_.join(shared_from_this());
		read_message();
	}

	void handle_read(boost::system::error_code const &error)
	{
		if (error)
		{
			leave_from_game();
			return;
		}
		game_.deliver(message_);
		read_message();
	}

	void handle_write(boost::system::error_code const &error)
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

	void deliver(message const &message_to_deliver)
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
	void read_message()
	{
		boost::asio::async_read(
			socket_,
			boost::asio::buffer(message_.data(), message_.size()),
			boost::bind(
				&session::handle_read, shared_from_this(),
				boost::asio::placeholders::error));
	}
	void write_front_message()
	{
		const message message_delivering(message_queue_.front());
		boost::asio::async_write(
			socket_,
			boost::asio::buffer(message_delivering.data(), message_delivering.size()),
			boost::bind(
				&session::handle_write, shared_from_this(),
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
	game game_;
public:
	explicit server(unsigned port, boost::asio::io_service &io_service)
		: port_(port)
		, io_service_(io_service)
		, acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port_))
		, terminated_(false)
		, condition_()
		, mutex_()
		, game_()
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
