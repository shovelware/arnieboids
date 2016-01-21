#ifndef _PLAYER_H
#define _PLAYER_H

#include "Ship.hpp"

class Player : public Ship {
public:
	Player(thor::ParticleSystem &particleSystem, sf::Vector2f const &position, unsigned int maxHealth = 10u);
	~Player();

	//! Moves by velocity and cools gun.
	void update() override;

	//! Does nothing
	void onCollide(Ship* other) override;

private:
};
#endif