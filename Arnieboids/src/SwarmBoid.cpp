#include <include/SwarmBoid.hpp>

//define static members
std::list<SwarmBoid*> SwarmBoid::swarm_ = std::list<SwarmBoid*>();
float SwarmBoid::swarmThresholdRadius_ = 200.f;
Ship* SwarmBoid::swarmTarget_ = nullptr;

SwarmBoid::SwarmBoid(thor::ParticleSystem &particleSystem, sf::Vector2f position) :
Ship(particleSystem, position, 3.f)
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

	setOrigin(getLocalBounds().width * 0.5f, getLocalBounds().height * 0.5f);

	setFillColor(sf::Color(0, 100, 2));

	setOutlineThickness(1.f);
	setOutlineColor(sf::Color::Green);

	particleEmitter_.setParticleColor(getOutlineColor());
	particleEmitter_.setParticleLifetime(sf::seconds(0.25f));
	particleAngleVariance_ = 45.f;

	swarm_.push_back(this);
}

SwarmBoid::~SwarmBoid() {
	//remove self from static list of boids
	swarm_.erase(std::find(swarm_.begin(), swarm_.end(), this));
}

void SwarmBoid::update() {
	swarm();

	updateParticleEmitter();
}

void SwarmBoid::onCollide(Ship* other) {
	takeDamage(1u);
}

void SwarmBoid::setSwarmTarget(Ship* target) {
	swarmTarget_ = target;
}

void SwarmBoid::swarm() {
	sf::Vector2f sum(0, 0);

	static const float A = 100.0f;	//force of attraction
	static const float B = 50000.0f;	//force of seperation
	static const float N = 1.0f;	//attraction attenuation
	static const float M = 2.0f;	//seperatation attenuation

	int count = 0;
	for (SwarmBoid* boid : swarm_)
	{
		if (thor::length(boid->getPosition() - this->getPosition()) <= swarmThresholdRadius_) {
			sum += LenardJonesPotential(boid, count);
		}
	}

	//tend toward target
	if (swarmTarget_) {
		sum += LenardJonesPotential(swarmTarget_, count);
	}

	//get average
	sum /= static_cast<float>(count);

	velocity_ += sum;

	forward_ = thor::unitVector(velocity_);

	setRotation(atan2(forward_.y, forward_.x) * 57.296f);

	thrust();

	move(velocity_);

	//borders();
}

sf::Vector2f SwarmBoid::LenardJonesPotential(const Ship* const other, int& count) const {
	static const float A = 100.0f;	//force of attraction
	static const float B = 3000.0f;	//force of seperation
	static const float N = 1.0f;	//attraction attenuation
	static const float M = 1.8f;	//seperatation attenuation

	/*
	Lenard-Jones Potential function
	Vector R = me.position - you.position
	Real D = R.magnitude()
	Real U = -A / pow(D, N) + B / pow(D, M)
	R.normalise()
	force = force + R*U
	*/

	sf::Vector2f R;
	float D, U;

	R = this->getPosition() - other->getPosition();
	D = thor::length(R);

	if (D > 1)	//1 instead of 0, just in case of rounding errors
	{
		++count;
		if (other == swarmTarget_)
		{
			//apply no separation to swarmTarget_
			U = (-A / powf(D, N));
		}
		else {
			U = (-A / powf(D, N)) + (B / powf(D, M));	//Lenard-Jones Potential
		}

		R = thor::unitVector(R);

		//sum += R*U
		R *= U;
	}
	else
	{
		R = sf::Vector2f();
	}

	return R;

}
