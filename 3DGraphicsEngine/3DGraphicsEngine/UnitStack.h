#ifndef UNITSTACK_H
#define UNITSTACK_H

#include "GameObject.h"

#include "PathFinder.h"
#include "UnitAI.h"

class Hex;
class UnitStack : public GameObject
{
private:
	std::map<Hex::unitTypes, float> unitTypesToCost {{Hex::infantry, 20.0f}, {Hex::cavalry, 60.0f}, {Hex::cannon, 100.0f}};
	std::map<Hex::shipTypes, float> shipTypesToCost {{Hex::light, 20.0f}, {Hex::heavy, 35.0f}, {Hex::galley, 12.5f}, {Hex::trade, 25.0f}, {Hex::transport, 15.0f}};

	//Bool checks whether its a land or navy unit
	bool m_isLandUnit;
	//Number of units in the stack, if they were at full strength in 1000s.
	int m_numberOfUnits;
	//Number of units actually in the stack. So full strength stack - the casualties. 
	int m_numberOfLivingUnits;

	int m_unitID;

	PathFinder m_pathfinder;

	bool m_isSelected;
	bool m_updateImage;

	const int m_arrayWidth = 100;
	const int m_arrayHeight = 100;

	int m_timeLeftToMove = 0;

	sf::CircleShape m_shadowEffect;

	float m_bufferMultiplier = 1.1f;
	float m_shadowPositionMultiplier = 0.975f;

public:
	UnitStack(float radius, int numbSides, bool isLandUnit, int unitID, HexHandler* mapInfo);

	//Getters and setters
	int getNumberOfUnits()const {return m_numberOfUnits;}
	void setNumberOfUnits(int value) { m_numberOfUnits = value;}

	int getNumberOfLivingUnits()const { return m_numberOfLivingUnits;}
	void setNumberOfLivingUnits(int value){m_numberOfLivingUnits = value;}

	bool getIsLandUnit()const {return m_isLandUnit;}
	void setIsLandUnit(bool value) {m_isLandUnit = value;}

	bool getIsSelected()const {return m_isSelected;}
	void setIsSelected(bool value) { m_isSelected = value;}

	bool getUpdateImage()const {return m_updateImage;}

	void render(sf::RenderWindow& renderWindow) const override;
	void update(InputHandler& inputHandler, HexHandler* hexHandler) override;

	float getCost();

	int getUnitID()const {return m_unitID;}

	Hex* m_targetHex;
	Hex** m_hexArray;

	Hex* m_latestHex;

	bool isMouseOver(sf::Vector2i mousePosition, sf::View& windowView, sf::RenderWindow& window);

	int m_landUnits[3];
	int m_navalUnits[5];

	bool m_combatMode = false;
	bool m_destroyed = false;

	Nation* m_nation;
	UnitAI m_AI;

	Hex* m_AITargetHex;
};

#endif //!UNITSTACK_H

