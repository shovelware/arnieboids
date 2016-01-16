#ifndef _SWARM_BOID_H
#define _SWARM_BOID_H

#include <list>

#include "Ship.hpp"

class SwarmBoid : public Ship {
public:
	SwarmBoid(sf::Vector2f position);
	~SwarmBoid();
#
	//! Calls swarm()
	void update() override;

	//! Takes one damage.
	void onCollide(Ship* other) override;

	//! Sets the target for all swarm boids. They will tend toward this target.
	static void setSwarmTarget(Ship* target);
private:
	//! A list of all swarm boids. Boids add themselves in the ctor and remove themselves in the dtor
	static std::list<SwarmBoid*> swarm_;

	//! The ship that the swarm will tend toward, if any.
	static Ship* swarmTarget_;

	//! How close boids need to be before they will swarm with eachother
	static float swarmThresholdRadius_;

	void swarm();

	sf::Vector2f LenardJonesPotential(const Ship * const other, int& count) const;
};
#endif