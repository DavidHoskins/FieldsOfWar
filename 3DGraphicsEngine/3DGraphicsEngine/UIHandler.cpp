#include "UIHandler.h"

#include "UIText.h"
#include "UIBox.h"
#include "HexHandler.h"
#include "Hex.h"
#include "UnitStack.h"
#include "NationHandler.h"
#include "Nation.h"

//Constructor.
UIHandler::UIHandler()
{
	m_gameFont = new sf::Font();
	m_gameFont->loadFromFile("saveFiles/sansation.ttf");
	m_menuBanner = new sf::Texture();
	m_menuBanner->loadFromFile("Images/Banner.png");
	m_topBanner = new sf::Texture();
	m_topBanner->loadFromFile("Images/TopBanner.png");
	createHexUI();
	createDiploUI();
	createCombatUI();
	createUIBoxElement(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(1920.0f, 50.0f), sf::Color::Transparent, *m_topBanner);
	m_panelElements[3]->setUIOrder(0);
	m_panelElements[3]->setIsActive(true);


}

//Creates the UI for the Hexes when selected.
void UIHandler::createHexUI()
{
	//Background UI element.
	createUIBoxElement(sf::Vector2f(1225.0f, 385.0f), sf::Vector2f(700.0f, 700.0f), sf::Color::Transparent, *m_menuBanner);
	m_panelElements[0]->setUIOrder(0);

	//Province name element.
	createUITextElement(sf::Vector2f(1525.0f, 400.0f), 20, sf::Color::White);
	m_textElements[Name]->setString("Province");
	m_textElements[Name]->setUIOrder(1);

	//Income element.
	createUITextElement(sf::Vector2f(1350.0f, 550.0f), 20, sf::Color::White);
	m_textElements[Income]->setString("Income:");
	m_textElements[Income]->setUIOrder(1);

	//Expenses element.
	createUITextElement(sf::Vector2f(1350.0f, 600.0f), 20, sf::Color::White);
	m_textElements[Expenses]->setString("Expenses:");
	m_textElements[Expenses]->setUIOrder(1);

	//Buildings elements.
	createUITextElement(sf::Vector2f(1350.0f, 650.0f), 20, sf::Color::White);
	m_textElements[Buildings]->setString("Buildings:");
	m_textElements[Buildings]->setUIOrder(1);

	//Unit queues.
	createUITextElement(sf::Vector2f(1600.0f, 650.0f), 20, sf::Color::White);
	m_textElements[Units]->setString("Units:");
	m_textElements[Units]->setUIOrder(1);

	//Buildings button.
	createUITextElement(sf::Vector2f(1350.0f, 900.0f), 20, sf::Color::White, sf::Vector2f(200.0f, 40.0f), sf::Color::Blue);
	m_textElements[Buildings_Button]->setString("Buy Buildings");
	m_textElements[Buildings_Button]->setUIOrder(1);

	//Units button.
	createUITextElement(sf::Vector2f(1600.0f, 900.0f), 20, sf::Color::White, sf::Vector2f(200.0f, 40.0f), sf::Color::Blue);
	m_textElements[Units_Button]->setString("Buy Units");
	m_textElements[Units_Button]->setUIOrder(1);
}

//Creates the diplo UI for when it's selected.
void UIHandler::createDiploUI()
{
	//Background UI element.
	createUIBoxElement(sf::Vector2f(1500.0f, 600.0f), sf::Vector2f(700.0f, 700.0f), sf::Color::Transparent, *m_menuBanner);
	m_panelElements[1]->setUIOrder(0);

	//Units button.
	createUITextElement(sf::Vector2f(1550.0f, 680.0f), 10, sf::Color::White, sf::Vector2f(100.0f, 20.0f), sf::Color::Transparent, *m_menuBanner);
	m_textElements[Alliance]->setString("Alliance");
	m_textElements[Alliance]->setUIOrder(1);

	//Units button.
	createUITextElement(sf::Vector2f(1550.0f, 700.0f), 10, sf::Color::White, sf::Vector2f(100.0f, 20.0f), sf::Color::Red);
	m_textElements[War]->setString("War");
	m_textElements[War]->setUIOrder(1);

	//Units button.
	createUITextElement(sf::Vector2f(1550.0f, 720.0f), 10, sf::Color::White, sf::Vector2f(100.0f, 20.0f), sf::Color::Blue);
	m_textElements[RoyalMarriage]->setString("RoyalMarriage");
	m_textElements[RoyalMarriage]->setUIOrder(1);
}

//Create the UI for the combat system.
void UIHandler::createCombatUI()
{
	//Background UI element.
	createUIBoxElement(sf::Vector2f(1500.0f, 600.0f), sf::Vector2f(700.0f, 700.0f), sf::Color::Transparent, *m_menuBanner);
	m_panelElements[2]->setUIOrder(0);

	createUITextElement(sf::Vector2f(1550.0f, 660.0f), 10, sf::Color::White);
	m_textElements[RollValuesAtt]->setString("Roll Value");
	m_textElements[RollValuesAtt]->setUIOrder(1);

	createUITextElement(sf::Vector2f(1650.0f, 660.0f), 10, sf::Color::White);
	m_textElements[RollValuesDef]->setString("Roll Value");
	m_textElements[RollValuesDef]->setUIOrder(1);

	createUITextElement(sf::Vector2f(1550.0f, 680.0f), 10, sf::Color::White);
	m_textElements[LossValuesAtt]->setString("Loses");
	m_textElements[LossValuesAtt]->setUIOrder(1);

	createUITextElement(sf::Vector2f(1650.0f, 680.0f), 10, sf::Color::White);
	m_textElements[LossValuesDef]->setString("Loses");
	m_textElements[LossValuesDef]->setUIOrder(1);

	createUITextElement(sf::Vector2f(1550.0f, 700.0f), 10, sf::Color::White);
	m_textElements[UnitSizesAtt]->setString("Units");
	m_textElements[UnitSizesAtt]->setUIOrder(1);

	createUITextElement(sf::Vector2f(1650.0f, 700.0f), 10, sf::Color::White);
	m_textElements[UnitSizesDef]->setString("Units");
	m_textElements[UnitSizesDef]->setUIOrder(1);
}

//Destructor, deallocate memory here.
UIHandler::~UIHandler()
{
	for (int i = 0; i < m_textElements.size(); i++)
	{
		delete m_textElements[i];
	}
	for (int i = 0; i < m_panelElements.size(); i++)
	{
		delete m_panelElements[i];
	}
	delete m_gameFont;
	delete m_menuBanner;
	delete m_topBanner;
}

//Main render for the UI handler.
void UIHandler::render(sf::RenderWindow* window)
{
	window->setView(window->getDefaultView());
	int currentUIOrder = 0;
	while (currentUIOrder < m_kmaxNumUIOrder)
	{
		for (int i = 0; i < m_panelElements.size(); i++)
		{
			if (m_panelElements[i]->getIsActive())
			{
				if (m_panelElements[i]->getUIOrder() == currentUIOrder)
				{
					window->draw(*m_panelElements[i]);
					if (m_panelElements[i]->m_backgroundImage != nullptr)
					{
						window->draw(*m_panelElements[i]->m_backgroundImage);
					}
				}
			}
		}
		for (int i = 0; i < m_textElements.size(); i++)
		{
			if (m_textElements[i]->getIsActive())
			{
				if (m_textElements[i]->getUIOrder() == currentUIOrder)
				{
					if (m_textElements[i]->m_backgroundBox != nullptr)
					{
						window->draw(*m_textElements[i]->m_backgroundBox);
						if (m_textElements[i]->m_backgroundBox->m_backgroundImage != nullptr)
						{
							window->draw(*m_textElements[i]->m_backgroundBox->m_backgroundImage);
						}
					}
					window->draw(*m_textElements[i]);
				}
			}
		}
		currentUIOrder++;
	}
}

//Main update for UI handler.
void UIHandler::update(InputHandler* inputInfo, HexHandler* mapInfo, NationHandler* nationInfo)
{
	for (int i = 0; i < mapInfo->m_unitsInGame.size(); i++)
	{
		if (mapInfo->m_unitsInGame[i]->getIsSelected())
		{
			return;
		}
	}
	if (m_HexUI)
	{
		updateHexUI(inputInfo, mapInfo, nationInfo);
	}
	else
	{
		setUIElements(false);
	}

	if (m_CombatUI)
	{
		setCombatElements(true);
	}
	else
	{
		setCombatElements(false);
	}

	if (m_DiploUI)
	{
		updateDiploUI(inputInfo, mapInfo, nationInfo);
	}
	else
	{
		setDiploUIElements(false);
	}
}

//Updates the UI for the hexes which includes the buying for buildings and units.
void UIHandler::updateHexUI(InputHandler* inputInfo, HexHandler* mapInfo, NationHandler* nationInfo)
{
	setUIElements(true);
	m_textElements[Expenses]->setString("Expenses: " + std::to_string(mapInfo->m_hightlightedHex->getExpenses()));
	m_textElements[Income]->setString("Income: " + std::to_string(static_cast<int>(mapInfo->m_hightlightedHex->getIncome())));
	if (inputInfo->isMouseDown(m_isReleased))
	{
		if (m_textElements[Units_Button]->m_backgroundBox->IsPointOver(inputInfo->m_mousePosition))
		{
			mapInfo->buildUnit(nationInfo->m_currentNation, mapInfo, mapInfo->m_hightlightedHex);
		}
		if (m_textElements[Buildings_Button]->m_backgroundBox->IsPointOver(inputInfo->m_mousePosition))
		{
			if (nationInfo->m_currentNation->getMoney() > 1000)
			{
				nationInfo->m_currentNation->setMoney(nationInfo->m_currentNation->getMoney() - 1000);
				mapInfo->m_hightlightedHex->m_buildings.push_back(Hex::market);
			}
		}
	}
}

//Updates the UI for the diplomatic interface with buttons for creating or breaking specific relationships with the other nations.
void UIHandler::updateDiploUI(InputHandler* inputInfo, HexHandler* mapInfo, NationHandler* nationInfo)
{
	setDiploUIElements(true);
	if (nationInfo->m_currentNation->m_relations.getRelationshipState(m_diploNation, DiploInteractions::alliance))
	{
		m_textElements[Alliance]->setString("Cancel Alliance");
	}
	else
	{
		m_textElements[Alliance]->setString("Alliance");
	}
	if (nationInfo->m_currentNation->m_relations.getRelationshipState(m_diploNation, DiploInteractions::war))
	{
		m_textElements[War]->setString("Peace");
	}
	else
	{
		m_textElements[War]->setString("Declare War");
	}
	if (nationInfo->m_currentNation->m_relations.getRelationshipState(m_diploNation, DiploInteractions::royalMarriage))
	{
		m_textElements[RoyalMarriage]->setString("Break Marriage");
	}
	else
	{
		m_textElements[RoyalMarriage]->setString("Royal Marry");
	}

	if (inputInfo->isMouseDown(m_isReleased))
	{
		buttonUIUpdate(inputInfo, nationInfo, Alliance);
		buttonUIUpdate(inputInfo, nationInfo, War);
		buttonUIUpdate(inputInfo, nationInfo, RoyalMarriage);
	}
}

//If this button is pressed do the action required of it, since all elements act the same this is simplified here.
void UIHandler::buttonUIUpdate(InputHandler* inputInfo, NationHandler* nationInfo, UIElements elementToCheck)
{
	if (m_textElements[elementToCheck]->m_backgroundBox->IsPointOver(inputInfo->m_mousePosition))
	{
		switch (elementToCheck)
		{
		case Alliance:
			buttonAction(nationInfo, DiploInteractions::alliance);
			break;
		case RoyalMarriage:
			buttonAction(nationInfo, DiploInteractions::royalMarriage);
			break;
		case War:
			buttonAction(nationInfo, DiploInteractions::war);
			break;
		default:
			break;
		}
	}
}

//Completes the action of the button using the interaction enum passed through.
void UIHandler::buttonAction(NationHandler* nationInfo, DiploInteractions::interactionsTypes interaction)
{
	if (nationInfo->m_currentNation->m_relations.getRelationshipState(m_diploNation, interaction))
	{
		nationInfo->m_currentNation->m_relations.setRelationshipByType(nationInfo->m_currentNation, m_diploNation, interaction, false);
	}
	else
	{
		nationInfo->m_currentNation->m_relations.setRelationshipByType(nationInfo->m_currentNation, m_diploNation, interaction, true);
	}
}

//Sets the UI elements to the values input.
void UIHandler::setUIElements(bool value)
{
	m_panelElements[0]->setIsActive(value);
	m_textElements[Name]->setIsActive(value);
	m_textElements[Income]->setIsActive(value);
	m_textElements[Expenses]->setIsActive(value);
	m_textElements[Buildings]->setIsActive(value);
	m_textElements[Units]->setIsActive(value);
	m_textElements[Buildings_Button]->setIsActive(value);
	m_textElements[Units_Button]->setIsActive(value);
}

//Sets all diplo UI elements to the input value.
void UIHandler::setDiploUIElements(bool value)
{
	m_panelElements[1]->setIsActive(value);
	m_textElements[Alliance]->setIsActive(value);
	m_textElements[War]->setIsActive(value);
	m_textElements[RoyalMarriage]->setIsActive(value);
}

//Sets all combatElements to the input value.
void UIHandler::setCombatElements(bool value)
{
	m_panelElements[2]->setIsActive(value);
	m_textElements[RollValuesAtt]->setIsActive(value);
	m_textElements[RollValuesDef]->setIsActive(value);
	m_textElements[LossValuesAtt]->setIsActive(value);
	m_textElements[LossValuesDef]->setIsActive(value);
	m_textElements[UnitSizesAtt]->setIsActive(value);
	m_textElements[UnitSizesDef]->setIsActive(value);
}

//Create a text element using input values, returns a pointer to the UIText so that the class that called it can use the data on its update.
UIText* UIHandler::createUITextElement(const sf::Vector2f& elementPosition, int fontSize, const sf::Color& fontColour)
{
	UIText* temp = new UIText(static_cast<std::string>(""), *m_gameFont, fontSize);
	m_textElements.push_back(temp);
	temp->setFillColor(fontColour);
	temp->setPosition(elementPosition);
	return temp;

}

//Create a text element with a background panel.
UIText* UIHandler::createUITextElement(const sf::Vector2f& elementPosition, int fontSize, const sf::Color& fontColour, sf::Vector2f& boxDimensions, const sf::Color& boxColour)
{
	UIText* temp = createUITextElement(elementPosition, fontSize, fontColour);
	temp->m_backgroundBox = new UIBox(boxDimensions);
	temp->m_backgroundBox->setFillColor(boxColour);
	temp->m_backgroundBox->setPosition(temp->getPosition());
	return temp;
}

//Creates a text element with a background image.
UIText* UIHandler::createUITextElement(const sf::Vector2f& elementPosition, int fontSize, const sf::Color& fontColour, sf::Vector2f& boxDimensions, const sf::Color& boxColour, const sf::Texture& boxImage)
{
	UIText* temp = createUITextElement(elementPosition, fontSize, fontColour, boxDimensions, boxColour);
	temp->m_backgroundBox->m_backgroundImage = new sf::Sprite(boxImage);
	temp->m_backgroundBox->m_backgroundImage->setPosition(elementPosition);
	const sf::Texture* tempTexture = temp->m_backgroundBox->m_backgroundImage->getTexture();
	float width = static_cast<float>(tempTexture->getSize().x);
	float height = static_cast<float>(tempTexture->getSize().y);
	sf::Vector2f actualScale = sf::Vector2f((boxDimensions.x / width), (boxDimensions.y / height));
	temp->m_backgroundBox->m_backgroundImage->setScale(actualScale);
	return temp;
}

//Creates a box element and returns the UI element for use else where.
UIBox* UIHandler::createUIBoxElement(const sf::Vector2f& elementPosition, const sf::Vector2f& elementSize, const sf::Color& fillColor)
{
	UIBox* temp = new UIBox(elementSize);
	temp->setPosition(elementPosition);
	temp->setFillColor(fillColor);
	m_panelElements.push_back(temp);
	return temp;
}

//Creates a box with a set image.
UIBox* UIHandler::createUIBoxElement(const sf::Vector2f& elementPosition, const sf::Vector2f& elementSize, const sf::Color& fillColor, const sf::Texture& boxImage)
{
	UIBox* temp = createUIBoxElement(elementPosition, elementSize, fillColor);
	temp->m_backgroundImage = new sf::Sprite(boxImage);
	temp->m_backgroundImage->setPosition(elementPosition);
	const sf::Texture* tempTexture = temp->m_backgroundImage->getTexture();
	float width = static_cast<float>(tempTexture->getSize().x);
	float height = static_cast<float>(tempTexture->getSize().y);
	sf::Vector2f actualScale = sf::Vector2f((elementSize.x / width), (elementSize.y / height));
	temp->m_backgroundImage->setScale(actualScale);
	return temp;
}

//Updates the combat UI to the right values for output.
void UIHandler::updateCombatInfo(int rollA, int rollD, int unitNumberA, int unitNumberD, int losesA, int losesD)
{
	m_textElements[RollValuesAtt]->setString("Attacker roll: " + std::to_string(rollA));
	m_textElements[RollValuesDef]->setString("Defender roll: " + std::to_string(rollD));
	m_textElements[LossValuesAtt]->setString("Attacker loses: " + std::to_string(losesA));
	m_textElements[LossValuesDef]->setString("Defender loses: " + std::to_string(losesD));
	m_textElements[UnitSizesAtt]->setString("Attacker units: " + std::to_string(unitNumberA));
	m_textElements[UnitSizesDef]->setString("Defender units: " + std::to_string(unitNumberD));
}