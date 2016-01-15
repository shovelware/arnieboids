#include <include/Asteroid.hpp>

inline float rand(int max) {
	return rand() % max;
}

Asteroid::Asteroid(sf::Vector2f const& position, sf::Vector2f const& direction, float spinSpeed) :
Ship(position, rand(50) * 0.1f),
rotationSpeed_(spinSpeed)
{
	forward_ = thor::unitVector(direction);

	generateGeometry();
}

Asteroid::Asteroid(sf::Vector2f const& position, float spinSpeed) :
Ship(position, (rand() % 50) * 0.1f),
rotationSpeed_(spinSpeed)
{
	forward_.y = -cosf(thor::toRadian(rand(360)));
	forward_.x = sinf(thor::toRadian(rand(360)));
	forward_ = thor::unitVector(forward_);

	generateGeometry();
}

Asteroid::~Asteroid() {
}

void Asteroid::update() {
	move(forward_);
	rotate(rotationSpeed_);
}

void Asteroid::onCollide(Ship* other) {
}

void Asteroid::generateGeometry() {
	size_t numPoints = rand(5) + 5;
	float radius = rand(30) + 20;
	float deltaTheta = thor::toRadian(static_cast<float>(360 / numPoints));
	float theta = deltaTheta;

	setPointCount(numPoints);

	for (size_t i = 0u; i < numPoints; ++i)
	{
		setPoint(i, sf::Vector2f(rand(10) + radius * cosf(theta), rand(10) + radius * sinf(theta)));
		theta += deltaTheta;
	}

	setFillColor(sf::Color(
		rand() % 50 + 100,	//add some varience to the color
		rand() % 50 + 50,
		rand() % 20 + 20));
	setOutlineColor(sf::Color::White);
	setOutlineThickness(-5.f);	//don't outdent the visible shape (draw the outline inside the shape bounds, not around the outside)
}
