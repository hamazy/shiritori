/** -*- mode: c++; coding: utf-8; -*- 
 *@file
 *@brief
 */
#ifndef SHIRITORI_PLAYER_HPP_
#define SHIRITORI_PLAYER_HPP_

namespace shiritori {

class player
{
public:
	typedef boost::shared_ptr<player> pointer;
	virtual ~player() {}
	virtual void deliver(std::string const &message) = 0;
};

} // namespace shiritori

#endif // SHIRITORI_PLAYER_HPP_
