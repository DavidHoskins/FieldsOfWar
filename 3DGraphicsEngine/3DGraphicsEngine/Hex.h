#ifndef HEX_H
#define HEX_H

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <map>

#include "InputHandler.h"
#include "GameObject.h"

class UnitStack;
class UIHandler;
class UIText;
class UIBox;

class Hex : public GameObject
{
public:
	enum shipTypes{light, heavy, galley, trade, transport};
	enum unitTypes{cavalry, infantry, cannon};
	enum buildingTypes{market, temple, fishery, factory};
	enum tileTypes{grass, hill, field, city, inlandSea, openSea};

	std::map<tileTypes, int> tileTypesToBaseValue {{grass, 10}, {hill, 2}, {field, 15}, {city, 20}, {inlandSea, 5}, {openSea, 0}};


	int hexID;
	bool isDock;

	std::vector<Hex::unitTypes> m_unitBuildQueue;
	std::vector<Hex::shipTypes> m_shipBuildQueue;
	std::vector<Hex::buildingTypes> m_buildingsBuildQueue;
	std::vector<Hex::buildingTypes> m_buildings;

	Hex(float radius, UIHandler* HexUI, sf::Texture** textures);
	Hex(UIHandler* HexUI, sf::Texture** textures);
	~Hex();

	bool MouseOver(sf::RenderWindow* window, InputHandler& input);
	void render(sf::RenderWindow& renderWindow) const override;
	void update(InputHandler& inputHandler, HexHandler* hexHandler) override;

	tileTypes m_tileType;

	void ChangeColour(sf::Color colourChange);

	Hex* m_fastestHex;
	int m_costToReturn = 0;
	int m_baseMovementCost = 0;

	char getCountryTag()const {return m_countryTag;}
	void setCountryTag(char value){m_countryTag = value;}

	int getExpenses()const {return m_expenses;}
	float getIncome()const {return m_income;}
	void setTaxEfficiency(const float value);

	bool getUIActive()const {return m_UIActive;}
	void setUIActive(const bool value){m_UIActive = value;}

	sf::Sprite* m_HexSprite;
	sf::Texture** m_Textures;

private:
	char m_countryTag;

	bool m_hasSetPosition;

	float m_income;
	int m_expenses;
	float m_taxEfficiency;

	int m_baseValue;

	bool m_UIActive;

	void updateExpenses();
	void updateIncome();

	float sign(sf::Vector2i& pointToTest, sf::Vector2f& pointOne, sf::Vector2f& pointTwo);
	bool PointInTriangle(sf::Vector2i& pointToTest, sf::Vector2f& pointOne, sf::Vector2f& pointTwo, sf::Vector2f& pointThree);
	bool isMouseOverHex(float radius, sf::Vector2f& position, sf::RenderWindow* window);

};

#endif // !HEX_H

