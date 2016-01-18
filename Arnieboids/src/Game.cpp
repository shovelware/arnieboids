#include <include/Game.hpp>

inline float randFloat(float MAX) { return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / MAX)); };
inline float randFloat(float MIN, float MAX) { return MIN + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (MAX - MIN))); };

#pragma region PublicMemberFunctions
Game::Game(unsigned int winWidth, unsigned int winHeight, unsigned int timePerTick) :
window_(sf::VideoMode(winWidth, winHeight), "ArnieBoids", sf::Style::Default, sf::ContextSettings(0u, 0u, 8u)),	//AntiAliasing level: 8
camera_(sf::Vector2u(winWidth, winHeight)),
ships_(),
bullets_(),
controller_(),
tickClock_(),
timePerTick_(timePerTick),
timeOfLastTick_(tickClock_.now() - timePerTick_),
collisionSystem_(ships_, bullets_),
keyboard_(),
particleSystem_(),
particleTexture_()
{
	float w = winWidth;
	float h = winHeight;

	genStars(-w, w * 2, -h, h * 2, 1000U, 1, 5);

	ships_.push_back(new SwarmBoid(particleSystem_, sf::Vector2f(100.f, 100.f)));
	ships_.push_back(new SwarmBoid(particleSystem_, sf::Vector2f(300.f, 300.f)));
	ships_.push_back(new SwarmBoid(particleSystem_, sf::Vector2f(600.f, 400.f)));
	ships_.push_back(new SwarmBoid(particleSystem_, sf::Vector2f(700.f, 300.f)));
	ships_.push_back(new SwarmBoid(particleSystem_, sf::Vector2f(160.f, 333.f)));
	ships_.push_back(new SwarmBoid(particleSystem_, sf::Vector2f(360.f, 800.f)));
	ships_.push_back(new SwarmBoid(particleSystem_, sf::Vector2f(660.f, 100.f)));
	ships_.push_back(new SwarmBoid(particleSystem_, sf::Vector2f(760.f, 100.f)));

	ships_.push_back(new Player(particleSystem_, sf::Vector2f(200.f, 200.f)));
	camera_.setTarget(*ships_.rbegin());
	controlled_ = (*ships_.rbegin());
	SwarmBoid::setSwarmTarget(*ships_.rbegin());

	bullets_.push_back(new Missile(particleSystem_, *ships_.rbegin(), sf::Vector2f(100.f, 100.f), sf::Vector2f(0.1f, 1.f)));
	bullets_.push_back(new Missile(particleSystem_, *ships_.rbegin(), sf::Vector2f(500.f, 500.f), sf::Vector2f(-1.f, 0.f)));

	ships_.push_back(new Asteroid(particleSystem_, sf::Vector2f(rand() % 500, rand() % 500)));
	ships_.push_back(new Asteroid(particleSystem_, sf::Vector2f(rand() % 500, rand() % 500)));
	ships_.push_back(new Asteroid(particleSystem_, sf::Vector2f(rand() % 500, rand() % 500)));
	ships_.push_back(new Asteroid(particleSystem_, sf::Vector2f(rand() % 500, rand() % 500)));
	ships_.push_back(new Asteroid(particleSystem_, sf::Vector2f(rand() % 500, rand() % 500)));
	ships_.push_back(new Asteroid(particleSystem_, sf::Vector2f(rand() % 500, rand() % 500)));

	bullets_.push_back(new Bullet(sf::Vector2f(100.f, 100.f), sf::Vector2f(1.f, 0.1f)));
	bullets_.push_back(new Bullet(sf::Vector2f(1000.f, 1000.f), sf::Vector2f(-1.f, -1.f)));
	bullets_.push_back(new Bullet(sf::Vector2f(0.f, 0.f), sf::Vector2f(200.f, 200.f)));
	bullets_.push_back(new Bullet(sf::Vector2f(0.f, 0.f), sf::Vector2f(6.f, 1.f)));

	//fade particles out at end of life
	particleSystem_.addAffector(thor::AnimationAffector(thor::FadeAnimation(0.f, 0.25f)));

	particleTexture_.loadFromFile("./particle.png");
	particleSystem_.setTexture(particleTexture_);
}

Game::~Game() {
	for (auto itr = bullets_.begin(), end = bullets_.end();
		itr != end;
		++itr)
	{
		delete *itr;
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

		//If time since last tick is greater or equal to the time per tick...
		while (tickClock_.now() - timeOfLastTick_ >= timePerTick_)
		{
			timeOfLastTick_ += timePerTick_;

			//Handle events from the RenderWindow
			handleEvents();

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
		case sf::Event::Closed:
			window_.close();
			break;
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
	collisionSystem_.Check();

	particleSystem_.update(sf::milliseconds(16));

	camera_.update();
	keyboard_.update();

	for (auto itr = bullets_.begin(), end = bullets_.end();
	itr != end; /*No increment*/)
	{
		if ((*itr)->isActive()) {
		
			(*itr++)->update();
		}
		
		else {
		
			float emissionRate;
			if(dynamic_cast<Missile*>(*itr)) {
				emissionRate = 100.f;
			} else {
				emissionRate = 20.f;
			}
		
		    //generate a particle explosion at the dead ship's position
			thor::UniversalEmitter emitter;
			emitter.setParticlePosition((*itr)->getPosition());
			emitter.setEmissionRate(100.f);
			emitter.setParticleScale(sf::Vector2f(0.25f, 0.25f));
			emitter.setParticleColor((*itr)->getFillColor());
			emitter.setParticleVelocity([](){ return thor::Distributions::deflect(sf::Vector2f(1.f,1.f), 360.f)() * thor::Distributions::uniform(50.f, 100.f)(); });
			particleSystem_.addEmitter(emitter, sf::seconds(0.1f));
			
			delete *itr;
			bullets_.erase(itr++);
		}
	}

	for (auto itr = ships_.begin(), end = ships_.end();
	itr != end;
		++itr)
	{
		//remove ship if dead, update if not
		if ((*itr)->isDead())
		{
			//generate a particle explosion at the dead ship's position
			thor::UniversalEmitter emitter;
			emitter.setParticlePosition((*itr)->getPosition());
			emitter.setEmissionRate(100.f);
			emitter.setParticleScale(sf::Vector2f(0.25f, 0.25f));
			emitter.setParticleColor((*itr)->getFillColor());
			emitter.setParticleVelocity([](){ return thor::Distributions::deflect(sf::Vector2f(1.f,1.f), 360.f)() * thor::Distributions::uniform(50.f, 100.f)(); });
			particleSystem_.addEmitter(emitter, sf::seconds(0.1f));

			delete *itr;
			itr = ships_.erase(itr);
		}
		else {
			(*itr)->update();
		}
	}

	//Keyboard updates
	if (keyboard_.isKeyDown(sf::Keyboard::Escape))
	{
		window_.close();
	}

	if (keyboard_.isKeyDown(sf::Keyboard::W) || keyboard_.isKeyDown(sf::Keyboard::Up))
	{
		controlled_->thrust();
	}

	if (keyboard_.isKeyDown(sf::Keyboard::D) || keyboard_.isKeyDown(sf::Keyboard::Right))
	{
		controlled_->turnRight();
	}

	if (keyboard_.isKeyDown(sf::Keyboard::A) || keyboard_.isKeyDown(sf::Keyboard::Left))
	{
		controlled_->turnLeft();
	}

	if (keyboard_.isKeyDown(sf::Keyboard::Space))
	{
		if (controlled_->trigger())
		{
			//Add a bullet here
			bullets_.push_back(new Bullet(controlled_->getPosition(), controlled_->getForward()));
		}
	}
}

void Game::genStars(float minX, float maxX, float minY, float maxY, unsigned int amount, float minR, float maxR)
{
	if (minX <= maxX && minY <= maxY && minR <= maxR)
	{
		stars_.clear();

		sf::Vector2f p;
		float r;
		sf::Color c;

		for (unsigned int i = 0; i < amount; ++i)
		{
			p.x = randFloat(minX, maxX);
			p.y = randFloat(minY, maxY);

			r = randFloat(minR, maxR);

			c.r = randFloat(128, 255);
			c.g = randFloat(128, 255);
			c.b = randFloat(128, 255);

			stars_.push_back(Star(p, r, c));
		}
	}

}

void Game::draw() {
	window_.clear();
	window_.setView(camera_);

	//draw stars
	for (auto itr = stars_.begin(), end = stars_.end();
	itr != end;
		++itr)
	{
		window_.draw(*itr);
	}

	//draw particles
	window_.draw(particleSystem_);

	//drawm bullets
	for (auto itr = bullets_.begin(), end = bullets_.end();
		itr != end;
		++itr)
	{
		window_.draw(**itr);
	}

	//draw ships
	for (auto itr = ships_.begin(), end = ships_.end();
		itr != end;
		++itr)
	{
		window_.draw(**itr);
	}

	window_.display();
}
#pragma endregion
