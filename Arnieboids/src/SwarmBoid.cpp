#include <include/SwarmBoid.hpp>
#include <include/Player.hpp>

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
	Ship::update();
	swarm();
}

void SwarmBoid::onCollide(Ship* other) {

	//Don't take damage from other boids
	if (dynamic_cast<Player*>(other))
	{
		takeDamage();
	}
}

void SwarmBoid::setSwarmTarget(Ship* target) {
	swarmTarget_ = target;
}

void SwarmBoid::swarm() {
	sf::Vector2f sum(0, 0);

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

	turnToward(sum);

	thrust();
	clampToMaxSpeed();

	move(velocity_);

	updateParticleEmitter();

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

	//if swarming toward target and distance to target is too far away for kamikaze...
	if (other == swarmTarget_ && D > 100 && D < 600){
		R = getPosition() - extrapolate(other->getPosition(), other->getForward(), thor::length(other->getVelocity()) * D);
		D = thor::length(R);
	}

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

sf::Vector2f SwarmBoid::extrapolate(sf::Vector2f const& position, sf::Vector2f const& direction, float distance) const {
	return position + (thor::unitVector(direction) * distance);
}
