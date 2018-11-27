#include "UIText.h"
#include "UIBox.h"

//Constructor.
UIText::UIText(const std::string& text, const sf::Font& font, unsigned int characterSize)
	: sf::Text(text, font, characterSize)
	, m_backgroundBox(nullptr)
	,m_active(false)
{
}

//Destructor.
UIText::~UIText()
{
	if (m_backgroundBox != nullptr)
	{
		delete m_backgroundBox;
	}
}

void UIText::setText(const std::string text) 
{
	this->setString(text);
}