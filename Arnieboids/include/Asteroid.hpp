#ifndef _ASTEROID_H
#define _ASTEROID_H

#include <SFML/Graphics.hpp>
#include "Ship.hpp"

/*!
* \brief Asteroid class.
* Asteroids are technically ships but do things a little differently.
* Constantly move forward at fixed speed, collisions impart some velocity change.
*/
class Asteroid : public Ship {
public:
	/*!
	 * \param position Initial position of the Asteroid.
	 * \param direction The heading that the Asteroid will travel on
	 * \spinSpeed How fast (in degrees per tick) that the asteroid will rotate
	 */
	Asteroid(thor::ParticleSystem &particleSystem, sf::Vector2f const &position, sf::Vector2f const &direction, float spinSpeed = (rand() % 50 - 25) * 0.1f);
	Asteroid(thor::ParticleSystem &particleSystem, sf::Vector2f const &position, float spinSpeed = (rand() % 50 - 25) * 0.1f);
	~Asteroid();

	//! Moves us forward and keps us spinning
	void update() override;

	/*!
	 * Colliding with ships will impart some of that ship's velocity on us
	 * \remarks Asteroids do not "collide", with each other; they grind against and damage each other
	 */
	void onCollide(Ship* other) override;

private:
	//! How fast we rotate per tick (in degrees)
	float rotationSpeed_;

	/*!
	* Generates a convex shape with a randomly generated number of sides
	* and variations on the x and y position of each vertex.
	* \remarks Uses Rcos(theta) to place points along a circle and then uses rand() vary the positions.
	* \remarks Generates a random shade of brown as fill color.
	* \remarks Though shape outline is thick, it is drawn inside the shape bounds (because it's negative).
	*/
	void generateGeometry();
};
#endif