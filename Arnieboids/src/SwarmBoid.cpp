#include <include/SwarmBoid.hpp>

//define static list of boids
std::list<SwarmBoid*> SwarmBoid::swarm_ = std::list<SwarmBoid*>();

float SwarmBoid::swarmThresholdRadius_ = FLT_MAX;

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

	swarm_.push_back(this);
}

SwarmBoid::~SwarmBoid() {
	//remove self from static list of boids
	swarm_.erase(std::find(swarm_.begin(), swarm_.end(), this));
}

void SwarmBoid::update() {
	swarm();
}

void SwarmBoid::onCollide(Ship* other) {
	--health_;
}

// returns sin(theta)*length(a)*length(b), where theta = the angle between vectors a,b
inline float perpDot(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return (a.y*b.x) - (a.x*b.y);
}

void SwarmBoid::swarm() {
	/*
	Lenard-Jones Potential function
	Vector R = me.position - you.position
	Real D = R.magnitude()
	Real U = -A / pow(D, N) + B / pow(D, M)
	R.normalise()
	force = force + R*U
	*/
	sf::Vector2f R;
	sf::Vector2f sum(0, 0);
	float D, U;

	static const float A = 100.0f;	//force of attraction
	static const float B = 5000.0f;	//force of seperation
	static const float N = 1.0f;	//attraction attenuation
	static const float M = 2.0f;	//seperatation attenuation

	int count = 0;
	for (SwarmBoid* boid : swarm_)
	{
		R = this->getPosition() - boid->getPosition();
		D = thor::length(R);

		if (D > 1)	//1 instead of 0, just in case of rounding errors
		{
			++count;
			U = (-A / powf(D, N)) + (B / powf(D, M));	//Lenard-Jones Potential

			R = thor::unitVector(R);

			//sum += R*U
			R *= U;
			sum += R;
		}
	}
	//get average
	sum /= static_cast<float>(count);

	velocity_ += sum;
	clampToMaxSpeed();
	//borders();
	move(velocity_);
}
