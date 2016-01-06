#include <include/SwarmBoid.hpp>

//define static list of boids
std::list<SwarmBoid*> SwarmBoid::swarm_ = std::list<SwarmBoid*>();

SwarmBoid::SwarmBoid(sf::Vector2f position) :
Ship(position, 3.f)
{
	setPointCount(6u);

	// Shape:
	//	 ______
	//	/	   \
	//	\______/
	//

	setPoint(0u, sf::Vector2f(0, 10));
	setPoint(1u, sf::Vector2f(10, 0));
	setPoint(2u, sf::Vector2f(20, 0));
	setPoint(3u, sf::Vector2f(30, 10));
	setPoint(4u, sf::Vector2f(20, 20));
	setPoint(5u, sf::Vector2f(10, 20));

	setOrigin(15.f, 5.f);

	setFillColor(sf::Color(0, 100, 2));

	setOutlineThickness(1.f);
	setOutlineColor(sf::Color::Green);
}

SwarmBoid::~SwarmBoid() {
	//remove self from static list of boids
	swarm_.erase(std::find(swarm_.begin(), swarm_.end(), this));
}

void SwarmBoid::update() {
	//TODO: swarm!
}

void SwarmBoid::onCollide(Ship* other) {
	--health_;
}
