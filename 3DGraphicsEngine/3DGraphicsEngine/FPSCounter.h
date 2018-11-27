#include <iostream>
#include "UIText.h"
#include "UIHandler.h"

static class FPSCounter
{
private:
	float m_framesPerSecond;

public:
	UIHandler* m_handler = new UIHandler();

	FPSCounter(UIHandler* handler);
	~FPSCounter();
	void setFPSCounter(const float framesPerSecond);
};

