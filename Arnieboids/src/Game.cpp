#include <include/Game.hpp>

#pragma region PublicMemberFunctions
Game::Game(unsigned int winWidth, unsigned int winHeight, unsigned int timePerTick) :
window_(sf::VideoMode(winWidth, winHeight), "ArnieBoids", sf::Style::Default, sf::ContextSettings(0u, 0u, 8u)),	//AntiAliasing level: 8
camera_(sf::Vector2u(winWidth, winHeight)),
ships_(),
bullets_(),
controller_(),
tickClock_(),
timePerTick_(timePerTick),
timeOfLastTick_(tickClock_.now() - timePerTick_)
{
	ships_.push_back(new Player(sf::Vector2f(200.f, 200.f)));
	camera_.setTarget(*(--ships_.end()));
}

Game::~Game() {
	for (auto itr = bullets_.begin(), end = bullets_.end();
		itr != end;
		++itr)
	{
		//delete *itr;
	}

	for (auto itr = ships_.begin(), end = ships_.end();
		itr != end;
		++itr)
	{
		delete *itr;
	}
}

int Game::run() {

	while (window_.isOpen()) {
		//Handle events from the RenderWindow
		handleEvents();

		//If time since last tick is greater or equal to the time per tick...
		while (tickClock_.now() - timeOfLastTick_ >= timePerTick_)
		{
			timeOfLastTick_ += timePerTick_;

			//Update bullets and ships.
			update();

			//Draw bullets and ships.
			draw();
		}
	}

	return 0;
}
#pragma endregion

#pragma region PrivateMemberFunctions
void Game::handleEvents() {
	sf::Event evnt;
	while (window_.pollEvent(evnt))
	{
		switch (evnt.type)
		{
		case sf::Event::Closed: break;
		case sf::Event::Resized: break;
		case sf::Event::LostFocus: break;
		case sf::Event::GainedFocus: break;
		case sf::Event::TextEntered: break;
		case sf::Event::KeyPressed: break;
		case sf::Event::KeyReleased: break;
		case sf::Event::MouseWheelMoved: break;
		case sf::Event::MouseWheelScrolled: break;
		case sf::Event::MouseButtonPressed: break;
		case sf::Event::MouseButtonReleased: break;
		case sf::Event::MouseMoved: break;
		case sf::Event::MouseEntered: break;
		case sf::Event::MouseLeft: break;
		case sf::Event::JoystickButtonPressed: break;
		case sf::Event::JoystickButtonReleased: break;
		case sf::Event::JoystickMoved: break;
		case sf::Event::JoystickConnected: break;
		case sf::Event::JoystickDisconnected: break;
		case sf::Event::TouchBegan: break;
		case sf::Event::TouchMoved: break;
		case sf::Event::TouchEnded: break;
		case sf::Event::SensorChanged: break;
		case sf::Event::Count: break;
		default: break;
		}
	}//end while
}

void Game::update() {

	camera_.update();

	for (auto itr = bullets_.begin(), end = bullets_.end();
		itr != end;
		++itr)
	{
		//(*itr)->update();
	}

	for (auto itr = ships_.begin(), end = ships_.end();
		itr != end;
		++itr)
	{
		(*itr)->update();
	}
}

void Game::draw() {
	window_.clear();
	window_.setView(camera_);

	for (auto itr = bullets_.begin(), end = bullets_.end();
		itr != end;
		++itr)
	{
		//window_.draw(**itr);
	}

	for (auto itr = ships_.begin(), end = ships_.end();
		itr != end;
		++itr)
	{
		window_.draw(**itr);
	}

	window_.display();
}
#pragma endregion
