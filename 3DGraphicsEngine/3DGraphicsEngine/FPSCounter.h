#include <iostream>

static class FPSCounter
{
private:
	float m_framesPerSecond;

public:
	FPSCounter();
	~FPSCounter();
	static void setFPSCounter(const float framesPerSecond);
};

