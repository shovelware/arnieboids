#include <include/Predator.hpp>
#include <include/Player.hpp>

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
	turnSpeed_ *= 2;
}

Predator::~Predator() {
	flock_.erase(std::find(flock_.begin(), flock_.end(), this));
}

void Predator::update() {
	sf::Vector2f desiredPosition = flock();
	turnToward(desiredPosition);
	thrust();
	float dotProd = 0;

	if (prey_)
	{
		auto const displacement = prey_->getShortestDisplacement(getPosition());
		dotProd = thor::dotProduct(forward_, thor::unitVector(displacement));
	}

	if (dotProd > 0.99)
	{
		if (trigger())
			fireCallback_(new Bullet(getPosition(), forward_));
	}

	ticks_ = (ticks_ + 1) % INT_MAX;

	//Cool weapon
	coolDown_ -= tickToSec(1);

	Ship::update();
	move(velocity_);
}

void Predator::setPrey(Ship* prey) {
	prey_ = prey;
}

sf::Vector2f Predator::separation() const {
	static float desiredSeparation = 200;

	sf::Vector2f steer(0, 0);
	float count = 0;

	//For every predator in the flock, check if it's too close
	for (auto itr = flock_.begin();
		itr != flock_.end();
		++itr)
	{
		Predator* flockMember = *itr;
		if (this != flockMember) {
			sf::Vector2f diff = flockMember->getShortestDisplacement(getPosition());
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
	if (prey_)
	{
		sf::Vector2f diff = prey_->getShortestDisplacement(getPosition());
		float distance = thor::length(diff);
		if (distance < desiredSeparation) {
			diff = thor::unitVector(diff);
			diff /= distance;
			steer += diff;
			++count;
			}
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
	static float neighbourDist = 2000;
	sf::Vector2f sum(0, 0);
	float count = 0;
	
	for (auto itr = flock_.begin();
		itr != flock_.end();
		++itr)
	{
		Predator* flockMember = *itr;
		if (this != flockMember) {
			//Is the flockmember close enough to us?
			auto const memberPos = flockMember->getClosestPosition(getPosition());
			float distance = thor::length(this->getPosition() - memberPos);
			if (distance < neighbourDist)
			{
				sum += memberPos;
				++count;
			}
		}
	}

	////Tend toward prey_
	//sum += prey_->getPosition();
	//++count;

	if (count > 1)
	{
		sum /= count;
		sum = thor::unitVector(sum) * MAX_SPEED_;
		
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
			float distance = thor::length(flockMember->getShortestDisplacement(getPosition()));
			if (distance < neighbourDist)
			{
				sum += flockMember->getForward();
				++count;
			}
		}
	}

	////Align to prey_
	//float distance = thor::length(this->getPosition() - prey_->getPosition());
	//if (distance < neighbourDist)
	//{
	//	sum += prey_->getForward();
	//	++count;
	//}

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

void Predator::onCollide(Ship* other) {
	if (dynamic_cast<Player*>(other))
	{
		takeDamage();
	}
}

sf::Vector2f Predator::flock() const {
	sf::Vector2f sep = separation();
	sf::Vector2f ali = alignment();
	sf::Vector2f coh = cohesion();

	//Arbitrarily weight these forces
	sep *= 2.5f;
	ali *= 0.5f;
	coh *= 1.0f;
	sf::Vector2f pry(0, 0);
	if (prey_)
	{
		pry = thor::unitVector(prey_->getShortestDisplacement(getPosition()));
	}
	pry *= 5.0f;

	sf::Vector2f total = sep + ali + coh + pry;

	if (thor::length(total) > 0)
	{
		total = thor::unitVector(total);
	}

	return total;
}
