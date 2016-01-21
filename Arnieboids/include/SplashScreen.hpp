#ifndef _SPLASH_H
#define _SPLASH_H

#include "Game.hpp"
#include <thread>

class SplashScreen {
public:
	SplashScreen() {}
	~SplashScreen() {}

	//! This is not how to use unique pointers!
	void run(std::unique_ptr<Game> &game, sf::RenderWindow &window) {
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