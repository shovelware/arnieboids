#ifndef _CAMERA_H
#define _CAMERA_H

#include <SFML\Graphics\View.hpp>
#include <include\Ship.hpp>

/*!
* \brief Camera class.
* Camera that can follow target a ship.
* Zoom and Independent movement also possible.
* Handles drawing of HUD, only if a ship is targeted.
* \remarks **I,J,K,L** moves the camera. **I** and **O** zoom in and out respectively.
*/

class Camera : public sf::View
{
public:
	Camera(sf::RenderTarget& target);

	/*
	 * \brief Sets the follow target.
	 * The camera will follow this ship until the camera is unlocked or the ship is destroyed.
	 */
	void setTarget(Ship* target);
	Ship* getTarget();
	
	//! Clears the camera's target, can keep camera on target's last position or reset
	void clearTarget(bool savexf);

	//! Loads a font, required for drawing HUD
	void loadFont(std::string filename);

	//! Updates camera's center to where the target is
	void update();

	//! Move controls for the camera
	void move(sf::Vector2f xf);	void moveReset();

	//! Zoom controls for camera
	void zoomIn();
	void zoomOut();
	void zoomSet(float mult);
	void zoomReset();

	//! HUD Drawing controls, both of these change render target for their duration
	void drawHUD();
	void drawRadar(std::list<Ship*>& objList, sf::Vector2f centre, float radius);

	float getZoomPercent() const; //!< Returns zoom factor as a percentage
private:
	Ship* target_;			//!< The target followed ship
	sf::Vector2f screenSize_; //!< Keep a record of the screen size for resets

	float zoomFactor_; //!< The internal factor of the zoom
	float zoomStep_; //!< Zoom step [0.25 == 25%]

	sf::Vector2f move_; //!< Translation Vector of camera

	sf::Font* font_; //!< Font to draw HUD with
	sf::Text text_; //!< HUD is drawn with a single text
	sf::RenderTarget& rentrg_; //!< HUD is drawn to this target

	int radarSize_; //!< Radar size within HUD
	sf::Vector2f radarPos_; //!< Radar position within HUD

	void drawText(std::string info, sf::Vector2f pos, sf::Color color); //! Draws a color text at pos to HUD, called by drawHUD()
	void drawRadarObj(sf::ConvexShape* shape); //!< Draws a single radar object to radar, called by drawRadar()

	sf::Vector2f worldToRadar(sf::Vector2f const& worldPos); //!< Converts a vector from world space down through range and into radar on-screen space
	sf::Vector2f translateVector(sf::Vector2f pos, sf::FloatRect src, sf::FloatRect dest); //!< Translates pos in coordinate space src to equivalent in coord space dest
};
#endif