#include <include/Player.hpp>
#include <iostream>

Player::Player(sf::Vector2f const& position, unsigned int maxHealth) :
Ship(position, 3.f),
MAX_HEALTH_(maxHealth)
{
	setPointCount(3u);

	// Shape:
	//	   / \
	//	  /	  \
	//	 /	   \
	//	/_______\

	setPoint(0u, sf::Vector2f(10, 0));
	setPoint(1u, sf::Vector2f(20, 30));
	setPoint(2u, sf::Vector2f(0, 30));

	setOrigin(10.f, 15.f);

	setFillColor(sf::Color::Blue);

	setOutlineThickness(1.f);
	setOutlineColor(sf::Color::Cyan);
}

Player::~Player() {
	
}

float turnDir = 1.f;	//demo
bool shouldTurn = true;	//demo

void Player::update() {

	//rotate(turnDir);	//demo purposes (until input is implemented)
	if ((int)getRotation() % 180 < 1) {	//demo
		if (shouldTurn) turnDir *= -1;	//demo
		shouldTurn = !shouldTurn;		//demo
	}									//demo

	thrust();

	//move(velocity_);
}

void Player::onCollide(Ship* other) {
}