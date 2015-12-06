#ifndef _GAME_H
#define _GAME_H

#include <chrono>
#include <list>

#include <SFML/Graphics.hpp>

#include "Ship.hpp"
#include "Player.hpp"
#include "XController.hpp"
#include "Camera.hpp"

class Bullet;

/*!
 * Updates and draws all ships and bullets.
 * Culls dead bullets and ships.
 * 
 * \remarks Chrono members are used for fixed timestep
 */
class Game {
public:
	Game(unsigned int winWidth = 800U, unsigned int winHeight = 800U, unsigned int timePerTick = 16U);
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
	sf::RenderWindow window_;

	//! All ships in the game
	std::list<Ship*> ships_;

	//! All bullets in the game
	std::list<Bullet*> bullets_;

	//! For taking input from an Xbox controller
	XController controller_;

	//! Camera that follows ships and has a HUD
	Camera camera_;

	std::chrono::steady_clock tickClock_;
	std::chrono::milliseconds timePerTick_;	//!< Milliseconds per update/draw tick
	std::chrono::time_point<std::chrono::system_clock> timeOfLastTick_;	//!< Time at which the last update/draw tick occurred.

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