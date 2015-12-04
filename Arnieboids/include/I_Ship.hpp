#ifndef _I_SHIP_H
#define _I_SHIP_H

#include <SFML/Graphics/ConvexShape.hpp>

#include <Thor/Vectors.hpp>

/*!
 * \brief Ship interface
 * Abstract class that inherits from sf::ConvexShape.
 * Contains members common to ALL ships.
 */
class I_Ship : public sf::ConvexShape {
public:
	I_Ship(float maxSpeed);
	~I_Ship();

	virtual void update() = 0;	//!< Hides sf::Shape::update()

protected:
	const float MAX_SPEED_;	//!< Maximum length of velocity_ vector.
	sf::Vector2f velocity_;	//!< Change of position of ship per update.

	/*!
	 * \brief Clamps the length of the velocity_ vector to MAX_SPEED_
	 */
	void clampToMaxSpeed();
};
#endif