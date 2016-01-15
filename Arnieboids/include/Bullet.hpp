#ifndef _BULLET_H
#define _BULLET_H

#include <SFML/Graphics.hpp>

#include <Thor/Vectors.hpp>

class Bullet : public sf::ConvexShape {
public:
	Bullet(sf::Vector2f const &position, sf::Vector2f const &direction, const float speed = 5.f);
	~Bullet();

	virtual void update();
	//! Is the bullet active?
	bool isActive() const;

protected:
	//! How far the bullet travels each update
	float speed_;

	//! Ticks since bullet has spawned
	unsigned int ticks_;

	//! The direction or heading of the bullet
	sf::Vector2f forward_;

	int lifeTime_;

	bool active_;

	//! Ticks to seconds
	float tickToSec(unsigned int ticks) const;

};
#endif