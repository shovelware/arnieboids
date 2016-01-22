#ifndef _TILED_BACKDROP_H
#define _TILED_BACKDROP_H

#include <list>
#include <SFML/Graphics.hpp>
#include "Star.hpp"

/*
 * \brief A large backdrop of stars.
 * The stars are generated once and drawn to a texture. That texture is then drawn to avoid many draw calls.
 */
class TiledBackdrop {
public:
	TiledBackdrop(sf::Vector2f size);
	~TiledBackdrop();

	//! Draws the texture to the target.
	void draw(sf::RenderTarget &target) const;
private:
	sf::Sprite sprite_;
	sf::Texture texture_;

	//! Generates a field of stars and returns a RenderTexture pointer (Be sure to delete it when done!).
	sf::RenderTexture* genStars(float minX, float maxX, float minY, float maxY, float minR, float maxR, unsigned int amount = 420u) const;
	
	//! Returns a render texture that is 3 times the size of the one passed.
	sf::Texture expand(sf::RenderTexture const &oldTex) const;
};
#endif