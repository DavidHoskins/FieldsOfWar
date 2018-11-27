#include "Hex.h"

#include "UnitStack.h"
#include "UIHandler.h"
#include "UIText.h"
#include "UIBox.h"

//Constructor with defined radius
Hex::Hex(float radius, UIHandler* HexUI, sf::Texture** textures)
	:GameObject(radius, 6)
	, m_fastestHex(nullptr)
	,m_countryTag('n')
	,m_income(0)
	,m_expenses(0)
	,m_taxEfficiency(1.0f)
	,m_baseValue(0)
	, m_hasSetPosition(false)
{
	setFillColor(sf::Color::Transparent);
	setOutlineColor(sf::Color::Black);
	setOutlineThickness(5.0f);
	m_tileType = grass;
	m_Textures = textures;
	m_HexSprite = new sf::Sprite(*m_Textures[Hex::grass]);
	sf::Vector2f temp = sf::Vector2f(this->getPosition().x, this->getPosition().y);
	m_HexSprite->setPosition (temp);
	float width = m_Textures[Hex::inlandSea]->getSize().x;
	float height = m_Textures[Hex::inlandSea]->getSize().y;
	sf::Vector2f actualScale = sf::Vector2f(((radius * 2) / width), ((radius * 2) / height));
	m_HexSprite->setScale(actualScale);
	m_updatedRender = false;
}

//Basic constructor.
Hex::Hex(UIHandler* HexUI, sf::Texture** textures)
	:GameObject(80, 6)
	,m_fastestHex(nullptr)
	, m_countryTag('n')
	, m_income(0)
	, m_expenses(0)
	, m_taxEfficiency(1.0f)
	, m_baseValue(0)
{
	setFillColor(sf::Color::Transparent);
	setOutlineColor(sf::Color::Black);
	setOutlineThickness(5.0f);
	m_tileType = grass;
	m_Textures = textures;
	m_HexSprite = new sf::Sprite(*m_Textures[Hex::grass]);
	m_HexSprite->setPosition(this->getPosition());
	m_updatedRender = false;
}

//Destructor.
Hex::~Hex()
{
	m_fastestHex = nullptr;
	delete m_HexSprite;
}

//Checks if the mouse is over the hex.
bool Hex::MouseOver(sf::RenderWindow* window, InputHandler& input)
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
	sf::Vector2i ofset = sf::Vector2i(static_cast<int>(window->getView().getCenter().x - window->getDefaultView().getCenter().x), static_cast<int>(window->getView().getCenter().y - window->getDefaultView().getCenter().y));
	mousePosition = sf::Vector2i(ofset.x + mousePosition.x, ofset.y + mousePosition.y);
	int deltaX = static_cast<int>(abs(mousePosition.x - (getPosition().x + 80.0f)));
	int deltaY = static_cast<int>(abs(mousePosition.y - (getPosition().y + 80.0f)));
	int radius = static_cast<int>(getRadius());

	sf::Vector2f currentPosition = getPosition();
		if (isMouseOverHex(getRadius(), currentPosition, window))
		{
			if (!input.m_fillNation && input.m_editorMode)
			{
				switch (input.m_currentTileType)
				{
				case 0:
					m_tileType = grass;
					break;
				case 1:
					m_tileType = hill;
					break;
				case 2:
					m_tileType = field;
					break;
				case 3:
					m_tileType = city;
					break;
				case 4:
					m_tileType = inlandSea;
					break;
				case 5:
					m_tileType = openSea;
					break;
				case 6:
					return true;
					break;
				default:
					break;
				}
			}
			return true;
		}
		return false;
}

//Checks whether the mouse falls within a hex around the position of the object with a specified radius.
bool Hex::isMouseOverHex(float radius, sf::Vector2f& position, sf::RenderWindow* window)
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
	sf::Vector2f ofsetRadius = sf::Vector2f(window->getView().getSize().x / window->getDefaultView().getSize().x, window->getView().getSize().y / window->getDefaultView().getSize().y);
	sf::Vector2i ofset = sf::Vector2i(static_cast<int>(window->getView().getCenter().x - window->getDefaultView().getCenter().x), static_cast<int>(window->getView().getCenter().y - window->getDefaultView().getCenter().y));
	mousePosition = sf::Vector2i(ofset.x + mousePosition.x, ofset.y + mousePosition.y);

	float deltaXCent = (position.x - window->getView().getCenter().x);
	float deltaYCent = (position.y - window->getView().getCenter().y);
	sf::Vector2f ofsetViewPort = sf::Vector2f(window->getView().getCenter().x + (deltaXCent / ofsetRadius.x), window->getView().getCenter().y + (deltaYCent / ofsetRadius.y));
	int deltaX = static_cast<int>(abs(mousePosition.x - ((ofsetViewPort.x) + (radius / ofsetRadius.x))));
	int deltaY = static_cast<int>(abs(mousePosition.y - ((ofsetViewPort.y) + (radius / ofsetRadius.x))));


	if (deltaX >  (radius / ofsetRadius.x) || deltaY >  (radius / ofsetRadius.y))
	{
		return false;
	}
	else if (deltaX > (((radius / ofsetRadius.x) * 0.85f)) || deltaY > (((radius / ofsetRadius.y) * 0.5f)))
	{
		//Checks if outside the first check but inside the triangles that make up the edge.
		if (PointInTriangle(sf::Vector2i(deltaX, deltaY), sf::Vector2f(0.0f, (radius / ofsetRadius.x)), sf::Vector2f((0.85f * (radius / ofsetRadius.x)), ((radius / ofsetRadius.x) * 0.5f)), sf::Vector2f(-(0.85f * (radius / ofsetRadius.x)), ((radius / ofsetRadius.x) * 0.5f))))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return true;
}

//returns true if a point to test falls within a triangle created by the the passed through points.
bool Hex::PointInTriangle(sf::Vector2i& pointToTest, sf::Vector2f& pointOne, sf::Vector2f& pointTwo, sf::Vector2f& pointThree)
{
	bool b1, b2, b3;

	b1 = sign(pointToTest, pointOne, pointTwo) < 0.0f;
	b2 = sign(pointToTest, pointTwo, pointThree) < 0.0f;
	b3 = sign(pointToTest, pointThree, pointOne) < 0.0f;

	return ((b1 == b2) && (b2 == b3));

}

//returns the product of the points input.
float Hex::sign(sf::Vector2i& pointToTest, sf::Vector2f& pointOne, sf::Vector2f& pointTwo)
{
	return (pointToTest.x - pointTwo.x) * (pointOne.y - pointTwo.y) - (pointOne.x - pointTwo.x) * (pointToTest.y - pointTwo.y);
}

//Main update for hex.
void Hex::update(InputHandler& inputHandler, HexHandler* hexHandler)
{
	m_updatedRender = true;
	if (!m_hasSetPosition)
	{
		sf::Vector2f temp = sf::Vector2f(this->getPosition().x, this->getPosition().y);
		m_HexSprite->setPosition(temp);
		m_hasSetPosition = true;
	}
	switch (m_tileType)
	{
	case grass:
		if (m_HexSprite->getTexture() != m_Textures[m_tileType])
		m_HexSprite->setTexture(*m_Textures[m_tileType]);
		m_baseMovementCost = 2;
		break;
	case hill:
		if (m_HexSprite->getTexture() != m_Textures[m_tileType])
		m_HexSprite->setTexture(*m_Textures[m_tileType]);
		m_baseMovementCost = 3;
		break;
	case field:
		if(m_HexSprite->getTexture() != m_Textures[m_tileType])
		m_HexSprite->setTexture(*m_Textures[m_tileType]);
		m_baseMovementCost = 1;
		break;
	case city:
		if (m_HexSprite->getTexture() != m_Textures[m_tileType])
		m_HexSprite->setTexture(*m_Textures[m_tileType]);
		m_baseMovementCost = 1;
		break;
	case inlandSea:
		if (m_HexSprite->getTexture() != m_Textures[m_tileType])
		m_HexSprite->setTexture(*m_Textures[m_tileType]);
		m_baseMovementCost = 2;
		break;
	case openSea:
		if (m_HexSprite->getTexture() != m_Textures[m_tileType])
		m_HexSprite->setTexture(*m_Textures[m_tileType]);
		m_baseMovementCost = 4;
		break;
	default:
		break;
	}
	updateIncome();
}

//Main render for hex
void Hex::render(sf::RenderWindow& renderWindow) const
{
	//renderWindow.draw(*this);
	renderWindow.draw(*m_HexSprite);
}

//This changes the colour of the Hex depending on what is passed through.
void Hex::ChangeColour(sf::Color colourChange)
{
		//this->setOutlineColor(colourChange);
	if (m_updatedRender) 
	{
		m_updatedRender = false;
		this->m_HexSprite->setColor(colourChange);
	}
}


//Setter with checks the value doesn't go over 1 as this is used as a multiplier.
void Hex::setTaxEfficiency(const float value)
{
	if (value >= 1.0f)
	{
		m_taxEfficiency = 1.0f;
	}
	else
	{
		m_taxEfficiency = value;
	}
}

//Calculates the expenses generated by this hex.
void Hex::updateExpenses()
{

}

//Calculates the income generated by this hex.
void Hex::updateIncome()
{
	m_baseValue = tileTypesToBaseValue[m_tileType];
	m_income = m_baseValue * m_taxEfficiency;
	if (m_buildings.size() > 0)
	{
		m_income += (m_income * (static_cast<float>(m_buildings.size()) / 10.0f));
	}
}

