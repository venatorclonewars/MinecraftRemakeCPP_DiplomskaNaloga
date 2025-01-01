#pragma once
#include <chrono>
#include <OGL3D/OPrerequisites.h>

class OTime
{
public:
	OTime();
	~OTime();

	void onUpdate();

public:
	static OTime* get();
	f32 deltaTime = 0.0f;

private:
	std::chrono::system_clock::time_point m_previousTime;
};

