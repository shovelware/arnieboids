#ifndef _ASTEROID_H
#define _ASTEROID_H

#include <SFML/Graphics.hpp>
#include "Ship.hpp"

class Asteroid : public Ship {
public:
	Asteroid(sf::Vector2f const &position, sf::Vector2f const &direction, float spinSpeed = (rand() % 50 - 25) * 0.1f);
	Asteroid(sf::Vector2f const &position, float spinSpeed = (rand() % 50 - 25) * 0.1f);
	~Asteroid();

	void update() override;
	void onCollide(Ship* other) override;

private:
	float rotationSpeed_;

	void generateGeometry();
};
#endif