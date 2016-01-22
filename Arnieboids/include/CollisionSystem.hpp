#ifndef _COLLISION_SYSTEM_H
#define _COLLISION_SYSTEM_H

#include <list>
#include "Ship.hpp"
#include "Player.hpp"
#include "Predator.hpp"
#include "Bullet.hpp"
#include "Pickup.hpp"

/*!
* \brief Collision System class.
* Provides SAT collision for Convex Shapes.
* Contains references to game lists
*/


class CollisionSystem {
public:
	CollisionSystem(std::list<Ship*>& shipList, std::list<Bullet*>& bulletList, std::list<Pickup*>& pickupList);
	~CollisionSystem();

	void Check() const;

private:
	std::list<Ship*>& ships_;
	std::list<Bullet*>& bullets_;
	std::list<Pickup*>& pickups_;

	//! Check two ships for collision
	bool checkPair(sf::ConvexShape* first, sf::ConvexShape* second) const;

	//! Get the axies of a shape and insert them into the list
	void getAxies(const sf::ConvexShape* const shape, std::list<sf::Vector2f>& axies) const;

	//! Project a shape onto an axis and return the line projection
	sf::Vector2f projectOntoAxis(const sf::ConvexShape* const shape, sf::Vector2f &axis) const;
};
#endif