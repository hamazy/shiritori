#ifndef SHIRITORI_SERVER_HPP
#define SHIRITORI_SERVER_HPP

namespace shiritori {

class server {
	const unsigned m_port;
public:
	explicit server(unsigned port)
		: m_port(port) {}

	virtual ~server() {}

	void start()
	{
	}

private:
	server();
	server(const server &src);
	server& operator=(const server &src);

};

}

#endif	// SHIRITORI_SERVER_HPP
