#ifndef NATION_H
#define NATION_H

#include <vector>

#include "Hex.h"
#include "UIText.h"
#include "AI.h"
#include "RelationshipHandler.h"

class UnitStack;
class UIText;

class Nation
{
private:

	std::string m_countryTag;
	std::string m_countryName;
	int m_money;
	int m_income;
	int m_expenses;

	void calculateIncome();
	void calculateExpenses();


public:
	Nation(const int mapWidth, const int mapHeight);
	~Nation();

	UIText* m_NationText;

	sf::Color* m_nationColor;

	void update();

	std::string getCountryTag()const {return m_countryTag;}
	void setCountryTag(std::string value){m_countryTag = value;}

	std::string getCountryName()const {return m_countryName;}
	void setCountryName(std::string value){m_countryName = value;}

	bool ownsHex(Hex* hexToTest);

	std::vector<Hex*> m_ownedTiles;
	std::vector<UnitStack*> m_ownedUnits;

	std::vector<int> m_ownedUnitsID;
	std::vector<int> m_ownedProvincesID;

	int getMoney()const {return m_money;}
	void setMoney(const int value){m_money = value;}
	void addMoney(const int value){m_money += value;}

	void setIncome(const int value){m_income = value;}
	int getIncome()const {return m_income;}

	void setExpenses(const int value){m_expenses = value;}
	int getExpenses()const {return m_expenses;}

	void removeHex(Hex* hexToRemove);
	bool checkAtWar();

	AI m_nationsAI;
	RelationshipHandler m_relations;


};

#endif //!NATION_H

