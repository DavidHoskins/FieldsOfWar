#include "UIBox.h"

UIBox::UIBox(const sf::Vector2f& size)
	:sf::RectangleShape(size)
	,m_active(false)
{
}

//Destructor.
UIBox::~UIBox()
{
	delete m_backgroundImage;
}

//Checks if a 2D point falls over a UI position.
bool UIBox::IsPointOver(sf::Vector2i& point)
{
	if (point.x > getPosition().x && point.x < getPosition().x + getSize().x)
	{
		if (point.y > getPosition().y && point.y < getPosition().y + getSize().y)
		{
			return true;
		}
	}
	return false;
}