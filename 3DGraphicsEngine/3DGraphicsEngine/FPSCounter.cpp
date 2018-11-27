#include "FPSCounter.h"

FPSCounter::FPSCounter(UIHandler* handler)
{
	m_handler = handler;
}

FPSCounter::~FPSCounter()
{

}

void FPSCounter::setFPSCounter(const float framesPerSecond) 
{
	std::cout << framesPerSecond << std::endl;
	m_handler->createUITextElement(sf::Vector2f(1900.f, 20.f), 10, sf::Color::Red);
}