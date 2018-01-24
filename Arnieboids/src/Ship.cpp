#include <include/Ship.hpp>

sf::FloatRect Ship::gameBounds_ = sf::FloatRect();

// returns sin(theta)*length(a)*length(b), where theta = the angle between vectors a,b
inline float perpDot(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return (a.y*b.x) - (a.x*b.y);
}

Ship::Ship(thor::ParticleSystem &particleSystem, sf::Vector2f const &position, float maxSpeed, unsigned int health) :
MAX_SPEED_(maxSpeed),
MAX_HEALTH_(health),
velocity_(0.f, 0.f),
accel_(0.f, 0.f),
health_(MAX_HEALTH_),
forward_(0.f, -1.f),
turnSpeed_(2.f),
thrust_(0.1f),
refireTime_(2.f),
coolDown_(0),
radarRange_(500),
ticks_(0),
particleAngleVariance_(15.f)
{
	setPosition(position);
	
	//hold connection to particle system
	connection_ = particleSystem.addEmitter(thor::refEmitter(particleEmitter_));
	//Derived classes' ctor should specify most details of emitter
	particleEmitter_.setParticleLifetime(sf::seconds(1.f));
	particleEmitter_.setParticleVelocity(sf::Vector2f());
	particleEmitter_.setEmissionRate(20.f);
	particleEmitter_.setParticleRotationSpeed(thor::Distributions::uniform(-100.f, 100.f));
	particleEmitter_.setParticleScale(sf::Vector2f(0.2f, 0.2f));

	debug_rect_ = sf::RectangleShape(sf::Vector2f(1, 1));
	debug_rect_.setOutlineColor(sf::Color::Magenta);
	debug_rect_.setOutlineThickness(0.1f);
}

Ship::~Ship() {
	//disconnect from particle system
	connection_.disconnect();
}

void Ship::update()
{
	//Apply friction	
	velocity_ *= 0.99f;

	//Apply delta velocity
	velocity_ += accel_;
	clampToMaxSpeed();

	updateParticleEmitter();

	accel_.x = 0;
	accel_.y = 0;

	debug_rect_.setPosition(getPosition() - sf::Vector2f(0.5f, 0.5f));
}

void Ship::takeDamage(unsigned amount) {
	if (amount <= health_)
	{
		health_ -= amount;
	}
	else
	{
		health_ = 0U;
	}
}

bool Ship::isDead() const {
	return health_ <= 0;
}

void Ship::clampToMaxSpeed() {
	if (thor::length(velocity_) > MAX_SPEED_)
	{
		velocity_ = thor::unitVector(velocity_);
		velocity_ *= MAX_SPEED_;
	}
}

void Ship::thrust() {

	//Clear previous accel
	accel_.x += forward_.x * thrust_;
	accel_.y += forward_.y * thrust_;
}

void Ship::brake() {
	if (velocity_.x != 0 || velocity_.y != 0)
	{
		accel_.x -= velocity_.x * thrust_ * 0.5f;
		accel_.y -= velocity_.y * thrust_ * 0.5f;
	}
}

void Ship::turnLeft()
{
	rotate(-turnSpeed_);

	//Get rotation in radians
	float rotRads = thor::toRadian(getRotation());

	//Calculate forward vector
	forward_.y = -cosf(rotRads);
	forward_.x = sinf(rotRads);
}

void Ship::turnRight()
{
	rotate(turnSpeed_);

	//Get rotation in radians
	float rotRads = thor::toRadian(getRotation());

	//Calculate forward vector
	forward_.y = -cosf(rotRads);
	forward_.x = sinf(rotRads);
}

bool Ship::trigger()
{
	bool fired = false;
	
	//Handle firing logic, return success here
	if (coolDown_ <= 0)
	{
		coolDown_ = refireTime_;
		fired = true;
	}

	return fired;
}

void Ship::turnToward(sf::Vector2f const& direction) {
	//find out which way we should turn to face target
	float d = perpDot(direction, forward_);
	if (d > 0.f) {
		//turn clockwise
		turnRight();
	}
	else {
		//turn counter-clockwise
		turnLeft();
	}
}

sf::Vector2f Ship::getForward() const
{
	return forward_;
}

sf::Vector2f Ship::getVelocity() const
{
	return velocity_;
}

sf::Vector2f Ship::getClosestPosition(sf::Vector2f const &point) const
{
	auto& const absolutePosition = getPosition();
	
	sf::Vector2f shortestXTranslation;
	if (point.x > absolutePosition.x) {
		shortestXTranslation = absolutePosition + sf::Vector2f(gameBounds_.width, 0.f);
	}
	else {
		shortestXTranslation = absolutePosition - sf::Vector2f(gameBounds_.width, 0.f);
	}


	sf::Vector2f shortestYTranslation;
	if (point.y > absolutePosition.y) {
		shortestYTranslation = absolutePosition + sf::Vector2f(0.f, gameBounds_.height);
	}
	else {
		shortestYTranslation = absolutePosition - sf::Vector2f(0.f, gameBounds_.height);
	}

	auto closestPosition = absolutePosition;
	auto const absoluteDistance = thor::length(point - absolutePosition);
	if (thor::length(point - shortestXTranslation) < absoluteDistance) {
		closestPosition.x = shortestXTranslation.x;
	}
	if (thor::length(point - shortestYTranslation) < absoluteDistance) {
		closestPosition.y = shortestYTranslation.y;
	}

	return closestPosition;
}

sf::Vector2f Ship::getShortestDisplacement(sf::Vector2f const & point) const
{
	return getClosestPosition(point) - point;
}

void Ship::setRadarRange(float range)
{
	radarRange_ = range;
}

float Ship::getRadarRange() const
{
	return radarRange_;
}

int Ship::getHealth() const
{
	return health_;
}

void Ship::addHealth(unsigned int health)
{
	//If the addition is less than max
	if (health_ + health < MAX_HEALTH_)
	{
		health_ += health;
	}

	else health_ = MAX_HEALTH_;
}

unsigned int Ship::getMaxHealth() const
{
	return MAX_HEALTH_;
}

void Ship::setGameBounds(sf::FloatRect const & gameBounds)
{
	gameBounds_ = gameBounds;
}

float Ship::tickToSec(unsigned int ticks) const
{
	return (16.f / 1000.f) * ticks;
}

void Ship::updateParticleEmitter() 
{

	float acc = thor::length(accel_) * 100;

	particleEmitter_.setEmissionRate(acc);

	if (acc != 0)
	{
		sf::Vector2f vAcc = thor::unitVector(accel_);
		sf::Vector2f vEng = thor::unitVector(-forward_);

		//If our accel is not from engine, draw RCS particles
		if (acos(thor::dotProduct(vAcc, vEng)) > 10)
		{
			particleEmitter_.setParticlePosition(getPosition() + (thor::unitVector(accel_) * getLocalBounds().height * 0.4f));
			particleEmitter_.setParticleVelocity(thor::Distributions::deflect(-forward_ * (acc * 10.f), particleAngleVariance_));
			particleEmitter_.setParticleScale(sf::Vector2f(0.1f, 0.1f));
		}

		//Otherwise draw regular engine particles
		else
		{
			particleEmitter_.setParticlePosition(getPosition() + (-forward_ * getLocalBounds().height * 0.4f));
			particleEmitter_.setParticleVelocity(thor::Distributions::deflect(thor::unitVector(accel_) * (acc * 10.f), particleAngleVariance_));
			particleEmitter_.setParticleScale(sf::Vector2f(0.2f, 0.2f));
		}
	}
}
