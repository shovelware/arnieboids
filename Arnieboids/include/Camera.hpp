#ifndef _CAMERA_H
#define _CAMERA_H

#include <SFML\Graphics\View.hpp>
#include <include\Ship.hpp>

/*!
* Follow camera that can target a ship
* Also draws that ship's hp and radar to screen
*/

class Camera : public sf::View
{
public:
	Camera(sf::Vector2u screenSize);
	Camera(sf::Vector2u screenSize, Ship* target);

	void setTarget(Ship* target);
	Ship* getTarget();
	void clearTarget();

	/*!
	* Updates camera's center to where the target is
	* May also have some nice tweening later
	*/
	void update();

	//! Not implemented!
	void move();

	//! Not implemented!
	void zoomIn();
	void zoomOut();
	void zoomSet();
	void zoomReset();

	//! Not implemented!
	void drawHUD();
private:
	Ship* target_;			//!< The target followed ship
	sf::Vector2f screenSize_; //!< Keep a record of the screen size for resets
};
#endif