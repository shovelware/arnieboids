#include <include/Predator.hpp>

std::list<Predator*> Predator::flock_ = std::list<Predator*>();
Ship* Predator::prey_ = nullptr;

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

	flock_.insert(flock_.end(), this);

	velocity_ = thor::Distributions::deflect(sf::Vector2f(1, 1), 360)();
}

Predator::~Predator() {
	flock_.erase(std::find(flock_.begin(), flock_.end(), this));
}

void Predator::update() {
	turnToward(flock());
	thrust();
	Ship::update();
	move(velocity_);
}

void Predator::setPrey(Ship* prey) {
	prey_ = prey;
}

sf::Vector2f Predator::separation() const {
	static float desiredSeparation = 100;

	sf::Vector2f steer(0, 0);
	float count = 0;

	//For every predator in the flock, check if it's too close
	for (auto itr = flock_.begin();
		itr != flock_.end();
		++itr)
	{
		Predator* flockMember = *itr;
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

	//Avoid colliding with prey_
	sf::Vector2f diff = this->getPosition() - prey_->getPosition();
	float distance = thor::length(diff);
	if (distance < desiredSeparation) {
		diff = thor::unitVector(diff);
		diff /= distance;
		steer += diff;
		++count;
	}

	//average out the steering
	if (count > 1)
	{
		steer /= count;
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

sf::Vector2f Predator::cohesion() const {
	static float neighbourDist = 500;
	sf::Vector2f sum(0, 0);
	float count = 0;
	
	for (auto itr = flock_.begin();
		itr != flock_.end();
		++itr)
	{
		Predator* flockMember = *itr;
		if (this != flockMember) {
			//Is the flockmember close enough to us?
			float distance = thor::length(this->getPosition() - flockMember->getPosition());
			if (distance < neighbourDist)
			{
				sum += this->getPosition() - flockMember->getPosition();
				++count;
			}
		}
	}

	//Tend toward prey_
	sum += prey_->getPosition();
	++count;

	if (count > 1)
	{
		sum /= count;
		sum = thor::unitVector(sum) * thrust_;
		
		return sum;
	}

	return sf::Vector2f(0, 0);
	
}

sf::Vector2f Predator::alignment() const {
	static float neighbourDist = 500;

	sf::Vector2f sum(0, 0);
	float count = 0;
	for (auto itr = flock_.begin();
		itr != flock_.end();
		++itr)
	{
		Predator* flockMember = *itr;
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

	//Align to prey_
	float distance = thor::length(this->getPosition() - prey_->getPosition());
	if (distance < neighbourDist)
	{
		sum += prey_->getForward();
		++count;
	}

	sf::Vector2f steer(0, 0);

	//If there are flock members close enough for alignment...
	if (count > 0)
	{
		sum /= count;

		sum = thor::unitVector(sum);

		return sum;
	}

	return steer;
}

sf::Vector2f Predator::seek(sf::Vector2f const& pos) const {
	sf::Vector2f desired;
	desired -= pos;	//A vector pointing from our location to the target

	//Normalize and scale to max speed
	desired = thor::unitVector(desired);
	desired *= MAX_SPEED_;

	//Steering = desired - velocity
	return desired - velocity_;
}

void Predator::onCollide(Ship* other) {
}

sf::Vector2f Predator::flock() const {
	sf::Vector2f sep = separation();
	sf::Vector2f ali = alignment();
	sf::Vector2f coh = cohesion();

	//Arbitrarily weight these forces
	sep *= 3.0f;
	ali *= 1.0f;
	coh *= 2.0f;

	return sep + ali + coh;
}