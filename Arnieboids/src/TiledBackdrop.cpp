#include <include/TiledBackdrop.hpp>

inline float randFloat(float MAX) { return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / MAX)); };
inline float randFloat(float MIN, float MAX) { return MIN + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (MAX - MIN))); };

TiledBackdrop::TiledBackdrop(sf::Vector2f size) {

	//generate a star field and draw it to the rendertexture
	size *= 0.5f;
	sf::RenderTexture* renderTex = genStars(-size.x, size.x, -size.y, size.y, 0.5f, 2.5f);

	//expand the star field to a 3x3 grid
	texture_ = expand(*renderTex);
	texture_.setSmooth(true);
	sprite_ = sf::Sprite(texture_);

	//center the star field at 0,0
	sprite_.setPosition(-size * 2.5f);

	//delete the now unused render texture
	delete renderTex;
}

TiledBackdrop::~TiledBackdrop() {
}

void TiledBackdrop::draw(sf::RenderTarget& target) const {
	target.draw(sprite_);
}

sf::RenderTexture* TiledBackdrop::genStars(float minX, float maxX, float minY, float maxY, float minR, float maxR, unsigned int amount) const {
	_ASSERT(minX <= maxX);
	_ASSERT(minY <= maxY);
	_ASSERT(minR <= maxR);

	//used to render all stars at once, without many draw calls
	sf::RenderTexture* renderTex = new sf::RenderTexture();
	renderTex->create(maxX - minX, maxY - minY);

	sf::Vector2f p;
	float r;
	sf::Color c;

	//generate stars
	for (unsigned int i = 0; i < amount; ++i)
	{
		p.x = randFloat(0, maxX - minX);
		p.y = randFloat(0, maxY - minY);

		r = randFloat(minR, maxR);

		c.r = randFloat(64, 128);
		c.g = randFloat(128, 255);
		c.b = randFloat(64, 128);

		//add the new star to the render texture
		renderTex->draw(Star(p, r, c));
	}

	renderTex->display();

	return renderTex;
}

sf::Texture TiledBackdrop::expand(sf::RenderTexture const &oldTex) const {
	sf::Vector2i originalSize = sf::Vector2i(oldTex.getSize());

	sf::RenderTexture newRendTex;
	newRendTex.create(originalSize.x * 3, originalSize.y * 3);

	for (int i = 0; i < 9; ++i)
	{
		sf::Vector2f newPos;
		newPos.x += originalSize.x * (i % 3);
		newPos.y += originalSize.y * (i / 3);	//integer division
		
		//create temporary sprite to allow us to draw old renderTexture to new one at specific place
		sf::Sprite temp = sf::Sprite(oldTex.getTexture());
		temp.setPosition(newPos);

		//draw the old texture onto the new
		newRendTex.draw(temp);
	}

	//update the contents of the target texture
	newRendTex.display();

	return newRendTex.getTexture();
}
