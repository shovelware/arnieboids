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

	/*! \brief 
	 * \returns The desired heading after alignment.
	 */
	sf::Vector2f alignment() const;

	/*! \brief
	 * \returns
	 */
	sf::Vector2f seek(sf::Vector2f const &pos) const;

	sf::Vector2f flock() const;
};
#endif