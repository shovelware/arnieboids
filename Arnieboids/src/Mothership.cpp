#include <include/Mothership.hpp>

Ship* Mothership::target_ = nullptr;

Mothership::Mothership(thor::ParticleSystem& particleSystem, std::function<void(Bullet*)> const& fireCallback, std::function<void(Ship*)> spawnCallback, sf::Vector2f position) :
Ship(particleSystem, position, 2.f, 4u),
particleSystem_(particleSystem),
ticksSinceLastSpawn_(rand() % 100),	//offset spawn times a bit
ticksPerSpawn_(1000u),
fireBullet_(fireCallback),
spawnShip_(spawnCallback)
{
	setPointCount(6u);

	float radius = 50;
	float deltaTheta = thor::toRadian(static_cast<float>(360 / 7));
	float theta = 0;

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
	if (++ticksSinceLastSpawn_ >= ticksPerSpawn_)
	{
		//spawnShip_(new Predator(particleSystem_, fireBullet_, getPosition()));
		ticksSinceLastSpawn_ = 0u;
	}

	switch (calculateState())
	{
	case WANDER:
		wander();
		break;
	case EVADE:
		evade();
		break;
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

void Mothership::evade() {
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

	turnToward(steer);
}

void Mothership::wander() {
	sf::Vector2f displacement = getPosition() - wanderWaypoint_;
	float distance = thor::length(displacement);
	if (distance < 100 || distance > 1000)
	{
		wanderWaypoint_ = getPosition() + thor::Distributions::deflect(forward_ * 1000.f, 90)();
	}

	turnToward(displacement);
}
