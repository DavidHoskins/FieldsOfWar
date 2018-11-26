#include "InputHandler.h"

#include <SFML/Window/Event.hpp>
#include <iostream>

#include "HexHandler.h"
#include "UIBox.h"
#include "UIText.h"
#include "NationHandler.h"
#include "Nation.h"

//Main constructor.
InputHandler::InputHandler()
{
	m_clock.restart();
}

//Input for the main menu and country selection menu.
void InputHandler::updateMainMenuInputHandler(HexHandler& mapInfo, sf::RenderWindow& window, UIHandler& UIInfo, sf::View& view)
{
	m_mousePosition = sf::Mouse::getPosition(window);
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		if (mapInfo.mouseOverHex(&window, this) != nullptr)
		{
			if (mapInfo.mouseOverHex(&window, this)->getCountryTag() != ' ')
			{
				for (int i = 0; mapInfo.m_nationsInfo->m_nationsInGame.size(); i++)
				{
					if (mapInfo.m_nationsInfo->m_nationsInGame[i]->ownsHex(mapInfo.mouseOverHex(&window, this)))
					{
						mapInfo.m_nationsInfo->m_currentNation = mapInfo.m_nationsInfo->m_nationsInGame[i];
						return;
					}
				}
			}
		}
	}
}

//Checks which tile type you wish to change the tile to.
void InputHandler::updateInputHandler(int& gameSpeed, HexHandler& mapInfo, sf::RenderWindow& window, UIHandler& UIInfo, sf::View& view)
{
	m_mousePosition = sf::Mouse::getPosition(window);

	if (m_editorMode)
	{
		//editorUpdate();
	}

	//Start game selection will be changed once editor is finished and start loads from file.
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
	{
		if (m_selectNation == false)
		{
			m_selectNation = true;
		}
		else
		{
			m_selectNation = false;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		if (gameSpeed != 0 && !m_pressedKey)
		{
			m_oldGameSpeed = gameSpeed;
			gameSpeed = 0;
			m_pressedKey = true;
		}
		else if (!m_pressedKey)
		{
			gameSpeed = m_oldGameSpeed;
			m_pressedKey = true;
		}
	}
	else
	{
		m_pressedKey = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
	{
		gameSpeed = 1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
	{
		gameSpeed = 2;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
	{
		gameSpeed = 3;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
	{
		gameSpeed = 4;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
	{
		gameSpeed = 5;
	}
	cameraControls(view);
	//Grouping.
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
	{
		groupUnits(mapInfo);
	}
	//Splitting
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		{
			splitUnits(mapInfo);
		}
	}
	bool wasOverUnit = false;
	bool alreadySelectedTop = false;
	if (isMouseDown())
	{
		for (int y = 0; y < mapInfo.m_unitsInGame.size(); y++)
		{
			if (mapInfo.m_unitsInGame[y]->m_nation == mapInfo.m_nationsInfo->m_currentNation)
			{
				if (mapInfo.m_unitsInGame[y]->isMouseOver(m_mousePosition, view, window))
				{
					if (!mapInfo.m_unitsInGame[y]->getIsSelected())
					{
						mapInfo.m_unitsInGame[y]->setIsSelected(true);
						wasOverUnit = true;
					}
					else if (alreadySelectedTop)
					{
						mapInfo.m_unitsInGame[y]->setIsSelected(false);
					}
					else
					{
						alreadySelectedTop = true;
						wasOverUnit = true;
					}
				}
			}
		}
		if (UIInfo.m_HexUI)
		{
			return;
		}

		Hex* temp = nullptr;

		if (wasOverUnit)
		{

		}
		else if ((temp = mapInfo.mouseOverHex(&window, this)) != nullptr)
		{
			for (int y = 0; y < mapInfo.m_unitsInGame.size(); y++)
			{
				if (mapInfo.m_unitsInGame[y]->getIsSelected())
				{
					mapInfo.m_unitsInGame[y]->m_targetHex = temp;
					mapInfo.m_unitsInGame[y]->setIsSelected(false);
				}
			}

			if (mapInfo.m_nationsInfo->m_nationToAddTo != nullptr)
			{
				if (!mapInfo.m_nationsInfo->m_nationToAddTo->ownsHex(temp))
				{
					mapInfo.m_nationsInfo->m_nationToAddTo->m_ownedTiles.push_back(temp);
					std::cout << "Adding province: " << temp->hexID << " to Nation: ";
					std::cout << mapInfo.m_nationsInfo->m_nationToAddTo->getCountryTag() << std::endl;
				}
			}
			else if (mapInfo.m_nationsInfo->m_currentNation != nullptr)
			{
				if (mapInfo.m_nationsInfo->m_currentNation->ownsHex(temp))
				{
					if (mapInfo.m_hightlightedHex != nullptr)
					{
						mapInfo.m_hightlightedHex->setUIActive(false);
					}
					mapInfo.m_hightlightedHex = temp;
					if (!UIInfo.m_DiploUI)
					{
						UIInfo.m_HexUI = true;
					}
				}
				else
				{
					Nation* tempNation = findHexOwner(temp, mapInfo.m_nationsInfo);
					if (tempNation != nullptr && !UIInfo.m_HexUI)
					{
						UIInfo.m_DiploUI = true;
						UIInfo.m_diploNation = tempNation;
					}
				}
			}
		}
	}

	else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		UIInfo.m_HexUI = false;
		UIInfo.m_DiploUI = false;
		UIInfo.m_diploNation = nullptr;
		mapInfo.m_hightlightedHex = nullptr;
		for (int i = 0; i < mapInfo.m_unitsInGame.size(); i++)
		{
			if (mapInfo.m_unitsInGame[i]->getIsSelected())
			{
				mapInfo.m_unitsInGame[i]->setIsSelected(false);
			}
		}

	}
	mapInfo.adjustPosition(*this);

}

//Checks which nation owns hex.
Nation* InputHandler::findHexOwner(Hex* hexToCheck, NationHandler* nations)
{
	Nation* nationToReturn = nullptr;
	for (int i = 0; i < nations->m_nationsInGame.size(); i++)
	{
		if (nations->m_nationsInGame[i]->ownsHex(hexToCheck))
		{
			return nations->m_nationsInGame[i];
		}
	}
	return nationToReturn;
}

//Camera controls for the game.
void InputHandler::cameraControls(sf::View& view)
{
	timeSinceLastFrame = m_clock.getElapsedTime().asSeconds();
	m_clock.restart();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		if (view.getCenter().y + (m_panningSpeed * timeSinceLastFrame) > -10)
		{
			view.setCenter(view.getCenter().x, view.getCenter().y - (m_panningSpeed * timeSinceLastFrame));
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		if (view.getCenter().y + (m_panningSpeed * timeSinceLastFrame) < 8000)
		{
			view.setCenter(view.getCenter().x, view.getCenter().y + (m_panningSpeed * timeSinceLastFrame));
		}

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		if (view.getCenter().x + (m_panningSpeed * timeSinceLastFrame) > -10)
		{
			view.setCenter(view.getCenter().x - (m_panningSpeed * timeSinceLastFrame), view.getCenter().y);
		}

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		if (view.getCenter().x + (m_panningSpeed * timeSinceLastFrame) < 8000)
		{
			view.setCenter(view.getCenter().x + (m_panningSpeed * timeSinceLastFrame), view.getCenter().y);
		}

	}
}

//Grouping units when called into one stack.
void InputHandler::groupUnits(HexHandler& mapInfo)
{
	std::vector<UnitStack*> unitsSelected;
	for (int i = 0; i < mapInfo.m_unitsInGame.size(); i++)
	{
		if (mapInfo.m_unitsInGame[i]->getIsSelected())
		{
			if (mapInfo.m_nationsInfo->m_currentNation == mapInfo.m_unitsInGame[i]->m_nation)
			{
				unitsSelected.push_back(mapInfo.m_unitsInGame[i]);
			}
		}
	}
	if (unitsSelected.size() > 0)
	{
		unitsSelected[0]->setIsSelected(false);
		for (int i = 1; i < unitsSelected.size(); i++)
		{
			unitsSelected[0]->m_landUnits[Hex::infantry] += unitsSelected[i]->m_landUnits[Hex::infantry];
			unitsSelected[i]->m_destroyed = true;
			unitsSelected[i]->setIsSelected(false);
		}
	}
}

void InputHandler::splitUnits(HexHandler& mapInfo)
{
	std::vector<UnitStack*> unitsSelected;
	for (int i = 0; i < mapInfo.m_unitsInGame.size(); i++)
	{
		if (mapInfo.m_unitsInGame[i]->getIsSelected())
		{
			if (mapInfo.m_nationsInfo->m_currentNation == mapInfo.m_unitsInGame[i]->m_nation)
			{
				unitsSelected.push_back(mapInfo.m_unitsInGame[i]);
			}
		}
	}
	for (int i = 0; i < unitsSelected.size(); i++)
	{
		if (unitsSelected[i]->m_landUnits[Hex::infantry] % 1000 == 0)
		{
			mapInfo.m_unitsInGame.push_back(new UnitStack(20.0f, 20, true, static_cast<int>(mapInfo.m_unitsInGame.size()), &mapInfo));
			mapInfo.m_unitsInGame[mapInfo.m_unitsInGame.size() - 1]->m_nation = mapInfo.m_nationsInfo->m_currentNation;
			mapInfo.m_unitsInGame[mapInfo.m_unitsInGame.size() - 1]->m_landUnits[Hex::infantry] = (unitsSelected[i]->m_landUnits[Hex::infantry] / 2);
			unitsSelected[i]->m_landUnits[Hex::infantry] = (unitsSelected[i]->m_landUnits[Hex::infantry] / 2);
			if (unitsSelected[i]->m_landUnits[Hex::infantry] % 1000 != 0)
			{
				unitsSelected[i]->m_landUnits[Hex::infantry] += (unitsSelected[i]->m_landUnits[Hex::infantry] % 1000);
				mapInfo.m_unitsInGame[mapInfo.m_unitsInGame.size() - 1]->m_landUnits[Hex::infantry] -= (unitsSelected[i]->m_landUnits[Hex::infantry] % 1000);
			}
		}
	}
}

//This method allows access to editor elements.
void InputHandler::editorUpdate()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
	{
		m_currentTileType = 0;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
	{
		m_currentTileType = 1;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
	{
		m_currentTileType = 2;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
	{
		m_currentTileType = 3;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
	{
		m_currentTileType = 4;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
	{
		m_currentTileType = 5;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
	{
		m_createNation = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
	{
		m_fillNation = true;
		m_emptyNation = false;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
	{
		m_emptyNation = true;
	}
}

//Checks if the left click is down.
bool InputHandler::isMouseDown()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		if (m_releasedButton)
		{
			m_releasedButton = false;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		m_releasedButton = true;
		return false;
	}
}

//Checks if mouse is down and has been released.
bool InputHandler::isMouseDown(bool &released)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		if (released)
		{
			released = false;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		released = true;
		return false;
	}
}
