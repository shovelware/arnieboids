#include <include/Player.hpp>
#include <iostream>

Player::Player(sf::Vector2f const& position, unsigned int maxHealth) :
Ship(2.f),
MAX_HEALTH_(maxHealth)
{
	setPointCount(3u);

	setPoint(0u, sf::Vector2f(10, 0));
	setPoint(1u, sf::Vector2f(20, 30));
	setPoint(2u, sf::Vector2f(0, 30));

	setOrigin(10.f, 15.f);

	setPosition(position);

	setFillColor(sf::Color::Blue);

	setOutlineThickness(1.f);
	setOutlineColor(sf::Color::Cyan);
}

Player::~Player() {
	
}

float turnDir = 1.f;	//demo
bool shouldTurn = true;	//demo

void Player::update() {

	rotate(turnDir);	//demo purposes (until input is implemented)
	if ((int)getRotation() % 180 < 1) {	//demo
		if (shouldTurn) turnDir *= -1;	//demo
		shouldTurn = !shouldTurn;		//demo
	}									//demo

	//Get rotation in radians
	float rotRads = getRotation() * 0.017f;

	//Calculate forward vector
	forward_.y = -cosf(rotRads);
	forward_.x = sinf(rotRads);

	//Calculate and pplay delta velocity
	velocity_.x += forward_.x * thrust_;
	velocity_.y += forward_.y * thrust_;

	clampToMaxSpeed();

	move(velocity_);
}

void Player::onCollide(Ship* other) {
}