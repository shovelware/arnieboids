#ifndef _SWARM_BOID_H
#define _SWARM_BOID_H

#include <list>

#include "Ship.hpp"

/*
 * \brief Ship with swarming behaviour.
 * Tends toward swarmTarget_ position and plots intercept course.
 */
class SwarmBoid : public Ship {
public:
	SwarmBoid(thor::ParticleSystem &particleSystem, sf::Vector2f position);
	~SwarmBoid();

	//! Calls swarm()
	void update() override;

	//! Takes one damage if other is the Player.
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

	/*
	 * \brief Uses LenardJonesPotential() to figure out where boid should be, relative to other boids and swarmTarget_.
	 * Calls LendardJonesPotential() on every other boid in swarm_ and on swarmTarget_.
	 */
	void swarm();

	/*
	 * \brief Implementation of Lenard-Jones Potential algorithm.
	 * \param other The ship to run the algorithm on.
	 * \param count[out] A reference to an int. Incremented if the algorithm allows itself to run on the ship.
	 * \remarks If the swarmTarget_ is moving, the position used is the target's forward vector extrpolated by it's velocity. Allows boids to intercept target.
	 * \returns A vector of where the boid should go.
	 */
	sf::Vector2f LenardJonesPotential(const Ship * const other, int& count) const;

	/*
	 * \brief Calculate a new position that is a given distance away from the starting point in a given direction.
	 * \param position The point to be extrapolated.
	 * \param direction The direction to be extrpolated in.
	 * \param distance The amount to extrapolate.
	 */
	sf::Vector2f extrapolate(sf::Vector2f const& position, sf::Vector2f const &direction, float distance) const;
};
#endif