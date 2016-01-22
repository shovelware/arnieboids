#ifndef _SPLASH_H
#define _SPLASH_H

#include "Game.hpp"
#include <thread>

/*
 * \brief Very basic splashscreen that is displayed while the Game object is contructed.
 * Draws a sprite to the screen and constructs the game object.
 * Replaces the blank white screen that is othewise present before anything is drawn.
 */
class SplashScreen {
public:
	SplashScreen() {}
	~SplashScreen() {}

	/*
	 * Loads a splash image and draws it to the window ONCE only.
	 * Then constructs the game object. The splash image remains on the screen until the game begin to draw.
	 * \remarks This is not how one should use unique pointers!
	 */
	void run(std::unique_ptr<Game> &game, sf::RenderWindow &window) const {
		sf::Texture texture;
		texture.loadFromFile("./loading.png");
		
		sf::Sprite sprite = sf::Sprite(texture);

		window.clear();
		window.draw(sprite);
		window.display();

		game = std::make_unique<Game>(window);
	}
};
#endif