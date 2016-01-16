#ifndef _MISSILE_H
#define _MISSILE_H

#include "Bullet.hpp"

#include "Ship.hpp"

class Missile : public Bullet {
public:
	/*!
	 * \param target The target for the missile
	 * \param position Initial position
	 * \param direction Launch direction
	 */
	Missile(const Ship* const target, sf::Vector2f  const &position, sf::Vector2f const &direction, const float maxSpeed = 5.f, const float acceleration = 0.1f, const float turnSpeed = 2.f);
	~Missile();

	//! Turns to face target while thrusting forward.
	virtual void update() override;
protected:
	//! Ship to be tracked.
	const Ship* const TARGET_;

	//! Maximum speed of the missile. Higher value == larger turning circle
	const float MAX_SPEED_;

	//! How fast (per tick) the missile accelerates.
	const float ACCELERATION_;

	//! Has quickly the missile can turn. Higher value == tighter turning circle.
	const float TURN_SPEED_;

	//! True if the missile has yet to reach max speed.
	bool isAccelerating_;
};
#endif