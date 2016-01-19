#include <include/Predator.hpp>

Predator::Predator(thor::ParticleSystem& particleSystem, std::function<void(Bullet*)> const &fireCallback, sf::Vector2f position) :
Ship(particleSystem, position, 4.f),
fireCallback_(fireCallback)
{
	setPointCount(6u);

	setPoint(0u, sf::Vector2f(10, 0));
	setPoint(1u, sf::Vector2f(0, 40));
	setPoint(2u, sf::Vector2f(10, 50));
	setPoint(3u, sf::Vector2f(20, 50));
	setPoint(4u, sf::Vector2f(30, 40));
	setPoint(5u, sf::Vector2f(20, 0));

	setOrigin(getLocalBounds().width * 0.5f, getLocalBounds().height * 0.5f);

	setOutlineColor(sf::Color::Yellow);
	setFillColor(sf::Color(100, 100, 0));
	setOutlineThickness(-2.f);

	particleEmitter_.setParticleColor(getOutlineColor());
}

Predator::~Predator() {
}

void Predator::update() {
}

void Predator::onCollide(Ship* other) {
}
