#include "HexHandler.h"
#include <iostream>

#include "NationHandler.h"
#include "Nation.h"
#include "UIHandler.h"
#include "UnitStack.h"

//Constructor.
HexHandler::HexHandler(UIHandler* UIInfo)
	:m_updateScreen(true)
{
	hexImageLoading();

	m_mapTiles = new Hex*[(m_hexMapWidth*m_hexMapHeight)];
	for (int i = 0; i < m_hexMapWidth; i++)
	{
		for (int j = 0; j < m_hexMapHeight; j++)
		{
			m_mapTiles[i + (j * m_hexMapWidth)] = new Hex(40, UIInfo, m_HexImageTextures);
			m_mapTiles[i + (j * m_hexMapWidth)]->hexID = (i + (j* m_hexMapWidth));
		}
	}

	resetPosition();
	gameFontLoading();
	UIInfo->m_mapInfo = this;
}

//Loads all the hex images into their specific location in the texture array, this makes for more consistent callback of the texture data later on.
void HexHandler::hexImageLoading()
{
	m_HexImageTextures[Hex::grass] = new sf::Texture();
	m_HexImageTextures[Hex::grass]->loadFromFile("Images/Grass.png");

	m_HexImageTextures[Hex::field] = new sf::Texture();
	m_HexImageTextures[Hex::field]->loadFromFile("Images/Field.png");

	m_HexImageTextures[Hex::hill] = new sf::Texture();
	m_HexImageTextures[Hex::hill]->loadFromFile("Images/Hill.png");

	m_HexImageTextures[Hex::city] = new sf::Texture();
	m_HexImageTextures[Hex::city]->loadFromFile("Images/Mountain.png");

	m_HexImageTextures[Hex::inlandSea] = new sf::Texture();
	m_HexImageTextures[Hex::inlandSea]->loadFromFile("Images/InlandSea.png");

	m_HexImageTextures[Hex::openSea] = new sf::Texture();
	m_HexImageTextures[Hex::openSea]->loadFromFile("Images/OpenSea.png");
}

//Loads and sets up the data for the fonts to be used throughout the game.
void HexHandler::gameFontLoading()
{
	m_gameFont = new sf::Font();
	m_gameFont->loadFromFile("saveFiles/sansation.ttf");
	m_dateText.setFont(*m_gameFont);
	m_dateText.setCharacterSize(18);
	m_dateText.setStyle(sf::Text::Regular);
	m_dateText.setFillColor(sf::Color::Black);
	m_dateText.setPosition(sf::Vector2f(700.0f, 12.5f));
	m_dateText.setString("Date:" + std::to_string(m_date));
}

//Destructor.
HexHandler::~HexHandler()
{
	m_targetPointOne = nullptr;
	delete m_targetPointOne;
	m_targetPointTwo = nullptr;
	delete m_targetPointTwo;

	//Clears all units from the game.
	for (int y = 0; y < m_unitsInGame.size(); y++)
	{
		for (int i = 0; i < m_hexMapWidth; i++)
		{
			for (int j = 0; j < m_hexMapHeight; j++)
			{
				m_unitsInGame[y]->m_hexArray = nullptr;
			}
		}
		delete m_unitsInGame[y];
	}

	//Clears all the hexes from the game.
	for (int i = 0; i < m_hexMapWidth; i++)
	{
		for (int j = 0; j < m_hexMapHeight; j++)
		{
			delete m_mapTiles[i + (j * m_hexMapWidth)];
			m_mapTiles[i + (j * m_hexMapWidth)] = nullptr;
		}
	}
	delete[] m_mapTiles;
	m_mapTiles = nullptr;

	//Deletes all the Hex textures.
	for (int i = 0; i < 6; i++)
	{
		delete m_HexImageTextures[i];
	}
	delete m_gameFont;
}

//Finds a hex using the ID.
Hex* HexHandler::findHexByID(int ID)
{
	for (int i = 0; i < m_hexMapWidth; i++)
	{
		for (int j = 0; j < m_hexMapHeight; j++)
		{
			if (m_mapTiles[i + (j * m_hexMapWidth)]->hexID == ID)
			{
				return m_mapTiles[i + (j * m_hexMapWidth)];
			}
		}
	}
	//If this is returned then no hex could be found.
	return nullptr;
}

//main update for HexHandler
void HexHandler::Update(sf::RenderWindow* window, InputHandler& input)
{

	for (int i = 0; i < m_hexMapWidth; i++)
	{
		for (int j = 0; j < m_hexMapHeight; j++)
		{
			m_mapTiles[i + (j * m_hexMapWidth)]->update(input, this);
		}
	}

	if (!m_combatMode)
	{
		m_date++;
		m_dateText.setString("Date:" + std::to_string(m_date));
		for (int i = 0; i < m_unitsInGame.size(); i++)
		{
			m_unitsInGame[i]->update(input, this);
			if (m_unitsInGame[i]->m_destroyed == true)
			{
				m_unitsInGame[i]->setPosition(-400, -400);
				m_unitsInGame[i]->m_nation = nullptr;
			}
		}
		checkForOccupation();
	}

	if(!initialSetup)
	{
		AIThread = std::thread(AIUpdate, m_nationsInfo, this);
		initialSetup = true;
	}
	if (AIThread.joinable()) 
	{
		AIThread.join();
		AIThread = std::thread(AIUpdate, m_nationsInfo, this);
	}
}

//Static update for multithreading.
void HexHandler::AIUpdate(NationHandler* m_nationsInfo, HexHandler* HexArray)
{
	for (int i = 0; i < m_nationsInfo->m_nationsInGame.size(); i++)
	{
		if (m_nationsInfo->m_nationsInGame[i] != m_nationsInfo->m_currentNation)
		{
			m_nationsInfo->m_nationsInGame[i]->m_nationsAI.update(m_nationsInfo->m_nationsInGame[i], HexArray);
		}
	}
	return;
}


//When called it makes sure that all tiles in the array are in the right position.
void HexHandler::resetPosition()
{
	float buffer = (m_mapTiles[0]->getRadius() * 2) * m_kbufferAdjustment;
	for (int i = 0; i < m_hexMapWidth; i++)
	{
		for (int j = 0; j < m_hexMapHeight; j++)
		{
			if (j % 2 == 0)
			{
				m_mapTiles[i + (j * m_hexMapWidth)]->setPosition(i * buffer, (j * buffer) * m_kbufferAdjustment);
			}
			else
			{
				m_mapTiles[i + (j * m_hexMapWidth)]->setPosition(i * buffer + (m_mapTiles[i + (j * m_hexMapWidth)]->getRadius() * m_kbufferAdjustment), (j * buffer) * m_kbufferAdjustment);
			}
		}
	}
}

//This is called to move the hexes by the amount the user has specified.
void HexHandler::adjustPosition(InputHandler& input)
{
	if (input.m_ofsetPosition.x != 0 || input.m_ofsetPosition.y != 0)
	{
		float buffer = (m_mapTiles[0]->getRadius() * 2) * m_kbufferAdjustment;
		for (int i = 0; i < m_hexMapWidth; i++)
		{
			for (int j = 0; j < m_hexMapHeight; j++)
			{
				if (j % 2 == 0)
				{
					m_mapTiles[i + (j * m_hexMapWidth)]->setPosition(((i * buffer) - input.m_ofsetPosition.x), ((j * buffer) * m_kbufferAdjustment) - input.m_ofsetPosition.y);
				}
				else
				{
					m_mapTiles[i + (j * m_hexMapWidth)]->setPosition(((i * buffer) + (m_mapTiles[i + (j * m_hexMapWidth)]->getRadius() * m_kbufferAdjustment) - input.m_ofsetPosition.x), ((j * buffer) * m_kbufferAdjustment) - input.m_ofsetPosition.y);
				}
			}
		}
		m_updateScreen = true;
	}
}

//This changes the radius of all the hexes in the array.
void HexHandler::changeRadius(float radiusSize)
{
	for (int i = 0; i < m_hexMapHeight; i++)
	{
		for (int j = 0; j < m_hexMapWidth; j++)
		{
			m_mapTiles[i + (j * m_hexMapWidth)]->setRadius(radiusSize);
		}
	}
	resetPosition();
	m_updateScreen = true;
}

//Checks if mouse if over the hex.
Hex* HexHandler::mouseOverHex(sf::RenderWindow* window, InputHandler* input)
{
	Hex* temp = nullptr;
	for (int i = 0; i < getMapWidth(); i++)
	{
		for (int j = 0; j < getMapHeight(); j++)
		{
			if (getHexAtPosition(i, j)->MouseOver(window, *input))
			{
				temp = getHexAtPosition(i, j);
			}
		}
	}
	return temp;
}

//When this is called a unit will be built.
void HexHandler::buildUnit(Nation* nationInfo, HexHandler* mapInfo, Hex* hexToUse)
{
	if (nationInfo->getMoney() > 30 && nationInfo->m_ownedUnits.size() < 10)
	{
		nationInfo->setMoney(nationInfo->getMoney() - 30);
		mapInfo->m_unitsInGame.push_back(new UnitStack(10, 20, true, static_cast<int>(mapInfo->m_unitsInGame.size()), mapInfo));
		nationInfo->m_ownedUnits.push_back(mapInfo->m_unitsInGame[(mapInfo->m_unitsInGame.size() - 1)]);
		nationInfo->m_ownedUnits[(nationInfo->m_ownedUnits.size() - 1)]->m_nation = nationInfo;
		mapInfo->m_unitsInGame[(mapInfo->m_unitsInGame.size() - 1)]->setPosition(hexToUse->getPosition());
		mapInfo->m_unitsInGame[(mapInfo->m_unitsInGame.size() - 1)]->m_latestHex = hexToUse;
	}
}

//Checks whether a hex has a unit of an occupying force on it.
void HexHandler::checkForOccupation()
{
	for (int i = 0; i < m_nationsInfo->m_nationsInGame.size(); i++)
	{
		if (m_nationsInfo->m_nationsInGame[i]->checkAtWar())
		{
			for (int j = 0; j < m_nationsInfo->m_nationsInGame[i]->m_ownedTiles.size(); j++)
			{
				UnitStack* temp = checkEnemyUnitsOnHex(m_nationsInfo->m_nationsInGame[i]->m_ownedTiles[j], m_nationsInfo->m_nationsInGame[i]);
				if (temp != nullptr)
				{
					temp->m_nation->m_ownedTiles.push_back(m_nationsInfo->m_nationsInGame[i]->m_ownedTiles[j]);
					m_nationsInfo->m_nationsInGame[i]->removeHex(m_nationsInfo->m_nationsInGame[i]->m_ownedTiles[j]);
				}
			}
		}
	}
}

//Checks whether any enemy unit stacks are on this hex and if it is, it returns it.
UnitStack* HexHandler::checkEnemyUnitsOnHex(Hex* hexToCheck, Nation* hexOwner)
{
	for (int i = 0; i < m_nationsInfo->m_nationsInGame.size(); i++)
	{
		if (m_nationsInfo->m_nationsInGame[i] != hexOwner)
		{
			if (hexOwner->m_relations.getRelationshipState(m_nationsInfo->m_nationsInGame[i], DiploInteractions::interactionsTypes::war))
			{
 				for (int j = 0; j < m_nationsInfo->m_nationsInGame[i]->m_ownedUnits.size(); j++)
				{
					if (m_nationsInfo->m_nationsInGame[i]->m_ownedUnits[j]->m_latestHex == hexToCheck)
					{
						return m_nationsInfo->m_nationsInGame[i]->m_ownedUnits[j];
					}
				}
			}
		}
	}
	return nullptr;
}
