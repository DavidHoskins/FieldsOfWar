#include "UnitStack.h"

#include "InputHandler.h"
#include "Hex.h"
#include "HexHandler.h"
#include "Nation.h"
#include "UnitStack.h"

//Constructor.
UnitStack::UnitStack(float radius, int numbSides, bool isLandUnit, int unitID, HexHandler* mapInfo)
	:GameObject(radius, numbSides)
	, m_numberOfUnits(1000)
	, m_isLandUnit(isLandUnit)
	, m_unitID(unitID)
{
	m_landUnits[Hex::infantry] = 1000;
	m_hexArray = mapInfo->getHexArray();
	m_latestHex = mapInfo->getHexAtPosition(0, 0);

	m_shadowEffect = sf::CircleShape((radius * m_bufferMultiplier), numbSides);
	m_shadowEffect.setFillColor(sf::Color::White);
}

//Main render for the unit's graphics.
void UnitStack::render(sf::RenderWindow& renderWindow) const
{
	if (!m_destroyed)
	{
		if (getIsSelected())
		{
			renderWindow.draw(m_shadowEffect);
		}
		renderWindow.draw(*this);
	}
}

//Calculates the cost of all the units in the stack.
float UnitStack::getCost()
{
	float temp = 0;
	if (!m_destroyed)
	{
		if (m_isLandUnit)
		{
			temp += ((m_landUnits[Hex::infantry] / 1000) * unitTypesToCost[Hex::infantry]);
			temp += ((m_landUnits[Hex::cavalry] / 1000)  * unitTypesToCost[Hex::cavalry]);
			temp += ((m_landUnits[Hex::cannon] / 1000)  * unitTypesToCost[Hex::cannon]);
		}
		else
		{
			temp += (m_navalUnits[Hex::light] * shipTypesToCost[Hex::light]);
			temp += (m_navalUnits[Hex::galley] * shipTypesToCost[Hex::galley]);
			temp += (m_navalUnits[Hex::heavy] * shipTypesToCost[Hex::heavy]);
			temp += (m_navalUnits[Hex::trade] * shipTypesToCost[Hex::trade]);
			temp += (m_navalUnits[Hex::transport] * shipTypesToCost[Hex::transport]);
		}
	}
	return temp;
}

//Main update for the unit logic.
void UnitStack::update(InputHandler& inputHandler, HexHandler* hexHandler)
{
	if(getFillColor() == sf::Color::White)
	{
		setFillColor(*m_nation->m_nationColor);
	}
	if(getIsSelected())
	{
		for(int i = 0; i < static_cast<unsigned>(m_pathfinder.m_returnPath.size()); i++)
		{
			m_pathfinder.m_returnPath[i]->ChangeColour(sf::Color::Black);
		}
	}
	if (this->m_nation != hexHandler->m_nationsInfo->m_currentNation)
	{
		m_AI.update(this, hexHandler);
	}
	if (m_targetHex != nullptr)
	{
		m_latestHex->getActive();
  		m_pathfinder.FindPath(m_latestHex, m_targetHex, m_hexArray, m_arrayWidth, m_arrayHeight, true);
		if (m_pathfinder.m_returnPath.size() > 0)
		{
			m_timeLeftToMove = m_pathfinder.m_returnPath[m_pathfinder.m_returnPath.size() - 1]->m_baseMovementCost;
		}
		m_targetHex = nullptr;
	}
	if (m_combatMode == false)
	{
		if (m_pathfinder.m_returnPath.size() > 0)
		{
			if (m_timeLeftToMove == 0)
			{
				m_latestHex = m_pathfinder.m_returnPath[m_pathfinder.m_returnPath.size() - 1];
				sf::Vector2f temp(m_latestHex->getPosition().x + m_latestHex->getRadius(), m_latestHex->getPosition().y + m_latestHex->getRadius());
				this->setPosition(temp);
				temp = sf::Vector2f(m_latestHex->getPosition().x + (m_latestHex->getRadius() * m_shadowPositionMultiplier), m_latestHex->getPosition().y + (m_latestHex->getRadius() * m_shadowPositionMultiplier));
				m_shadowEffect.setPosition(temp);
				m_pathfinder.m_returnPath.pop_back();
				if (m_pathfinder.m_returnPath.size() > 0)
				{
					m_timeLeftToMove = m_pathfinder.m_returnPath[m_pathfinder.m_returnPath.size() - 1]->m_baseMovementCost;
				}
			}
			else
			{
				m_timeLeftToMove--;
			}
		}
		else
		{
			sf::Vector2f temp(m_latestHex->getPosition().x + m_latestHex->getRadius(), m_latestHex->getPosition().y + m_latestHex->getRadius());
			this->setPosition(temp);
			temp = sf::Vector2f(m_latestHex->getPosition().x + (m_latestHex->getRadius() * m_shadowPositionMultiplier), m_latestHex->getPosition().y + (m_latestHex->getRadius() * m_shadowPositionMultiplier));
			m_shadowEffect.setPosition(temp);
		}
	}
}

bool UnitStack::isMouseOver(sf::Vector2i mousePosition,sf::View& windowView, sf::RenderWindow& window)
{
	sf::Vector2i ofset = sf::Vector2i(static_cast<int>(windowView.getCenter().x - window.getDefaultView().getCenter().x), static_cast<int>(windowView.getCenter().y - window.getDefaultView().getCenter().y));
	sf::Vector2f ofsetRadius = sf::Vector2f(window.getView().getSize().x / window.getDefaultView().getSize().x, window.getView().getSize().y / window.getDefaultView().getSize().y);
	mousePosition = sf::Vector2i(ofset.x + mousePosition.x, ofset.y + mousePosition.y);
	float deltaXCent = (getPosition().x - window.getView().getCenter().x);
	float deltaYCent = (getPosition().y - window.getView().getCenter().y);
	sf::Vector2f ofsetViewPort = sf::Vector2f(window.getView().getCenter().x + (deltaXCent / ofsetRadius.x), window.getView().getCenter().y + (deltaYCent / ofsetRadius.y));

	float tempX = static_cast<float>(abs(mousePosition.x - ((ofsetViewPort.x) + (getRadius() / ofsetRadius.x))));
	float tempY = static_cast<float>(abs(mousePosition.y - ((ofsetViewPort.y) + (getRadius() / ofsetRadius.x))));

	if (tempX > getRadius())
	{
		return false;
	}
	else if (tempY > getRadius())
	{
		return false;
	}
	return true;
}
