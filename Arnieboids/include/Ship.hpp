#ifndef _SHIP_H
#define _SHIP_H

#include <SFML/Graphics/ConvexShape.hpp>

#include <Thor/Vectors.hpp>

/*!
 * \brief Base Ship class.
 * Abstract class that inherits from sf::ConvexShape.
 * Contains members common to all ships.
 */
class Ship : public sf::ConvexShape {
public:
	Ship(float maxSpeed);
	~Ship();

	virtual void update() = 0;	//!< Hides sf::Shape::update()

	virtual void onCollide(Ship* other) = 0;

	void takeDamage(unsigned int amount);

	bool isDead() const;

protected:
	const float MAX_SPEED_;	//!< Maximum length of velocity_ vector.
	sf::Vector2f velocity_;	//!< Change of position of ship per update.
	unsigned int health_;

	/*!
	 * \brief Clamps the length of the velocity_ vector to MAX_SPEED_
	 */
	void clampToMaxSpeed();
};
#endif