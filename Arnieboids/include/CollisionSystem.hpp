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
* Provides collision checks for SFML ConvexShapes.
* Provides SAT collision for narrow-phase checks and bounding box collision for broad-phase checks.
* \remarks Contains references to game lists.
* \remarks Due to a bug with bullets (but not missiles for some reason), SAT has been disabled for all bullets (solely bounding box collision is used).
*/
class CollisionSystem {
public:
	/*!
	 * Creates a collision system with **references** to lists of entities from Game
	 * \param shipList A reference to a list of ship pointers.
	 * \param bulletList A reference to a list of bullet pointers.
	 * \param pickupList A reference to a list of pickup pointers.
	 */
	CollisionSystem(std::list<Ship*>& shipList, std::list<Bullet*>& bulletList, std::list<Pickup*>& pickupList);
	~CollisionSystem();

	/*!
	 * \brief Checks for collision between all game entities.
	 * Checks for ship-ship collision (broad and narrow phase),
	 * checks for ship-bullet collision (broad phase only, with PLAYER/ENEMY factions),
	 * checks for ship-powerup collision (broad and narrow phase, only player and predators can collect powerups).
	 */
	void Check() const;

private:
	std::list<Ship*>& ships_;		//!< A reference to Game::ships_
	std::list<Bullet*>& bullets_;	//!< A reference to Game::bullets_
	std::list<Pickup*>& pickups_;	//!< A reference to Game::pickups_

	//! Check two ships for collision
	bool checkPair(sf::ConvexShape* first, sf::ConvexShape* second) const;

	//! Get the axies of a shape and insert them into the list
	void getAxies(const sf::ConvexShape* const shape, std::list<sf::Vector2f>& axies) const;

	//! Project a shape onto an axis and return the line projection
	sf::Vector2f projectOntoAxis(const sf::ConvexShape* const shape, sf::Vector2f &axis) const;
};
#endif