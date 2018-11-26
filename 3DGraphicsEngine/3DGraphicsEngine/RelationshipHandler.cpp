#include "RelationshipHandler.h"

#include "NationHandler.h"
#include "Nation.h"

//Destructor.
RelationshipHandler::~RelationshipHandler()
{
	for (int i = 0; i < m_diploInteractionsState.size(); i++)
	{
		delete m_diploInteractionsState[i];
	}
}

//Constructor.
RelationshipHandler::RelationshipHandler()
{
// 	for (int i = 0; i < nationsInGame->nationsInGame.size(); i++)
// 	{
// 		diploInteractionsState.push_back(new DiploInteractions());
// 		diploInteractionsState[i]->setNationID(nationsInGame->nationsInGame[i]->getCountryTag());
// 	}
}

//Main update for relationshipHandler.
void RelationshipHandler::Update(NationHandler* nations)
{
	for (int i = 0; i < nations->m_nationsInGame.size(); i++)
	{
		if (i == m_diploInteractionsState.size())
		{
			m_diploInteractionsState.push_back(new DiploInteractions());
			m_diploInteractionsState[(m_diploInteractionsState.size() - 1)]->setNationID(nations->m_nationsInGame[i]->getCountryTag());
		}
	}
}

//Sets the relationships of both the nations to the correct value.
void RelationshipHandler::setRelationshipByType(Nation* thisNation, Nation* otherNation, DiploInteractions::interactionsTypes interaction, bool value)
{
	for (int i = 0; i < m_diploInteractionsState.size(); i++)
	{
		if (m_diploInteractionsState[i]->getNationID() == otherNation->getCountryTag())
		{
			m_diploInteractionsState[i]->setInteractionState(interaction, value);
		}
		if (otherNation->m_relations.m_diploInteractionsState[i]->getNationID() == thisNation->getCountryTag())
		{
			otherNation->m_relations.m_diploInteractionsState[i]->setInteractionState(interaction, value);
		}
	}
}

//Returns whether the relationship state is true or false, so whether that specific relationship is currently in effect by the two states.
bool RelationshipHandler::getRelationshipState(Nation* otherNation, DiploInteractions::interactionsTypes interaction)
{
	for (int i = 0; i < m_diploInteractionsState.size(); i++)
	{
		if (m_diploInteractionsState[i]->getNationID() == otherNation->getCountryTag())
		{
			return m_diploInteractionsState[i]->getInteractionState(interaction);
		}
	}
	return false;
}