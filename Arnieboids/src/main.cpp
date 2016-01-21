#if _DEBUG
#pragma comment(lib, "sfml-graphics-d.lib")
#pragma comment(lib, "sfml-main-d.lib")
#pragma comment(lib, "sfml-network-d.lib")
#pragma comment(lib, "sfml-system-d.lib")
#pragma comment(lib, "sfml-window-d.lib")
#pragma comment(lib, "sfml-audio-d.lib")
#pragma comment(lib, "thor-d.lib")
#else
#pragma comment(lib, "sfml-graphics.lib")
#pragma comment(lib, "sfml-main.lib")
#pragma comment(lib, "sfml-network.lib")
#pragma comment(lib, "sfml-system.lib")
#pragma comment(lib, "sfml-window.lib")
#pragma comment(lib, "sfml-audio.lib")
#pragma comment(lib, "thor.lib")
#endif


#include <include/Game.hpp>
#include <include/SplashScreen.hpp>


int main()
{
	sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(800u, 800u), L"ArnieBoids", sf::Style::Titlebar, sf::ContextSettings(0u, 0u, 8u));	//8x antialiasing

	std::unique_ptr<Game> game;

	SplashScreen splash;

	splash.run(game, window);

	return game->run();
}
