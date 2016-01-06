#ifndef _MISSILE_H
#define _MISSILE_H

#include "Bullet.hpp"

#include "Ship.hpp"

class Missile : public Bullet {
public:
	Missile(const Ship* const target, sf::Vector2f  const &position, sf::Vector2f const &direction, const float maxSpeed = 5.f, const float acceleration = 0.1f, const float turnSpeed = 2.f);
	~Missile();

	virtual void update() override;
protected:

	const Ship* const TARGET_;

	const float MAX_SPEED_;

	const float ACCELERATION_;

	const float TURN_SPEED_;

	bool isAccelerating_;
};
#endif