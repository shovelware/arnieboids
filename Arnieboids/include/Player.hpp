#ifndef _PLAYER_H
#define _PLAYER_H

#include "Ship.hpp"

class Player : public Ship {
public:
	Player(sf::Vector2f const &position, unsigned int maxHealth = 10U);
	~Player();

	void update() override;

	//! Not implemented!
	void onCollide(Ship* other) override;
private:
	const unsigned int MAX_HEALTH_;
};
#endif