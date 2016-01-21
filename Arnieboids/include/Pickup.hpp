#ifndef _PICKUP_H
#define _PICKUP_H

#include <SFML/Graphics.hpp>

#include <Thor/Vectors.hpp>

#include "Ship.hpp"

class Pickup : public sf::ConvexShape {
public:
	Pickup(sf::Vector2f const &position, int value);
	void update();
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

	unsigned int value_;
	unsigned int ticks_;
	unsigned int takeTick_;
	unsigned int shieldTimeTotal_;
	unsigned int shieldTime_;

	float tickToSec(unsigned int ticks) const;
};
#endif