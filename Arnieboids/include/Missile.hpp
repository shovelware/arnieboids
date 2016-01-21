#ifndef _MISSILE_H
#define _MISSILE_H

#include <Thor/Particles.hpp>

#include "Bullet.hpp"

#include "Ship.hpp"

class Missile : public Bullet {
public:
	/*!
	 * \param target The target for the missile
	 * \param position Initial position
	 * \param direction Launch direction
	 */
	Missile(thor::ParticleSystem& particleSystem, std::function<void()> destructCallback, const Ship* const target, sf::Vector2f  const &position, sf::Vector2f const &direction, const float maxSpeed = 4.f, const float acceleration = 0.1f, const float turnSpeed = 1.f);
	~Missile();

	//! Turns to face target while thrusting forward.
	virtual void update() override;

	void setForward(sf::Vector2f const &direction);
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

	//! Emitter for creating particles
	thor::UniversalEmitter particleEmitter_;

	void updateParticleEmitter();

private:
	//! Connection of emitter to particle system
	thor::Connection connection_;

	//! Used to notify firing ship that this missile has exploded
	const std::function<void()> destructCallback_;
};
#endif