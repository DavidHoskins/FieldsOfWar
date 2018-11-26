#include "NationHandler.h"

#include <iostream>

#include "Nation.h"
#include "UnitStack.h"


//Constructor.
NationHandler::NationHandler()
{

}

//Clear up data here destructor.
NationHandler::~NationHandler()
{
	for (int i = 0; i < m_nationsInGame.size(); i++)
	{
		delete m_nationsInGame[i];
	}
	for (int j = 0; j < m_unitsInGame.size(); j++)
	{
		m_unitsInGame[j] = nullptr;
	}
}

//Main update.
void NationHandler::update()
{
	for (int i = 0; i < m_nationsInGame.size(); i++)
	{
		m_nationsInGame[i]->update();
		m_nationsInGame[i]->m_relations.Update(this);
	}
}

//Finds and returns the nation with the passed through tag.
Nation* NationHandler::findNationByTag(const std::string& tag)
{
	for (int i = 0; i < m_nationsInGame.size(); i++)
	{
		if (tag == m_nationsInGame[i]->getCountryTag())
		{
			return m_nationsInGame[i];
		}
	}
	return nullptr;
}

//Creates a nation with a specified tag a returns a pointer to that nation.
Nation* NationHandler::createNationWithTag(const std::string& tag)
{
	m_nationsInGame.push_back(new Nation(0, 0));
	m_nationsInGame[(m_nationsInGame.size() - 1)]->setCountryTag(tag);
	return m_nationsInGame[(m_nationsInGame.size() - 1)];

}

//Creates a new unit with a specified ID and returns the unit.
UnitStack* NationHandler::createUnitWithID(const int unitID, HexHandler* mapInfo)
{
	m_unitsInGame.push_back(new UnitStack(80, 20, false, unitID, mapInfo));
	return m_unitsInGame[(m_unitsInGame.size() - 1)];
}

//Creates a new nation by prompting the user for information.
void NationHandler::createNewNation()
{
	std::string nationTag;
	std::string nationName;
	std::cout << "Enter Nation TAG" << std::endl;
	std::cin >> nationTag;
	std::cout << "Enter Nation Name" << std::endl;
	std::cin >> nationName;

	Nation* temp = createNationWithTag(nationTag);
	temp->setCountryName(nationName);

}

//Fills a nation depending on the information input by the user.
void NationHandler::fillNationByTag()
{
	Nation* temp = nullptr;
	if (m_nationsInGame.size() == 0)
	{
		std::cout << "Error no nations in game please create a nation to fill a nation" << std::endl;
		return;
	}
	while (temp == nullptr)
	{
		std::string nationTag;
		std::cout << "Enter Nation TAG" << std::endl;
		std::cin >> nationTag;
		temp = findNationByTag(nationTag);
		if (temp == nullptr)
		{
			std::cout << "Error tag not found" << std::endl;
			return;
		}
	}
	m_nationToAddTo = temp;
}

//Selects a nation from the list to play as, this becomes the active country.
void NationHandler::selectNation()
{
	Nation* temp = nullptr;
	if (m_nationsInGame.size() == 0)
	{
		std::cout << "Error no nations in game please create a nation to fill a nation" << std::endl;
		return;
	}
	while (temp == nullptr)
	{
		std::string nationTag;
		std::cout << "Enter Nation TAG" << std::endl;
		std::cin >> nationTag;
		temp = findNationByTag(nationTag);
		if (temp == nullptr)
		{
			std::cout << "Error tag not found" << std::endl;
			return;
		}
	}
	m_currentNation = temp;
}