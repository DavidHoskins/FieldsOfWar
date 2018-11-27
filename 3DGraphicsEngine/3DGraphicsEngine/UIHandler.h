#ifndef UIHANDLER_H
#define UIHANDLER_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <string.h>

#include "NationHandler.h"

#include "DiploInteractions.h"

class UIText;
class UIBox;
class InputHandler;
class HexHandler;

class UIHandler
{
private:
	enum UIElements{Name, Income, Expenses, Buildings, Units, Buildings_Button, Units_Button, Alliance, War, RoyalMarriage, RollValuesAtt, RollValuesDef, 
		LossValuesAtt, LossValuesDef, UnitSizesAtt, UnitSizesDef};

	sf::Font* m_gameFont;
	sf::Texture* m_menuBanner;
	sf::Texture* m_topBanner;

	std::vector<UIText*> m_textElements;
	std::vector<UIBox*> m_panelElements;

	const int m_kmaxNumUIOrder = 6;
	void setUIElements(bool value);
	void setDiploUIElements(bool value);
	void setCombatElements(bool value);

	void createHexUI();
	void createDiploUI();
	void createCombatUI();

	void updateHexUI(InputHandler* inputInfo, HexHandler* mapInfo, NationHandler* nationInfo);
	void updateDiploUI(InputHandler* inputInfo, HexHandler* mapInfo, NationHandler* nationInfo);

	void buttonUIUpdate(InputHandler* inputInfo, NationHandler* nationInfo, UIElements elementToCheck);
	void buttonAction(NationHandler* nationInfo, DiploInteractions::interactionsTypes interaction);

	sf::Color textColor = sf::Color::Black;
public:
	UIHandler();
	~UIHandler();

	void update(InputHandler* inputInfo, HexHandler* mapInfo, NationHandler* nationInfo);
	void render(sf::RenderWindow* window);

	UIText* createUITextElement(const sf::Vector2f& elementPosition, int fontSize, const sf::Color& fontColour);
	UIText* createUITextElement(const sf::Vector2f& elementPosition, int fontSize, const sf::Color& fontColour, sf::Vector2f& boxDimensions, const sf::Color& boxColour);
	UIText* createUITextElement(const sf::Vector2f& elementPosition, int fontSize, const sf::Color& fontColour, sf::Vector2f& boxDimensions, const sf::Color& boxColour, const sf::Texture& boxImage);
	UIBox* createUIBoxElement(const sf::Vector2f& elementPosition, const sf::Vector2f& elementSize, const sf::Color& fillColor);
	UIBox* createUIBoxElement(const sf::Vector2f& elementPosition, const sf::Vector2f& elementSize, const sf::Color& fillColor, const sf::Texture& boxImage);

	HexHandler* m_mapInfo;

	bool m_HexUI;
	bool m_DiploUI;
	bool m_CombatUI;

	Nation* m_diploNation;

	bool m_isReleased = true;

	void updateCombatInfo(int rollA, int rollD, int unitNumberA, int unitNumberD, int losesA, int losesD);

};

#endif //!UIHANDLER_H

