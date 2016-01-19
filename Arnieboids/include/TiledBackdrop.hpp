#ifndef _TILED_BACKDROP_H
#define _TILED_BACKDROP_H

#include <list>
#include <SFML/Graphics.hpp>
#include "Star.hpp"

class TiledBackdrop {
public:
	TiledBackdrop(sf::Vector2f size);
	~TiledBackdrop();

	void draw(sf::RenderTarget &target) const;
private:
	sf::Sprite sprite_;
	sf::Texture texture_;

	sf::RenderTexture* genStars(float minX, float maxX, float minY, float maxY, float minR, float maxR, unsigned int amount = 420u) const;
	
	//! Returns a render texture that is 3 times the size of the one passed.
	sf::Texture expand(sf::RenderTexture const &oldTex) const;
};
#endif