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
	static std::list<SwarmBoid*> swarm_;
};
#endif