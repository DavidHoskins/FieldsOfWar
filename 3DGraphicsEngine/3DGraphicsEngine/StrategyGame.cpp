#include "StrategyGame.h"

#include "UIHandler.h"
#include "Nation.h"

//Constructor
StrategyGame::StrategyGame()
	:m_currentGameState(mainMenu)
	,m_window(new sf::RenderWindow(sf::VideoMode(1920, 1080), "StrategyGame", sf::Style::Fullscreen))
	,m_timeToTarget(0)
{
	m_UI = new UIHandler();
	m_Hexagon = new HexHandler(m_UI);
	m_nations = new NationHandler();
	m_input = new InputHandler();
	m_input->m_view = &m_view;
	m_clock = new sf::Clock();
	m_Hexagon->m_nationsInfo = m_nations;
	m_Hexagon->m_UI = m_UI;
	m_mapLoader.loadMapFromFile(m_Hexagon, m_nations, m_mapLoader.saveMapFileNames[0]);
	for (int i = 0; i < m_nations->m_nationsInGame.size(); i++)
	{
		for (int j = 0; j < m_nations->m_nationsInGame[i]->m_ownedProvincesID.size(); j++)
		{
			m_nations->m_nationsInGame[i]->m_ownedTiles.push_back(m_Hexagon->findHexByID(m_nations->m_nationsInGame[i]->m_ownedProvincesID[j]));
		}
	}
	for (int i = 0; i < m_nations->m_nationsInGame.size(); i++)
	{
		m_nations->m_nationsInGame[i]->m_NationText = m_UI->createUITextElement(sf::Vector2f(100.0f, 12.5f), 18, sf::Color::Black);
	}
	m_view.setSize((6000 * 0.93f), (6000 * 0.52f));
	m_view.setCenter(1920, 1080);
	m_defaultView = m_window->getDefaultView();
	m_defaultView.setSize(m_defaultView.getSize().x * 0.93f, m_defaultView.getSize().y * 0.52f);
	m_window->setView(m_view);
	update();
}

//Destructor.
StrategyGame::~StrategyGame()
{
	delete m_window;
	m_window = nullptr;
	delete m_Hexagon;
	m_Hexagon = nullptr;
	delete m_nations;
	m_nations = nullptr;
	delete m_input;
	m_input = nullptr;
	delete m_clock;
	m_clock = nullptr;
}

//games main update
void StrategyGame::update()
{
	m_clock->restart();
	m_timeSinceLastUpdate = m_clock->getElapsedTime();
	m_timeSinceLastRender = m_clock->getElapsedTime();
	while (m_window->isOpen())
	{
		switch (m_currentGameState)
		{
		case start:
			break;
		case mainMenu:
			menuUpdate();
			menuRender();
			break;
		case main:
			mainUpdate();
			mainRender();
			break;
		case combatMain:
			combatUpdate();
			combatRender();
			break;
		case end:
			break;
		}
	}
	//mapSaver.saveGameToFile(*Hexagon, *nations, "saveFiles/mapfile.gmf");
}

//Main update for main menu.
void StrategyGame::menuUpdate()
{
	sf::Event event;
	while (m_window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_window->close();
			return;
		}
	}
	sf::View temp = m_window->getView();
	m_input->updateMainMenuInputHandler(*m_Hexagon, *m_window, *m_UI, temp);
	inputUpdates();
	m_input->updateInputHandler(m_gameSpeed, *m_Hexagon, *m_window, *m_UI, m_view);
	if (m_nations->m_currentNation != nullptr)
	{
		m_currentGameState = main;
		m_view.setCenter(m_nations->m_currentNation->m_ownedTiles[0]->getPosition());
		m_scrollTarget = sf::Vector2f(2000.0f * 0.93f, 2000.0f * 0.52f);
		m_smoothingAmount = sf::Vector2f(((m_scrollTarget.x - m_view.getSize().x) / k_startBufferAmount), ((m_scrollTarget.y - m_view.getSize().y) / k_startBufferAmount));
		m_timeToTarget = k_startBufferAmount;
	}
	for (int i = 0; i < m_Hexagon->getMapWidth(); i++)
	{
		for (int j = 0; j < m_Hexagon->getMapHeight(); j++)
		{
			m_Hexagon->getHexAtPosition(i,j)->update(*m_input, m_Hexagon);
		}
	}
	for (int i = 0; i < m_nations->m_nationsInGame.size(); i++)
	{
		for (int j = 0; j < m_nations->m_nationsInGame[i]->m_ownedTiles.size(); j++)
		{
			m_nations->m_nationsInGame[i]->m_ownedTiles[j]->ChangeColour(*m_nations->m_nationsInGame[i]->m_nationColor);
		}
	}
}

//Main render for main menu.
void StrategyGame::menuRender()
{
	m_window->clear();
	m_window->setView(m_view);
	for (int i = 0; i < m_Hexagon->getMapHeight(); i++)
	{
		for (int j = 0; j < m_Hexagon->getMapWidth(); j++)
		{
			m_Hexagon->getHexAtPosition(j, i)->render(*m_window);
		}
	}
	m_window->display();
}

//Update for the main game-play.
int StrategyGame::mainUpdate()
{
	inputUpdates();
	m_input->updateInputHandler(m_gameSpeed, *m_Hexagon, *m_window, *m_UI, m_view);
	m_UI->update(m_input, m_Hexagon, m_nations);
	if (m_combatSystem.getCombatMode())
	{
		m_currentGameState = combatMain;
	}
	sf::Event event;
	while (m_window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_window->close();
			return -1;
		}
		if (event.type == sf::Event::MouseWheelMoved)
		{
			if (m_scrollTarget.x - event.mouseWheel.delta * k_scrollSpeed < 8000 && m_scrollTarget.x - event.mouseWheel.delta * k_scrollSpeed > 2000)
			{
				m_scrollTarget = sf::Vector2f((m_scrollTarget.x - event.mouseWheel.delta * k_scrollSpeed), (m_scrollTarget.y - event.mouseWheel.delta * k_scrollSpeed));
				m_smoothingAmount = sf::Vector2f(((m_scrollTarget.x - m_view.getSize().x) / k_bufferAmount), ((m_scrollTarget.y - m_view.getSize().y) / k_bufferAmount));
				m_timeToTarget = k_bufferAmount;
			}
		}
	}
	if (checkTimeUpdate() == 1)
	{

		m_Hexagon->Update(m_window, *m_input);
		m_nations->update();
		m_combatSystem.update(m_nations, m_UI);
		return 0;
	}
	else
	{
		return 0;
	}
	
}

//inputUpdates, this will be fixed to the inputUpdate thread.
void StrategyGame::inputUpdates()
{
	if (m_input->m_createNation)
	{
		m_nations->createNewNation();
		m_nations->m_nationsInGame[(m_nations->m_nationsInGame.size() - 1)]->m_NationText = m_UI->createUITextElement(sf::Vector2f(100, 30), 14, sf::Color::White);
		for (int i = 0; i < m_nations->m_nationsInGame.size(); i++)
		{
			m_nations->m_nationsInGame[i]->m_relations.m_diploInteractionsState.push_back(new DiploInteractions());
		}
		m_input->m_createNation = false;
	}
	if (m_input->m_selectNation)
	{
		m_nations->selectNation();
		m_input->m_selectNation = false;
	}
	if (m_input->m_fillNation)
	{
		m_nations->fillNationByTag();
		m_input->m_fillNation = false;
	}
	else if (m_input->m_emptyNation)
	{
		m_nations->m_nationToAddTo = nullptr;
	}
}

//Render for the main game-play.
void StrategyGame::mainRender()
{
	if (m_clock->getElapsedTime().asSeconds() > (m_timeSinceLastRender.asSeconds() + (1.0f / k_fps)))
	{
		float timeSinceLastFrame = m_clock->getElapsedTime().asSeconds() - m_timeSinceLastRender.asSeconds();
		m_timeSinceLastRender = m_clock->getElapsedTime();
		FPSCounter::setFPSCounter(1.0f / timeSinceLastFrame);

		if (m_Hexagon->getUpdateScreen())
		{
			m_window->clear();
			for (int i = 0; i < m_Hexagon->getMapHeight(); i++)
			{
				for (int j = 0; j < m_Hexagon->getMapWidth(); j++)
				{
					m_Hexagon->getHexAtPosition(j, i)->render(*m_window);
				}
			}
			for (int i = 0; i < m_Hexagon->m_unitsInGame.size(); i++)
			{
				m_Hexagon->m_unitsInGame[i]->render(*m_window);
			}
			m_Hexagon->setUpdateScreen(true);
			m_window->setView(m_defaultView);
			mainUIRender();
			m_window->setView(m_view);
			m_window->display();
		}
		if (m_timeToTarget > 0)
		{
			m_view.setSize((m_view.getSize().x + m_smoothingAmount.x), (m_view.getSize().y + m_smoothingAmount.y));
			m_window->setView(m_view);
			m_timeToTarget--;
		}
	}
}

//Render for UI as this needs to be called last to render on top of the in game elements.
void StrategyGame::mainUIRender()
{
	m_UI->render(m_window);
	m_window->draw(m_Hexagon->m_dateText);
	if (m_nations->m_currentNation != nullptr)
	{
		m_window->draw(*m_nations->m_currentNation->m_NationText);
	}
}

//combat mode.
void StrategyGame::combatUpdate()
{
	if (checkTimeUpdate() == 1)
	{
		m_combatSystem.update(m_nations, m_UI);
		if (!m_combatSystem.getCombatMode())
		{
			m_currentGameState = main;
		}
	}
	m_UI->update(m_input, m_Hexagon, m_nations);
}

//Combat mode rendering.
void StrategyGame::combatRender()
{
	m_window->clear();
	mainRender();
	m_window->setView(m_defaultView);
	mainUIRender();
	m_window->display();

}

//Checks if the time expected has passed and if it has the game will update.
int StrategyGame::checkTimeUpdate()
{
	sf::Time temp = m_clock->getElapsedTime();
	if (m_gameSpeed == 0)
	{
		m_timeSinceLastUpdate = m_clock->getElapsedTime();
		return 0;
	}
	else if (m_gameSpeed == 1)
	{
		if ((m_timeSinceLastUpdate.asSeconds() + 1.0f) > temp.asSeconds())
		{
			return 0;
		}
		else
		{
			m_timeSinceLastUpdate = temp;
			return 1;
		}
	}
	else if (m_gameSpeed == 2)
	{
		if ((m_timeSinceLastUpdate.asSeconds() + 0.5f) > temp.asSeconds())
		{
			return 0;
		}
		else
		{
			m_timeSinceLastUpdate = temp;
			return 1;
		}
	}
	else if (m_gameSpeed == 3)
	{
		if ((m_timeSinceLastUpdate.asSeconds() + 0.25f) > temp.asSeconds())
		{
			return 0;
		}
		else
		{
			m_timeSinceLastUpdate = temp;
			return 1;
		}
	}
	else if (m_gameSpeed == 4)
	{
		if ((m_timeSinceLastUpdate.asSeconds() + 0.175f) > temp.asSeconds())
		{
			return 0;
		}
		else
		{
			m_timeSinceLastUpdate = temp;
			return 1;
		}
	}
	return 1;
}
