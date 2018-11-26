#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/System/Vector2.hpp>

#include "UIHandler.h"

class HexHandler;
class Hex;

class InputHandler
{
public:
	InputHandler();
	bool isMouseDown();
	bool isMouseDown(bool &released);
	void updateInputHandler(int& gameSpeed, HexHandler& mapInfo, sf::RenderWindow& window, UIHandler& UIInfo, sf::View& view);

	void updateMainMenuInputHandler(HexHandler& mapInfo, sf::RenderWindow& window, UIHandler& UIInfo, sf::View& view);
	
	bool m_createNation;
	bool m_fillNation;
	bool m_emptyNation;
	bool m_selectNation;

	bool m_releasedButton = true;

	bool m_editorMode = true;

	int m_currentTileType;

	sf::Vector2f m_ofsetPosition = sf::Vector2f(0.0f,0.0f);
	sf::View* m_view;

	sf::Vector2i m_mousePosition;
private:
	void editorUpdate();
	void groupUnits(HexHandler& mapInfo);
	void splitUnits(HexHandler& mapInfo);

	Nation* findHexOwner(Hex* hexToCheck, NationHandler* nations);

	void cameraControls(sf::View& view);

	float m_panningSpeed = 1000.0f;
	float timeSinceLastFrame;

	sf::Clock m_clock;


	int m_oldGameSpeed;
	bool m_pressedKey;

};

#endif //!INPUTHANDLER_H

