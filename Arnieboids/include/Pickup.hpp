#ifndef _PICKUP_H
#define _PICKUP_H

#include <SFML/Graphics.hpp>

#include <Thor/Vectors.hpp>

#include "Ship.hpp"

/*!
* \brief Pickup class.
* A multi-purpose pickup that assists a Player or Predator.
* A pickup will add as much health as it can, then provide shields based on remaining value
*/
class Pickup : public sf::ConvexShape {
public:
	Pickup(sf::Vector2f const &position, int value);

	//! Updates colours, pickup logic, shield timing
	void update();

	//! Affects the taker with pickup's logic
	void take(Ship* taker);

	bool isActive();
	bool isOwned() const;
	Ship* getOwner() const;

	//! Used to play as sound in Game::update() when power up is collected
	bool wasPickedUpLastTick();

private:
	Ship* owner_;
	bool pickedUp_;

	void setShape(float radius);

	unsigned int value_;			//! Value of powerup, in HP
	unsigned int ticks_;			//!	Ticks passed since spawn
	unsigned int takeTick_;			//!	The tick number that we were picked up on
	unsigned int shieldTimeTotal_;	//!	Total shielding time, value_ * 2
	unsigned int shieldTime_;		//!	Shield time that counts up to total when active

	float tickToSec(unsigned int ticks) const;
};
#endif