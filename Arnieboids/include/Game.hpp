#ifndef _GAME_H
#define _GAME_H

#include <chrono>
#include <list>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Thor/Particles.hpp>
#include <Thor/Animations.hpp>

#include "Ship.hpp"
#include "Player.hpp"
#include "SwarmBoid.hpp"
#include "Bullet.hpp"
#include "Missile.hpp"
#include "KeyInput.hpp"
#include "XController.hpp"
#include "Camera.hpp"
#include "CollisionSystem.hpp"
#include "Asteroid.hpp"
#include "Predator.hpp"
#include "Mothership.hpp"
#include "Star.hpp"
#include "TiledBackdrop.hpp"
#include "Pickup.hpp"

/*!
 * Updates and draws all ships and bullets.
 * Culls dead bullets and ships.
 * 
 * \remarks Chrono members are used for fixed timestep
 */
class Game {
public:
	Game(sf::RenderWindow &window, unsigned int timePerTick = 16U);
	~Game();

	/*!
	 * \brief Implements the main game loop.
	 * While window_ is open:
	 * Calls handleEvents(),
	 * Implements a fixed timestep and calls update() and draw() inside it.
	 */
	int run();

private:
	//! Window that everything will be drawn to
	sf::RenderWindow& window_;

	//! Game bounds
	sf::FloatRect gameBounds_;

	//! All ships in the game
	std::list<Ship*> ships_;

	//! Ship affected by input
	Ship* controlled_;

	//! All bullets in the game
	std::list<Bullet*> bullets_;

	//! All pickups in the game
	std::list<Pickup*> pickups_;

	//! For keyboard input
	KeyInput keyboard_;

	//! For taking input from an Xbox controller
	XController controller_;

	//! Camera that follows ships and has a HUD
	Camera camera_;

	//! Background of stars
	TiledBackdrop backdrop_;

	//! Used to check for collision between shapes
	CollisionSystem collisionSystem_;

	//! Manages all particles
	thor::ParticleSystem particleSystem_;

	//! Texture for all particles
	sf::Texture particleTexture_;

	std::function<void(Bullet*)> fireBulletCallback_;
	std::function<void(Ship*)> spawnShipCallback_;

	std::chrono::steady_clock tickClock_;
	std::chrono::milliseconds timePerTick_;	//!< Milliseconds per update/draw tick
	std::chrono::time_point<std::chrono::system_clock> timeOfLastTick_;	//!< Time at which the last update/draw tick occurred.

	//! Wraps a passed convexShape to the opposite side of game bounds
	void wrap(sf::ConvexShape* cs);

	//! Resets behaviours dependant on player existing
	void playerDeath();

	//! Clears the game of entities
	void clearLists();

	//! Resets the game
	void reset();

	//! Adds a swarm of [amount] boids in circle around [pos] with [radius]
	void addBoidSwarm(sf::Vector2f const &pos, float amount, float radius);

	sf::SoundBuffer erasedSBuffer_, mineSBuffer_, planSBuffer_, backSBuffer_;
	sf::Sound erasedSound_, mineSound_, planSound_, backSound_;
	sf::Music bgMusic_;

	/*!
	 * Polls the RenderWindow for events and
	 * handles these events accordingly.
	 */
	void handleEvents();

	/*!
	 * Calls Update on all bullets and ships.
	 */
	void update();

	/*!
	 * Draws all bullets and ships.
	 */
	void draw();
};
#endif