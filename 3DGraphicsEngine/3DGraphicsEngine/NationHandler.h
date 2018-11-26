#ifndef NATIONHANDLER_H
#define NATIONHANDLER_H

#include <vector>

class Nation;
class UnitStack;
class HexHandler;

class NationHandler
{
public:
	NationHandler();
	~NationHandler();
	std::vector<Nation*> m_nationsInGame;
	Nation* findNationByTag(const std::string& tag);
	Nation* createNationWithTag(const std::string& tag);

	UnitStack* createUnitWithID(const int unitID, HexHandler* mapInfo);
	std::vector<UnitStack*> m_unitsInGame;
	void createNewNation();

	Nation* m_nationToAddTo;
	void fillNationByTag();
	void update();

	Nation* m_currentNation;

	void selectNation();

};

#endif // NATIONHANDLER_H

