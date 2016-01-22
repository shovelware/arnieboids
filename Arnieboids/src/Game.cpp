#include <include/Game.hpp>

inline float randFloat(float MAX) { return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / MAX)); };
inline float randFloat(float MIN, float MAX) { return MIN + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (MAX - MIN))); };

#pragma region PublicMemberFunctions
Game::Game(sf::RenderWindow &window, unsigned int timePerTick) :
window_(window),
camera_(window_),
ships_(),
bullets_(),
controller_(),
tickClock_(),
timePerTick_(timePerTick),
timeOfLastTick_(tickClock_.now() - timePerTick_),
collisionSystem_(ships_, bullets_, pickups_),
keyboard_(),
particleSystem_(),
particleTexture_(),
gameBounds_(-(window.getSize().x * 1.5f), -(window.getSize().y * 1.5f), window.getSize().x  * 1.5f, window.getSize().y  * 1.5f),
backdrop_(sf::Vector2f(window.getSize()) * 3.f)
{
	srand(time(NULL));
	//float w = winWidth;
	//float h = winHeight;
	//
	//genStars(-w, w * 2, -h, h * 2, 420U, 1, 5);

	//define function for adding bullets to list
	fireBulletCallback_ = [this](Bullet* bullet){ bullets_.push_back(bullet); };
	spawnShipCallback_ = [this](Ship * ship){ ships_.push_back(ship); };
	camera_.loadFont("game_over.ttf");

	//fade particles out at end of life
	particleSystem_.addAffector(thor::AnimationAffector(thor::FadeAnimation(0.f, 0.25f)));

	particleTexture_.loadFromFile("./particlez.png");
	particleSystem_.setTexture(particleTexture_);

	erasedSBuffer_.loadFromFile("./sound/erased.ogg");
	mineSBuffer_.loadFromFile("./sound/you_are_mine.ogg");
	planSBuffer_.loadFromFile("./sound/the_plan.ogg");
	backSBuffer_.loadFromFile("./sound/ill_be_back.ogg");

	erasedSound_.setBuffer(erasedSBuffer_);
	mineSound_.setBuffer(mineSBuffer_);
	planSound_.setBuffer(planSBuffer_);
	backSound_.setBuffer(backSBuffer_);

	planSound_.play();

	bgMusic_.openFromFile("./sound/music.ogg");
	bgMusic_.setLoop(true);
	bgMusic_.play();

	reset();
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

	for (auto itr = pickups_.begin(), end = pickups_.end();
	itr != end;
		++itr)
	{
		delete *itr;
	}
}

int Game::run() {

	while (window_.isOpen()) {

		//If time since last tick is greater or equal to the time per tick...
		if (tickClock_.now() - timeOfLastTick_ >= timePerTick_)
		{
			timeOfLastTick_ = tickClock_.now();

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
void Game::wrap(sf::ConvexShape * cs)
{
	sf::Vector2f curPos = cs->getPosition();
	sf::Vector2f newPos = curPos;

	//X check
	if (curPos.x < gameBounds_.left || curPos.x > gameBounds_.width)
	{
		//Wrap
		newPos.x *= -1;

		//Double check in case we're wrapped outside
		if (newPos.x < gameBounds_.left || newPos.x > gameBounds_.width)
		{
			int sign = (0 < newPos.x) - (newPos.x < 0);
			newPos.x = (gameBounds_.width - 10) * sign;
		}
	}

	//Y check
	if (curPos.y < gameBounds_.top|| curPos.y > gameBounds_.height)
	{
		//Wrap
		newPos.y *= -1;

		//Double check in case we're wrapped outside
		if (newPos.y < gameBounds_.top || newPos.y > gameBounds_.height)
		{
			int sign = (0 < newPos.y) - (newPos.y < 0);
			newPos.y = (gameBounds_.height- 10) * sign;
		}
	}

	if (curPos != newPos)
	{
		cs->setPosition(newPos);
	}

}

void Game::playerDeath()
{
	backSound_.play();
	camera_.clearTarget(true);
	controlled_ = nullptr;
	SwarmBoid::setSwarmTarget(nullptr);
	Predator::setPrey(nullptr);
	Mothership::setTarget(nullptr);
}

void Game::clearLists()
{
	for (auto itr = bullets_.begin(), end = bullets_.end();
	itr != end;
		++itr)
	{
		delete *itr;
	}

	bullets_.clear();

	for (auto itr = ships_.begin(), end = ships_.end();
	itr != end;
		++itr)
	{
		delete *itr;
	}

	ships_.clear();


	for (auto itr = pickups_.begin(), end = pickups_.end();
	itr != end;
		++itr)
	{
		delete *itr;
	}

	pickups_.clear();
}

void Game::reset()
{
	//Clean up previous game
	clearLists();
	particleSystem_.clearParticles();

	//Add a player
	ships_.push_back(new Player(particleSystem_, sf::Vector2f(200.f, 200.f)));
	Ship* player = *ships_.rbegin();

	//Target camera, controls, ai to player
	camera_.clearTarget(false);
	camera_.moveReset();
	camera_.setTarget(player);
	controlled_ = player;
	SwarmBoid::setSwarmTarget(player);
	Predator::setPrey(player);
	Mothership::setTarget(player);

	//! Game setup variables
	const int minSwarms = 1;
	const int maxSwarms = 4;
	const int minBoids = 5;
	const int maxBoids = 25;
	const int minSwarmRad = 100;
	const int maxSwarmRad = 250;

	const int minMothers = 2;
	const int maxMothers = 4;

	const int minPickups = 4;
	const int maxPickups = 8;

	const int minRoids = 4;
	const int maxRoids = 20;

	//Add several swarms (at least min, up to max)
	int swarms = randFloat(maxSwarms - minSwarms) + minSwarms;
	
	sf::Vector2f swarmPos(0, 0);
	for (int i = 0; i < swarms; ++i)
	{
		swarmPos = sf::Vector2f(randFloat(gameBounds_.left + 100, gameBounds_.width - 100), randFloat(gameBounds_.top + 100, gameBounds_.height - 100));

		float thisSwarmNum = randFloat(0, maxBoids - minBoids) + minBoids;
		float thisSwarmRad = randFloat(0, maxSwarmRad - minSwarmRad) + minSwarmRad;
		addBoidSwarm(swarmPos, thisSwarmNum, thisSwarmRad);
	}

	//Add some motherships
	int motherShips = randFloat(maxMothers-minMothers) + minMothers;

	sf::Vector2f motherPos(0, 0);

	for (int i = 0; i < motherShips; ++i)
	{
		motherPos = sf::Vector2f(randFloat(gameBounds_.left, gameBounds_.width), randFloat(gameBounds_.top, gameBounds_.height));

		ships_.push_back(new Mothership(particleSystem_, fireBulletCallback_, spawnShipCallback_, motherPos));
		(*--ships_.end())->setRotation(randFloat(0, 360));
	}

	//Add a few pickups
	int pickups = randFloat(maxPickups - minPickups) + minPickups;

	sf::Vector2f pickupPos(0, 0);

	for (int i = 0; i < pickups; ++i)
	{
		pickupPos = sf::Vector2f(randFloat(gameBounds_.left, gameBounds_.width), randFloat(gameBounds_.top, gameBounds_.height));

		pickups_.push_back(new Pickup(pickupPos, 5));
	}

	////Add some asteroids
	int roids = randFloat(maxRoids - minRoids) + minRoids;

	sf::Vector2f roidPos(0, 0);
	sf::Vector2f roidDir(0, -1);
	float roidSpinSpd = 0.25f;

	for (int i = 0; i < roids; ++i)
	{
		roidPos = sf::Vector2f(randFloat(gameBounds_.left, gameBounds_.width), randFloat(gameBounds_.top, gameBounds_.height));
		roidDir = thor::unitVector(sf::Vector2f(randFloat(-1, 1), randFloat(-1, 1)));
		float thisRoidSpinSpd = randFloat(0.25f, 1.f) + roidSpinSpd;

		ships_.push_back(new Asteroid(particleSystem_, roidPos, roidDir, thisRoidSpinSpd));
	}
}

void Game::addBoidSwarm(sf::Vector2f const & pos, float amount, float radius)
{
	sf::Vector2f boidPos(0, 0);
	float boidDist = 0;

	for (int i = 0; i <= amount; ++i)
	{
		boidDist = randFloat(radius);

		boidPos.x = boidDist * (cos((thor::Pi / 180) * randFloat(360)));
		boidPos.y = boidDist * (sin((thor::Pi / 180) * randFloat(360)));

		ships_.push_back(new SwarmBoid(particleSystem_, pos + boidPos));
	}
}

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

	 //Keyboard

	 //Escape : Close Window
	if (keyboard_.isKeyDown(sf::Keyboard::Escape))
	{
		window_.close();
	}

	//F5 : Reset game
	if (keyboard_.isKeyPressed(sf::Keyboard::F5))
	{
		reset();
	}

	//If we have a ship to control
	if (controlled_ != nullptr)
	{
		//Up, W : Thrust
		if (keyboard_.isKeyDown(sf::Keyboard::W) || keyboard_.isKeyDown(sf::Keyboard::Up))
		{
			controlled_->thrust();
		}
		//Right, D : Turn CW
		if (keyboard_.isKeyDown(sf::Keyboard::D) || keyboard_.isKeyDown(sf::Keyboard::Right))
		{
			controlled_->turnRight();
		}
		//Left, A : Turn CCW
		if (keyboard_.isKeyDown(sf::Keyboard::A) || keyboard_.isKeyDown(sf::Keyboard::Left))
		{
			controlled_->turnLeft();
		}
		//Down, S : Brake
		if (keyboard_.isKeyDown(sf::Keyboard::S) || keyboard_.isKeyDown(sf::Keyboard::Down))
		{
			controlled_->brake();
		}

		//Space : Fire
		if (keyboard_.isKeyDown(sf::Keyboard::Space))
		{
			if (controlled_->trigger())
			{
				//Add a bullet here
				Bullet * newBullet = new Bullet(controlled_->getPosition() + controlled_->getForward() * 8.f, controlled_->getForward(), 8.f + thor::length(controlled_->getVelocity()));
				newBullet->setFaction(Bullet::PLAYER);
				fireBulletCallback_(newBullet);
			}
		}

		//V : Damage self
		if (keyboard_.isKeyPressed(sf::Keyboard::V))
		{
			controlled_->takeDamage(1);
		}
	}

	//I : Zoom in
	if (keyboard_.isKeyDown(sf::Keyboard::I))
	{
		camera_.zoomIn();
	}
	//O : Zoom Out
	if (keyboard_.isKeyDown(sf::Keyboard::O))
	{
		camera_.zoomOut();
	}
	//P : Reset Zoom
	if (keyboard_.isKeyPressed(sf::Keyboard::P))
	{
		camera_.zoomReset();
	}

	float camSpeed = 5;
	sf::Vector2f camMove(0, 0);

	//U : Move camera up
	if (keyboard_.isKeyDown(sf::Keyboard::U))
	{
		camMove.y -= 1;
	}
	//H : Move camera left
	if (keyboard_.isKeyDown(sf::Keyboard::H))
	{
		camMove.x -= 1;
	}
	//J : Move camera down
	if (keyboard_.isKeyDown(sf::Keyboard::J))
	{
		camMove.y += 1;
	}
	//K : Move camera right
	if (keyboard_.isKeyDown(sf::Keyboard::K))
	{
		camMove.x += 1;
	}
	//Actual camera move
	if (camMove != sf::Vector2f(0, 0))
	{
		camMove = thor::unitVector(camMove);
		camMove *= camSpeed;
		camera_.move(camMove);
	}
	//L : Reset camera move
	if (keyboard_.isKeyPressed(sf::Keyboard::L))
	{
		if (camera_.getTarget() != nullptr)
		{
			camera_.moveReset();
		}
	}
	//M : Toggle camera follow
	if (keyboard_.isKeyPressed(sf::Keyboard::M))
	{
		if (camera_.getTarget() == nullptr)
		{
			if (controlled_ != nullptr)
			{
				camera_.setTarget(controlled_);
				camera_.moveReset();
			}
		}

		else camera_.clearTarget(true);
	}
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
			wrap(*itr);
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
	itr != end; /*No increment*/)
	{
		//remove ship if dead, update if not
		if ((*itr)->isDead())
		{
			//generate a particle explosion at the dead ship's position
			thor::UniversalEmitter emitter;
			emitter.setParticlePosition((*itr)->getPosition());
			emitter.setParticleScale(sf::Vector2f(0.25f, 0.25f));
			emitter.setParticleColor((*itr)->getFillColor());
			emitter.setParticleVelocity([](){ return thor::Distributions::deflect(sf::Vector2f(1.f,1.f), 360.f)() * thor::Distributions::uniform(50.f, 100.f)(); });
			
			//make a more fancy explosion for player ship
			if (*itr == controlled_) {
				emitter.setEmissionRate(1000.f);
				emitter.setParticleLifetime(thor::Distributions::uniform(sf::seconds(0.5f), sf::seconds(1.f)));
				particleSystem_.addEmitter(emitter, sf::seconds(1.f));
			}
			else {
				emitter.setEmissionRate(100.f);
				particleSystem_.addEmitter(emitter, sf::seconds(0.1f));
			}


			//If the player dies
			if (*itr == controlled_)
			{
				playerDeath();
			}
			else if (dynamic_cast<Mothership*>(*itr))
			{
				erasedSound_.play();
			}

			delete *itr;
			itr = ships_.erase(itr++);
		}
		else {
			wrap(*itr);
			(*itr++)->update();
		}
	}

	for (auto itr = pickups_.begin(), end = pickups_.end();
	itr != end; /*No increment*/)
	{
		//Remove pickup if inactive
		if (!(*itr)->isActive())
		{
			delete *itr;
			itr = pickups_.erase(itr++);
		}

		else {
			if ((*itr)->wasPickedUpLastTick())
			{
				mineSound_.play();
			}
			wrap(*itr);
			(*itr++)->update();
		}
	}
}

void Game::draw() {
	window_.clear();

	window_.setView(camera_);

	backdrop_.draw(window_);

	//draw particles
	window_.draw(particleSystem_);

	//draw bullets
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
		window_.draw((*itr)->debug_rect_);
	}

	//draw pickups
	for (auto itr = pickups_.begin(), end = pickups_.end();
	itr != end;
		++itr)
	{
		window_.draw(**itr);
	}

	camera_.drawHUD();
	camera_.drawRadar(ships_, sf::Vector2f(0, 0), 100);

	//Draw game bounds
	sf::RectangleShape boundsR(sf::Vector2f(gameBounds_.width * 2, gameBounds_.height * 2));
	boundsR.setOrigin(gameBounds_.width, gameBounds_.height);
	boundsR.setPosition(sf::Vector2f(0, 0));
	boundsR.setFillColor(sf::Color(255, 000, 000, 000));
	boundsR.setOutlineColor(sf::Color(255, 128, 064, 128));
	boundsR.setOutlineThickness(-1);
	window_.draw(boundsR);

	window_.display();
}
#pragma endregion

