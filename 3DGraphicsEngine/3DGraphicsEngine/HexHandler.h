#ifndef HEXHANDLER_H
#define HEXHANDLER_H

#include <thread>

#include <SFML/Graphics.hpp>

#include "PathFinder.h"
#include "UnitStack.h"

class NationHandler;
class UIHandler;

class HexHandler
{
private:
	sf::Texture* m_HexImageTextures[6];
	Hex** m_mapTiles;
	const int m_hexMapWidth = 100;
	const int m_hexMapHeight = 100;
	int m_numSuc = 0;

	Hex* m_targetPointOne;
	Hex* m_targetPointTwo;

	const float m_kbufferAdjustment = 0.875f;

	void resetPosition();

	bool m_updateScreen;
	sf::Font* m_gameFont;

	void checkForOccupation();
	UnitStack* checkEnemyUnitsOnHex(Hex* hexToCheck, Nation* hexOwner);

	void hexImageLoading();
	void gameFontLoading();

	static void AIUpdate(NationHandler* m_nationsInfo, HexHandler* HexArray);
	std::thread AIThread;
	bool initialSetup = false;

public:
	HexHandler(UIHandler* UIInfo);
	~HexHandler();
	void Update(sf::RenderWindow* window, InputHandler& input);

	int getMapWidth() const{return m_hexMapWidth;}
	int getMapHeight() const{return m_hexMapHeight;}

	void changeRadius(float radiusSize);

	bool getUpdateScreen() const{return m_updateScreen;}
	void setUpdateScreen(bool value){m_updateScreen = value;}


	//Returns a pointer to the Hex at the position specified.
	Hex* getHexAtPosition(int x, int y) {return m_mapTiles[x + (y * m_hexMapWidth)];}
	Hex* findHexByID(int ID);
	Hex** getHexArray()const {return m_mapTiles;}

	std::vector<UnitStack* > m_unitsInGame;
	NationHandler* m_nationsInfo;

	bool m_combatMode = false;

	void adjustPosition(InputHandler& input);
	int m_date = 1000;
	sf::Text m_dateText;

	Hex* m_hightlightedHex;
	Hex* mouseOverHex(sf::RenderWindow* window, InputHandler* input);
	void buildUnit(Nation* nationInfo, HexHandler* mapInfo, Hex* hexToUse);


	UIHandler* m_UI;
};

#endif // !HEXHANDLER_H

