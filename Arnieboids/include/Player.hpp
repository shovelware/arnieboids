#ifndef _PLAYER_H
#define _PLAYER_H

#include "Ship.hpp"

/*!
 * \brief Controllable player class.
 * Has no AI behaviour.
 * Controlled through keyboard input in Game::handleEvents()
 */
class Player : public Ship {
public:
	Player(thor::ParticleSystem &particleSystem, sf::Vector2f const &position, unsigned int maxHealth = 10u);
	~Player();

	//! Moves by velocity and cools gun.
	void update() override;

	//! Causes ship take damage if an asteroid was hit.
	void onCollide(Ship* other) override;

private:
};
#endif