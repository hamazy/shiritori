#ifndef SHIRITORI_SERVER_HPP
#define SHIRITORI_SERVER_HPP

#include <iostream>
#include <boost/thread.hpp>

namespace shiritori {

class server {
public:
	const unsigned m_port;
	bool m_terminated;
	boost::condition_variable m_condition;
	boost::mutex m_mutex;
public:
	explicit server(unsigned port)
		: m_port(port)
		, m_terminated(false)
		, m_condition()
		, m_mutex() {}

	virtual ~server() {}

	boost::thread start()
	{
		return boost::thread(boost::ref(*this));
	}

	void operator()()
	{
		boost::unique_lock<boost::mutex> lock(m_mutex);
		while (!m_terminated)
		{
			m_condition.wait(lock);
		}
	}

	void stop()
	{
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
