#ifndef UITEXT_H
#define UITEXT_H

#include <SFML/Graphics/Text.hpp>

class UIBox;

class UIText : public sf::Text
{
private:
	bool m_active = false;
	int m_UIOrder = 0;

public:
	UIBox* m_backgroundBox;
	~UIText();
	UIText(const std::string& text, const sf::Font& font, unsigned int characterSize);

	bool getIsActive()const {return m_active;} 
	void setIsActive(const bool value){m_active = value;}

	int getUIOrder()const {return m_UIOrder;}
	void setUIOrder(const int value){m_UIOrder = value;}
};

#endif // !UITEXT_H

