#ifndef _PLAYER_H
#define _PLAYER_H

#include "Ship.hpp"
#include "XController.hpp"

class Player : Ship {
public:
	Player(sf::Vector2f const &position, unsigned int maxHealth = 10U);
	~Player();

	void update() override;
private:
	XController controller_;
	const unsigned int MAX_HEALTH_;
};
#endif