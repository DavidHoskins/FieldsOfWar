#include <iostream>
#include "UIText.h"
#include "UIHandler.h"

static class FPSCounter
{
private:
	float m_framesPerSecond;
	UIHandler m_handler;

public:
	FPSCounter(UIHandler& handler);
	~FPSCounter();
	static void setFPSCounter(const float framesPerSecond);
};

