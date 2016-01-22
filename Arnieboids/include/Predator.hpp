#ifndef _PREDATOR_H
#define _PREDATOR_H

#include "Ship.hpp"
#include "Bullet.hpp"

/*
 * \brief Flocks with other predators and hunts Player ship.
 */
class Predator : public Ship {
public:
	Predator(thor::ParticleSystem &particleSystem, std::function<void(Bullet*)> const &fireCallback, sf::Vector2f position);
	~Predator();

	//! Makes the ship flock toward other predators and prey_. Fires bullet at prey_ if facing toward it.
	void update() override;

	//! Takes damage if colliding with player.
	void onCollide(Ship* other) override;

	static void setPrey(Ship* prey);
private:
	static Ship* prey_;
	static std::list<Predator*> flock_;
	std::function<void(Bullet*)> fireCallback_;

	/*! \brief Keeps ships from piling on top of each other
	 * \returns The desired position after separation.
	 */
	sf::Vector2f separation() const;

	/*! \brief Keeps the ship moving in the same direction as nearby flockmates.
	 * Finds the averagelocation of nearby flockmates and manipulates
	 * the steering force to move in that direction.
	 * \returns The desired position after cohesion.
	 */
	sf::Vector2f cohesion() const;

	/*!
	 * \returns The desired heading after alignment.
	 */
	sf::Vector2f alignment() const;

	/*
	 * Calls separation(), cohesion() and alignment().
	 * Tends toward prey_.
	 * Weights the results.
	 * \returns A vector that is the weighted sum of flocking algorithms and prey_ tendency.
	 */
	sf::Vector2f flock() const;
};
#endif