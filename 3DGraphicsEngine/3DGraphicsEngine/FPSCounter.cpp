#include "FPSCounter.h"

FPSCounter::FPSCounter(UIHandler* handler)
{
	m_handler = handler;
	m_uiText = m_handler->createUITextElement(sf::Vector2f(1800.f, 40.f), 25, sf::Color::Red);
}

FPSCounter::~FPSCounter()
{

}

void FPSCounter::setFPSCounter(const float framesPerSecond)
{
	//std::cout << framesPerSecond << std::endl;
	m_uiText->setText(std::to_string(static_cast<int>(ceil(framesPerSecond))));
	m_handler->setFPSElements(true);
}