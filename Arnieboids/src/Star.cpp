#include <include/Star.hpp>

Star::Star(sf::Vector2f position, float radius, sf::Color col) : CircleShape(radius)
{
	setPosition(position);
	setFillColor(col);

	setOutlineThickness(-radius * .2f);
	setOutlineColor(col - sf::Color(196, 0, 196));
}