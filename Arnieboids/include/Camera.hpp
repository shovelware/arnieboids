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
	Camera(sf::RenderTarget& target);

	void setTarget(Ship* target);
	Ship* getTarget();
	
	//! Clears the camera's target, can save camera transform
	void clearTarget(bool savexf);
	void loadFont(std::string filename);

	/*!
	* Updates camera's center to where the target is
	* May also have some nice tweening later
	*/
	void update();

	//! Moves controls for the camera
	void move(sf::Vector2f xf);
	void moveReset();

	//! Zoom controls for camera
	void zoomIn();
	void zoomOut();
	void zoomSet(float mult);
	void zoomReset();

	//! HUD Drawing controls, both of these change render target for their duration
	void drawHUD();
	void drawRadar(std::list<Ship*>& objList, sf::Vector2f centre, float radius);

	float getZoomPercent() const;
private:
	Ship* target_;			//!< The target followed ship
	sf::Vector2f screenSize_; //!< Keep a record of the screen size for resets

	float zoomFactor_; //!< The internal factor of the zoom
	float zoomStep_; //!< Zoom step [0.25 == 25%]

	sf::Vector2f move_; //!< Vector to move by

	sf::Font* font_; //!< Font to draw HUD with
	sf::Text text_; //!< HUD is drawn with a single text
	sf::RenderTarget& rentrg_; //!< HUD is drawn to this target

	int radarSize_; //!< Radar size within HUD
	sf::Vector2f radarPos_; //!< Radar position within HUD

	void drawText(std::string info, sf::Vector2f pos, sf::Color color);
	void drawRadarObj(sf::ConvexShape* shape); //!< 
	sf::Vector2f worldToRadar(sf::Vector2f const& worldPos);
};
#endif