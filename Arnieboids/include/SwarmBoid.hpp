#ifndef _SWARM_BOID_H
#define _SWARM_BOID_H

#include <list>

#include "Ship.hpp"

class SwarmBoid : public Ship {
public:
	SwarmBoid(sf::Vector2f position);
	~SwarmBoid();

	void update() override;
	void onCollide(Ship* other) override;
private:
	//! A list of all swarm boids. Boids add themselves in the ctor and remove themselves in the dtor
	static std::list<SwarmBoid*> swarm_;

	//! How close boids need to be before they will swarm with eachother
	static float swarmThresholdRadius_;

	void swarm();
};
#endif