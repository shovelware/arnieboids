#include <include/CollisionSystem.hpp>

CollisionSystem::CollisionSystem(std::list<Ship*>& shipList, std::list<Bullet*>& bulletList) :
ships_(shipList),
bullets_(bulletList)
{
}

CollisionSystem::~CollisionSystem() {
}

void CollisionSystem::Check() const {
	for (Ship* first : ships_)
	{
		for (Ship* second : ships_)
		{
			if (first != second &&
				checkPair(first, second))
			{
				first->onCollide(second);
				second->onCollide(first);
			}
		}

	}
}

bool CollisionSystem::checkPair(Ship* first, Ship* second) const {

	if (thor::length(first->getPosition() - second->getPosition()) <=
		first->getRadius() + second->getRadius())
	{
		bool collisionOccured = true;
		std::list<sf::Vector2f> axies;

		//get the axies of each ship's geometry
		getAxies(first, axies);
		getAxies(second, axies);

		float overlap = FLT_MAX;

		for (sf::Vector2f axis : axies)
		{
			//Project both shapes onto this axis
			sf::Vector2f A = projectOntoAxis(first, axis);
			sf::Vector2f B = projectOntoAxis(second, axis);

			float axisOverlap = 0;

			//Check for overlap between the projected shapes
			if (A.x >= B.x && A.x <= B.y)
			{
				axisOverlap = B.y - A.x;
			}
			else if (A.y >= B.x && A.y <= B.y)
			{
				axisOverlap = A.y - B.x;
			}
			else
			{
				//Overlap must occur on all axies or else collision did not occur.
				collisionOccured = false;
				break;
			}
		}//foreach

		return collisionOccured;
	}
	return false;
}

void CollisionSystem::getAxies(const Ship* const ship, std::list<sf::Vector2f>& axies) const {
	size_t pointCount = ship->getPointCount();
	for (size_t i = 0; i < pointCount; ++i)
	{
		sf::Vector2f p1 = ship->getPoint(i);
		sf::Vector2f p2 = ship->getPoint(i + 1 == pointCount ? 0 : i + 1);	//wrap around end of point container
		sf::Vector2f edge = p1 - p2;
		axies.push_back(sf::Vector2f(-edge.y, edge.x));
	}
}

sf::Vector2f CollisionSystem::projectOntoAxis(const Ship* const ship, sf::Vector2f &axis) const {
	if (thor::length(axis) != 1)
	{
		_ASSERT(thor::length(axis) != 0);
		//normalize the axis
		axis = thor::unitVector(axis);
	}
	sf::Vector2f shipPos = ship->getPosition();
	float min = thor::dotProduct(axis, shipPos + ship->getPoint(0));
	float max = min;	//max and min points on the axis
	float p;	//point on the axis

	size_t pointCount = ship->getPointCount();
	for (size_t i = 1; i < pointCount; ++i)
	{
		p = thor::dotProduct(axis, shipPos + ship->getPoint(i));

		if (p < min)
		{
			min = p;
		}
		else if (p > max)
		{
			max = p;
		}
	}

	return sf::Vector2f(min, max);
}
