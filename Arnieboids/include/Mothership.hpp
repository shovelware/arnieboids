#ifndef _MOTHERSHIP_H
#define _MOTHERSHIP_H

#include "Predator.hpp"
#include "Missile.hpp"

class Mothership : public Ship {
public:
	Mothership(thor::ParticleSystem &particleSystem, std::function<void(Bullet*)> const &fireCallback, std::function<void(Ship*)> spawnCallback, sf::Vector2f position);
	~Mothership();

	void update() override;
	void onCollide(Ship* other) override;

	//! Sets the target for all motherships
	static void setTarget(Ship* target);
protected:
	void updateParticleEmitter() override;
private:
	thor::ParticleSystem &particleSystem_;	//needed to spawn ships
	
	//! Number of calls to update since a Predator was spawned
	unsigned int ticksSinceLastSpawn_;

	//! Amount of ticks that must go by before we will spawn a Predator
	const unsigned int ticksPerSpawn_;

	//! Method used to fire a bullet into the game world
	std::function<void(Bullet*)> fireBullet_;

	//! Method used to spawn a ship into the game world
	std::function<void(Ship*)> spawnShip_;

	//! Target for all motherships
	static Ship* target_;

	enum State {
		WANDER,
		EVADE
	};
	State calculateState() const;

	void evade();
	void wander();
	sf::Vector2f wanderWaypoint_;

	void fire();
	const unsigned int MAX_LIVE_MISSILES;
	unsigned int liveMissiles_;
	void missileDestructed();
};
#endif