#ifndef _COLLISION_SYSTEM_H
#define _COLLISION_SYSTEM_H

#include <list>
#include "Ship.hpp"
#include "Bullet.hpp"

class CollisionSystem {
public:
	CollisionSystem(std::list<Ship*>& shipList, std::list<Bullet*>& bulletList);
	~CollisionSystem();

	void Check() const;

private:
	std::list<Ship*>& ships_;
	std::list<Bullet*>& bullets_;

	//! Check two ships for collision
	bool checkPair(sf::ConvexShape* first, sf::ConvexShape* second) const;

	//! Get the axies of a shape and insert them into the list
	void getAxies(const sf::ConvexShape* const shape, std::list<sf::Vector2f>& axies) const;

	//! Project a shape onto an axis and return the line projection
	sf::Vector2f projectOntoAxis(const sf::ConvexShape* const shape, sf::Vector2f &axis) const;
};
#endif