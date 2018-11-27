#include <iostream>
#include "UIText.h"
#include "UIHandler.h"

static class FPSCounter
{
private:
	float m_framesPerSecond;
	UIText* m_uiText;

public:
	UIHandler* m_handler;

	FPSCounter(UIHandler* handler);
	~FPSCounter();
	void setFPSCounter(const float framesPerSecond);
};

