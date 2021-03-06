#include <include/Bullet.hpp>

Bullet::Bullet(sf::Vector2f const &position, sf::Vector2f const &direction, const float speed) :
forward_(thor::unitVector(direction)),
speed_(speed),
lifeTime_(1),
active_(true),
ticks_(0),
damage_(1),
faction_(ENEMY)
{
	setPointCount(4u);

	setPoint(0u, sf::Vector2f(0, 0));
	setPoint(1u, sf::Vector2f(5, 0));
	setPoint(2u, sf::Vector2f(5, 5));
	setPoint(3u, sf::Vector2f(0, 5));

	setOrigin(getLocalBounds().width * 0.5f, getLocalBounds().height * 0.5f);

	setFillColor(sf::Color::Magenta);

	setOutlineThickness(1.f);
	setOutlineColor(sf::Color::Cyan);

	setPosition(position);
}

Bullet::~Bullet() {}

void Bullet::update() {
	if (active_)
	{
		rotate(10.f);
		move(forward_ * speed_);
		
		ticks_ = (ticks_ + 1) % INT_MAX;

		if (tickToSec(ticks_) >= lifeTime_)
		{
			active_ = false;
		}
	}
}

bool Bullet::isActive() const
{
	return active_;
}

void Bullet::setActive(bool active) {
	active_ = active;
}

int Bullet::getDamage() const
{
	return damage_;
}

Bullet::Faction Bullet::getFaction() const {
	return faction_;
}

void Bullet::setFaction(Faction faction) {
	faction_ = faction;
}

float Bullet::tickToSec(unsigned int ticks) const
{
	return (16.f / 1000.f) * ticks;
}
