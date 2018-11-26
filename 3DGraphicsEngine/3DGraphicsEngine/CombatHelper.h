#ifndef COMBATHELPER_H
#define COMBATHELPER_H

#include "UnitStack.h"

class NationHandler;

class CombatHelper
{
public:
	void update(NationHandler* nations, UIHandler* UISystem);
	bool getCombatMode()const {return m_combatMode;};
private:
	void checkForCombat(NationHandler* nations);
	UnitStack* checkUnits(std::vector<UnitStack*>& firstNationUnits, std::vector<UnitStack*>& secondNationUnits);
	std::vector<UnitStack*> m_unitInCombat;
	std::vector<UnitStack*> fillUnitsInCombat(NationHandler* nations, UnitStack* triggerNation);
	void combat(UIHandler* UISystem);
	void setCombatMode(bool value){m_combatMode = value;};
	bool m_combatMode = false;
};

#endif //!COMBATHELPER_H