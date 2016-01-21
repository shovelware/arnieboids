#ifndef _SHIP_H
#define _SHIP_H

#include <SFML/Graphics.hpp>

#include <Thor/Vectors.hpp>
#include <Thor/Particles.hpp>
#include <Thor/Math.hpp>

/*!
 * \brief Base Ship class.
 * Abstract class that inherits from sf::ConvexShape.
 * Contains members common to all ships.
 */
class Ship : public sf::ConvexShape {
public:
	Ship(thor::ParticleSystem &particleSystem, sf::Vector2f const &position, float maxSpeed, unsigned int health = 1u);
	~Ship();

	virtual void update();	//!< Hides sf::Shape::update()

	virtual void onCollide(Ship* other) = 0;

	//! Reduces health by the specified amount
	void takeDamage(unsigned int amount);

	//! Returns true if health is zero
	bool isDead() const;

	//! Apply acceleration in direction of forward vector
	void thrust();

	//! Apply deceleration if thrust is present
	void brake();

	//! Turn ship left
	void turnLeft();

	//! Turn ship right
	void turnRight();

	//! Attempt to fire, returns success
	bool trigger();

	/*!
	 * Turns toward a given direction
	 */
	void turnToward(sf::Vector2f const &direction);

	//! Returns forward direction of ship
	sf::Vector2f getForward() const;

	//! Returns radar radius
	void setRadarRange(float range);
	float getRadarRange() const;

protected:
	//! Maximum length of velocity vector
	const float MAX_SPEED_;

	float refireTime_;
	float coolDown_;
	unsigned int ticks_; //ticks_ = (ticks_ + 1) % INT_MAX;

	//! Delta position per update
	sf::Vector2f velocity_;

	//! Delta velocity after update !velocity_ += accel_ in update!
	sf::Vector2f accel_;

	//! Current health
	int health_;

	//! Unit vector for ship's heading
	sf::Vector2f forward_;

	//! How fast the ship accelerates
	float thrust_;

	//! How fast (in degrees) the ship can turn
	float turnSpeed_;

	//! Clamps the length of the velocity_ vector to MAX_SPEED_
	void clampToMaxSpeed();

	//! Ship's radar radius
	float radarRange_;

	//! Ticks to seconds
	float tickToSec(unsigned int ticks) const;

	//! Does nothing on it's own. Derived classes should play with this.
	thor::UniversalEmitter particleEmitter_;

	virtual void updateParticleEmitter();
	
	float particleAngleVariance_;

private:
	//! The ship's connection to the Game's particle system.
	thor::Connection connection_;
};
#endif