#ifndef _PREDATOR_H
#define _PREDATOR_H

#include "Ship.hpp"
#include "Bullet.hpp"

class Predator : public Ship {
public:
	Predator(thor::ParticleSystem &particleSystem, std::function<void(Bullet*)> const &fireCallback, sf::Vector2f position);
	~Predator();

	void update() override;
	void onCollide(Ship* other) override;
private:
	std::function<void(Bullet*)> fireCallback_;

	void separation();
	void cohesion();
	void alignment();

	void flock();
};
#endif