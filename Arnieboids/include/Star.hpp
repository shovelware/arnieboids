#ifndef _STAR_H
#define _STAR_H

#include <SFML/Graphics/CircleShape.hpp>

/*!
*	\brief Star class
*	A super-lightweight sf::CircleShape wrapper, for drawing starry background
*/

class Star : public sf::CircleShape
{
public:
	Star(sf::Vector2f position, float radius, sf::Color col);
};
#endif