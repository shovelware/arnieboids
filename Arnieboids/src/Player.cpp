#include <include/Player.hpp>

Player::Player(sf::Vector2f const& position, unsigned int maxHealth) :
Ship(2.f),
controller_(),
MAX_HEALTH_(maxHealth)
{
	setPosition(position);

	//TODO: set up points and origin
}

Player::~Player() {
	
}

void Player::update() {
	_ASSERT(getPointCount() > 0);
}