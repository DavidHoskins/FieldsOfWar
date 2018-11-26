#ifndef UIBOX_H
#define UIBOX_H

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>

class UIBox : public sf::RectangleShape
{
public:
	UIBox(const sf::Vector2f& size);
	UIBox(const sf::Vector2f& size, const sf::Texture& texture);
	~UIBox();
	bool IsPointOver(sf::Vector2i& point);
	bool getIsActive()const {return m_active;}
	void setIsActive(const bool value){m_active = value;}
	
	int getUIOrder()const {return m_UIOrder;}
	void setUIOrder(const int value){m_UIOrder = value;}

	sf::Sprite* m_backgroundImage;

private:
	bool m_active;
	int m_UIOrder;
};

#endif //!UIBOX_H

