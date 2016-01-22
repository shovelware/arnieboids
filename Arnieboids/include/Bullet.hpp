#ifndef _BULLET_H
#define _BULLET_H

#include <SFML/Graphics.hpp>

#include <Thor/Vectors.hpp>

/*!
* \brief Bullet class. 
* A simple bullet that moves in a direction for a lifetime.
* Dies if it hits something.
*/

class Bullet : public sf::ConvexShape {
public:
	Bullet(sf::Vector2f const &position, sf::Vector2f const &direction, const float speed = 10.f);
	~Bullet();

	//! Moves the bullet forward and checks if it has exceeded time to live
	virtual void update();

	//! Is the bullet active?
	bool isActive() const;

	void setActive(bool active);

	//! Gets the damage that the bullet will deal to something it hits.
	int getDamage() const;

	//! Every bullet either belongs to the player or to an enemy.
	enum Faction {
		PLAYER,
		ENEMY
	};

	Faction getFaction() const;
	void setFaction(Faction faction);

protected:
	//! How far the bullet travels each update
	float speed_;

	//! How much damage a bullet does
	int damage_;

	//! Ticks since bullet has spawned
	unsigned int ticks_;

	//! The direction or heading of the bullet
	sf::Vector2f forward_;

	//! The number of ticks that the bullet will remain alive for
	int lifeTime_;

	//! True if the bullet has been fired and hasn't collided with anything.
	bool active_;

	//! Ticks to seconds
	float tickToSec(unsigned int ticks) const;

	//! Will be PLAYER or ENEMY, depending on who fired it.
	Faction faction_;

};
#endif