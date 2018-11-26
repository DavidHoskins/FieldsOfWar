#include "CombatHelper.h"

#include "NationHandler.h"
#include "Nation.h"
#include "DiploInteractions.h"

//Main update.
void CombatHelper::update(NationHandler* nations, UIHandler* UISystem)
{
	if (!getCombatMode())
	{
		checkForCombat(nations);
	}
	else
	{
		combat(UISystem);
	}
}

//Checks whether a combat has been triggered.
void CombatHelper::checkForCombat(NationHandler* nations)
{
	for (int i = 0; i < nations->m_nationsInGame.size(); i++)
	{
		for (int j = 0; j < nations->m_nationsInGame.size(); j++)
		{
			if (i != j)
			{
				if (nations->m_nationsInGame[i]->m_relations.getRelationshipState(nations->m_nationsInGame[j], DiploInteractions::war))
				{
					UnitStack* temp = checkUnits(nations->m_nationsInGame[i]->m_ownedUnits, nations->m_nationsInGame[j]->m_ownedUnits);
					if (temp != nullptr)
					{
						setCombatMode(true);
						m_unitInCombat = fillUnitsInCombat(nations, temp);
						return;
					}
				}
			}
		}
	}
}

//Checks whether any of the two nations units should enter combat 
UnitStack* CombatHelper::checkUnits(std::vector<UnitStack*>& firstNationUnits, std::vector<UnitStack*>& secondNationUnits)
{
	for (int i = 0; i < firstNationUnits.size(); i++)
	{
		for (int j = 0; j < secondNationUnits.size(); j++)
		{
			if (firstNationUnits[i]->m_latestHex == secondNationUnits[j]->m_latestHex)
			{
				return firstNationUnits[i];
			}
		}
	}
	return nullptr;
}

//Returns an array of units that are on the same hex as the unit stack pushed through.
std::vector<UnitStack*> CombatHelper::fillUnitsInCombat(NationHandler* nations, UnitStack* triggerNation)
{
	std::vector<UnitStack*> valuesToReturn;
	for (int i = 0; i < nations->m_nationsInGame.size(); i++)
	{
		for (int j = 0; j < nations->m_nationsInGame[i]->m_ownedUnits.size(); j++)
		{
			if (nations->m_nationsInGame[i]->m_ownedUnits[j]->m_latestHex == triggerNation->m_latestHex)
			{
				valuesToReturn.push_back(nations->m_nationsInGame[i]->m_ownedUnits[j]);
			}
		}
	}
	return valuesToReturn;
}

//Combat calculations.
void CombatHelper::combat(UIHandler* UISystem)
{
	std::vector<UnitStack*> attackers;
	std::vector<UnitStack*> defenders;

	Nation* attackerNation = nullptr;
	Nation* defenderNation = nullptr;

	UISystem->m_CombatUI = true;


	for (int i = 0; i < m_unitInCombat.size(); i++)
	{
		if (attackerNation == nullptr)
		{
			attackerNation = m_unitInCombat[i]->m_nation;
			attackers.push_back(m_unitInCombat[i]);
		}
		else if (m_unitInCombat[i]->m_nation == attackerNation)
		{
			attackers.push_back(m_unitInCombat[i]);
		}
		else if (defenderNation == nullptr)
		{
			defenderNation = m_unitInCombat[i]->m_nation;
			defenders.push_back(m_unitInCombat[i]);
		}
		else if (m_unitInCombat[i]->m_nation == defenderNation)
		{
			defenders.push_back(m_unitInCombat[i]);
		}
	}

	int attackerInf = 0;
	int defenderInf = 0;

	for (int i = 0; i < attackers.size(); i++)
	{
		attackerInf += attackers[i]->m_landUnits[Hex::infantry];
	}

	for (int i = 0; i < defenders.size(); i++)
	{
		defenderInf += defenders[i]->m_landUnits[Hex::infantry];
	}

	if (attackerInf == 0 || defenderInf == 0)
	{
		UISystem->m_CombatUI = false;
		setCombatMode(false);
	}

	srand(static_cast<unsigned int>(time(NULL)));
	int attackRoll = (rand() % 10);
	int defenderRoll = (rand() % 10);
	float attackVal = (((float) attackerInf / 100.0f) * (float) attackRoll);
	float defenderVal = (((float) defenderInf / 100.0f) * (float) defenderRoll);

	int attackRemaining = (int) attackVal;
	int defendRemaining = (int) defenderVal;

	UISystem->updateCombatInfo(attackRoll, defenderRoll, attackerInf, defenderInf, (int) defenderVal, (int) attackVal);

	for (int i = 0; i < defenders.size(); i++)
	{
		if (defenders[i]->m_landUnits[Hex::infantry] < attackRemaining)
		{
			attackRemaining -= defenders[i]->m_landUnits[Hex::infantry];
			defenders[i]->m_landUnits[Hex::infantry] = 0;
			defenders[i]->m_destroyed = true;
		}
		else
		{
			defenders[i]->m_landUnits[Hex::infantry] -= attackRemaining;
			attackRemaining = 0;
		}
	}

	for (int i = 0; i < attackers.size(); i++)
	{
		if (attackers[i]->m_landUnits[Hex::infantry] < defendRemaining)
		{
			defendRemaining -= attackers[i]->m_landUnits[Hex::infantry];
			attackers[i]->m_landUnits[Hex::infantry] = 0;
			attackers[i]->m_destroyed = true;
		}
		else
		{
			attackers[i]->m_landUnits[Hex::infantry] -= defendRemaining;
			defendRemaining = 0;
		}
	}
}
