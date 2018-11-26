#pragma once
#include <vector>

#include "UnitAINode.h"
#include "AttackEnemy.h"
#include "AttackProvinceNode.h"
#include "HuntEnemyUnits.h"
#include "AvoidEnemy.h"
#include "AttackTerritory.h"


class UnitStack;
class Hex;
class HexHandler;

class UnitAI
{

private:
	//Used to store all AILogic nodes.
	std::vector<UnitAINode*> m_AILogic;
	
	//Used for the current AI logic path.
	std::vector<UnitAINode*> m_currentAIPath;

	void checkPreconditions(UnitStack* currentUnit, HexHandler* HexArray);
	void checkInRange(UnitStack* currentUnit, HexHandler* HexArray);
	void checkMoreUnitsInStack(UnitStack* currentUnit, HexHandler* HexArray);

public:
	std::vector<bool*> m_preconditionsState;
	void update(UnitStack* currentUnit, HexHandler* HexArray);
	UnitAI();
	~UnitAI();
};

