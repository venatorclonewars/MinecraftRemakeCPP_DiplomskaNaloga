#include <OGL3D/Time/OTime.h>
#include <OGL3D/OPrerequisites.h>

OTime::OTime()
{
}

OTime::~OTime()
{
}

void OTime::onUpdate()
{
	//computing delta time
	auto currentTime = std::chrono::system_clock::now();
	auto elapsedSeconds = std::chrono::duration<double>();

	if (m_previousTime.time_since_epoch().count())
		elapsedSeconds = currentTime - m_previousTime;

	m_previousTime = currentTime;

	deltaTime = (f32)elapsedSeconds.count();
}

OTime* OTime::get()
{
	static OTime time;
	return &time;
}
