#include <include/CollisionSystem.hpp>

CollisionSystem::CollisionSystem(std::list<Ship*>& shipList, std::list<Bullet*>& bulletList, std::list<Pickup*>& pickupList) :
ships_(shipList),
bullets_(bulletList),
pickups_(pickupList)
{
}

CollisionSystem::~CollisionSystem() {
}

void CollisionSystem::Check() const {
	for (Ship* first : ships_)
	{
		for (Ship* second : ships_)
		{
			//check if same ship
			if (first != second) {
				//do broadphase: check if bounding rects intersect
				if (first->getGlobalBounds().intersects(second->getGlobalBounds()))
				{
					//do narrowphase check with SAT
					if (checkPair(first, second))
					{
						first->onCollide(second);
						second->onCollide(first);
						break;
					}//end if(narrowphase)
				}//end if(broadphase)
			}//end if(same)
		}//end for(Ship* second)

		for (Bullet* bullet : bullets_) {
			//check if bullet intersects bounds
			if (first->getGlobalBounds().intersects(bullet->getGlobalBounds()))
			{
				//use SAT to check if collision occurred
				if (checkPair(first, bullet)) {
					first->takeDamage(bullet->getDamage());
					bullet->setActive(false);
				}
			}//end if(broadphase)
		}//end for bullets

		//For each ship with pickups
		for (Pickup* pickup : pickups_) {
			if (first->getGlobalBounds().intersects(pickup->getGlobalBounds()))
			{
				//Check if it's a player
				if (dynamic_cast<Player*>(first) || dynamic_cast<Predator*>(first))
				{
					if (checkPair(first, pickup))
					{
						if (!pickup->isOwned())
						{
							pickup->take(first);
						}
					}//fine
				}//player check
			}//broad
		}//end for pickups

	}//end for ships
}

bool CollisionSystem::checkPair(sf::ConvexShape* first, sf::ConvexShape* second) const {
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

void CollisionSystem::getAxies(const sf::ConvexShape* const shape, std::list<sf::Vector2f>& axies) const {
	size_t pointCount = shape->getPointCount();
	for (size_t i = 0; i < pointCount; ++i)
	{
		sf::Vector2f p1 = shape->getPoint(i);
		sf::Vector2f p2 = shape->getPoint(i + 1 == pointCount ? 0 : i + 1);	//wrap around end of point container
		sf::Vector2f edge = p1 - p2;
		axies.push_back(sf::Vector2f(-edge.y, edge.x));
	}
}

sf::Vector2f CollisionSystem::projectOntoAxis(const sf::ConvexShape* const shape, sf::Vector2f &axis) const {
	if (thor::length(axis) != 1)
	{
		_ASSERT(thor::length(axis) != 0);
		//normalize the axis
		axis = thor::unitVector(axis);
	}
	sf::Vector2f shipPos = shape->getPosition();
	float min = thor::dotProduct(axis, shipPos + shape->getPoint(0));
	float max = min;	//max and min points on the axis
	float p;	//point on the axis

	size_t pointCount = shape->getPointCount();
	for (size_t i = 1; i < pointCount; ++i)
	{
		p = thor::dotProduct(axis, shipPos + shape->getPoint(i));

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
