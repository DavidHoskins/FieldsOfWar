#ifndef STRATEGYGAME_H
#define STRATEGYGAME_H

#include <thread>

#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>

#include "HexHandler.h"
#include "InputHandler.h"
#include "mapSaveHandler.h"
#include "mapLoadHandler.h"
#include "NationHandler.h"
#include "CombatHelper.h"

class UIHandler;

class StrategyGame
{
private:
	sf::Vector2f m_scrollTarget;
	sf::Vector2f m_smoothingAmount;
	int m_timeToTarget;
	const int k_bufferAmount = 6;
	const int k_startBufferAmount = 60;

	sf::Time m_timeSinceLastRender;
	sf::Time m_timeSinceLastUpdate;
	sf::Clock* m_clock;

	int m_gameSpeed = 1;
	const int k_fps = 60;

	void update();

	int mainUpdate();
	void mainRender();
	void mainUIRender();
	void combatUpdate();
	void combatRender();

	void menuUpdate();
	void menuRender();

	InputHandler* m_input;
	mapSaveHandler m_mapSaver;

	float k_scrollSpeed = 100.0f;

	enum gameState
	{
		start,
		mainMenu,
		main,
		combatMain,
		end
	};

	gameState m_currentGameState;

	sf::RenderWindow* m_window;

	HexHandler* m_Hexagon;
	NationHandler* m_nations;
	UIHandler* m_UI;
	sf::View m_view;
	sf::View m_defaultView;


	int checkTimeUpdate();

	CombatHelper m_combatSystem;

	mapLoadHandler m_mapLoader;

public:
	StrategyGame();
	~StrategyGame();
	void inputUpdates();
};

#endif //!STRATEGYGAME_H

