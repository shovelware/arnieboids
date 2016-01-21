#include <include/Mothership.hpp>

Ship* Mothership::target_ = nullptr;

Mothership::Mothership(thor::ParticleSystem& particleSystem, std::function<void(Bullet*)> const& fireCallback, std::function<void(Ship*)> spawnCallback, sf::Vector2f position) :
Ship(particleSystem, position, 2.f, 4u),
particleSystem_(particleSystem),
ticksSinceLastSpawn_(rand() % 100),	//offset spawn times a bit
ticksPerSpawn_(1000u),
fireBullet_(fireCallback),
spawnShip_(spawnCallback),
MAX_LIVE_MISSILES(4u)
{
	setPointCount(6u);

	setPoint(0u, sf::Vector2f(0, -60));
	setPoint(1u, sf::Vector2f(20, -20));
	setPoint(2u, sf::Vector2f(40, 60));
	setPoint(3u, sf::Vector2f(0, 100));
	setPoint(4u, sf::Vector2f(-40, 60));
	setPoint(5u, sf::Vector2f(-20, -20));

	//setOrigin(getLocalBounds().width * 0.5f, getLocalBounds().height * 0.5f);

	setFillColor(sf::Color(200, 100, 0));
	setOutlineColor(sf::Color(255,200,100));
	setOutlineThickness(-2.f);

	thrust_ = 0.005f;
	turnSpeed_ *= 0.1f;

	particleAngleVariance_ = 90.f;
	particleEmitter_.setParticleLifetime(thor::Distributions::uniform(sf::seconds(0.1f), sf::seconds(0.3f)));
	particleEmitter_.setEmissionRate(250.f);
	particleEmitter_.setParticleColor(getFillColor());

	wanderWaypoint_ = getPosition();
}

Mothership::~Mothership() {
}

void Mothership::update() {
	//spawn a ship if it is time to do so
	if (++ticksSinceLastSpawn_ >= ticksPerSpawn_)
	{
		spawnShip_(new Predator(particleSystem_, fireBullet_, getPosition()));
		ticksSinceLastSpawn_ = 0u;
	}

	//wander or evade
	switch (calculateState())
	{
	case WANDER:
		turnToward(wander());
		turnToward(flock());
		break;
	case EVADE:
		turnToward(evade());
		if (trigger())
			fire();
		break;
	}

	ticks_ = (ticks_ + 1) % INT_MAX;

	//Cool weapon
	if (coolDown_ > 0) {
		coolDown_ -= tickToSec(1);
	}

	thrust();

	Ship::update();

	move(velocity_);
}

void Mothership::onCollide(Ship* other) {
}

void Mothership::setTarget(Ship* target) {
	target_ = target;
}

void Mothership::updateParticleEmitter() {
	particleEmitter_.setParticlePosition(getPosition() + (-forward_ * getLocalBounds().height * 0.4f));
	particleEmitter_.setParticleVelocity(thor::Distributions::deflect(thor::unitVector(-forward_) * 200.f, particleAngleVariance_));
	particleEmitter_.setParticleScale(sf::Vector2f(0.25f, 0.25f));
}

Mothership::State Mothership::calculateState() const {
	if (target_)
	{
		sf::Vector2f displacement = target_->getPosition() - this->getPosition();
		if (thor::length(displacement) <= 300)
		{
			return EVADE;
		}
	}

	return WANDER;
}

sf::Vector2f Mothership::evade() const {
	static float desiredSeparation = 1000;

	sf::Vector2f steer(0, 0);

	//Avoid colliding with prey_
	sf::Vector2f diff = this->getPosition() - target_->getPosition();
	float distance = thor::length(diff);
	if (distance < desiredSeparation) {
		diff = thor::unitVector(diff);
		diff /= distance;
		steer += diff;
	}

	if (thor::length(steer) > 0)
	{
		//steering = desired - velocity
		steer = thor::unitVector(steer);
		steer *= MAX_SPEED_;
		//steer -= velocity_;
	}

	return steer;
}

sf::Vector2f Mothership::wander() {
	sf::Vector2f displacement = getPosition() - wanderWaypoint_;
	float distance = thor::length(displacement);
	if (distance < 100 || distance > 1000)
	{
		wanderWaypoint_ = getPosition() + thor::Distributions::deflect(forward_ * 1000.f, 90)();
	}

	return displacement;
}

void Mothership::fire() {
	if (liveMissiles_ < MAX_LIVE_MISSILES)
	{
		fireBullet_(new Missile(
			particleSystem_,	//particle system to connect the missile's emitter to
			std::bind(&Mothership::missileDestructed, this),	//called when the missile is destructed
			target_,	//target for the missile
			getPosition(),	//spawn position
			forward_	//spawn direction
			));

		++liveMissiles_;
	}
}

void Mothership::missileDestructed() {
	_ASSERT(liveMissiles_ > 0);
	--liveMissiles_;
}

sf::Vector2f Mothership::separation() const {
	static float desiredSeparation = 100;

	sf::Vector2f steer(0, 0);
	float count = 0;

	for (auto itr = flock_.begin();
		itr != flock_.end();
		++itr)
	{
		Mothership* flockMember = *itr;
		if (this != flockMember) {
			sf::Vector2f diff = this->getPosition() - flockMember->getPosition();
			float distance = thor::length(diff);
			if (distance < desiredSeparation)
			{
				diff = thor::unitVector(diff);
				diff /= distance;
				steer += diff;
				++count;
			}
		}//end if(this != *itr)
	}//end for

	//average out the steering
	if (count > 1)
	{
		steer /= count;
	}

	if (thor::length(steer) > 0)
	{
		steer = thor::unitVector(steer);
		steer *= MAX_SPEED_;
	}

	return steer;
}

sf::Vector2f Mothership::cohesion() const {
	static float neighbourDist = 500;
	sf::Vector2f sum(0, 0);
	float count = 0;

	for (auto itr = flock_.begin();
		itr != flock_.end();
		++itr)
	{
		Mothership* flockMember = *itr;
		if (this != flockMember) {
			//Is the flockmember close enough to us?
			float distance = thor::length(this->getPosition() - flockMember->getPosition());
			if (distance < neighbourDist)
			{
				sum += flockMember->getPosition();
				++count;
			}
		}
	}

	if (count > 1)
	{
		sum /= count;
		sum = thor::unitVector(sum) * MAX_SPEED_;

		return sum;
	}

	return sf::Vector2f(0, 0);

}

sf::Vector2f Mothership::alignment() const {
	static float neighbourDist = 500;

	sf::Vector2f sum(0, 0);
	float count = 0;
	for (auto itr = flock_.begin();
		itr != flock_.end();
		++itr)
	{
		Mothership* flockMember = *itr;
		if (this != flockMember) {
			//Is the flockmember close enough?
			float distance = thor::length(this->getPosition() - flockMember->getPosition());
			if (distance < neighbourDist)
			{
				sum += flockMember->getForward();
				++count;
			}
		}
	}

	sf::Vector2f steer(0, 0);

	//If there are flock members close enough for alignment...
	if (count > 0 && thor::length(sum) > 0)
	{
		sum /= count;

		sum = thor::unitVector(sum);

		return sum;
	}

	return steer;
}

sf::Vector2f Mothership::flock() const {
	sf::Vector2f sep = separation();
	sf::Vector2f ali = alignment();
	sf::Vector2f coh = cohesion();

	//Arbitrarily weight these forces
	sep *= 2.5f;
	ali *= 0.5f;
	coh *= 1.0f;

	sf::Vector2f total = sep + ali + coh;

	if (thor::length(total) > 0)
	{
		total = thor::unitVector(total);
	}

	return total;
}
