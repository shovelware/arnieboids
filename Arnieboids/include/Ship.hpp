#ifndef _SHIP_H
#define _SHIP_H

#include <SFML/Graphics.hpp>

#include <Thor/Vectors.hpp>

/*!
 * \brief Base Ship class.
 * Abstract class that inherits from sf::ConvexShape.
 * Contains members common to all ships.
 */
class Ship : public sf::ConvexShape {
public:
	Ship(float maxSpeed, unsigned int health = 1u);
	~Ship();

	virtual void update() = 0;	//!< Hides sf::Shape::update()

	virtual void onCollide(Ship* other) = 0;

	//Reduces health by the specified amount
	void takeDamage(unsigned int amount);

	//! Returns true if health is zero
	bool isDead() const;

protected:
	//! Maxiumum length of velocity vector
	const float MAX_SPEED_;

	//! Delta position per update
	sf::Vector2f velocity_;	

	//! Current health
	unsigned int health_;

	//! Unit vector for ship's heading
	sf::Vector2f forward_;

	//! How fast the ship accelerates
	float thrust_;

	//! How fast (in degrees) the ship can turn
	float turnSpeed_;

	//! Clamps the length of the velocity_ vector to MAX_SPEED_
	void clampToMaxSpeed();

	//! Apply accelleration in direction of forward vector
	void thrust();
};
#endif