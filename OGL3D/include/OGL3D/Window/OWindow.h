#pragma once
#include <OGL3D/Math/ORect.h>

class OWindow
{
public:
	OWindow();
	~OWindow();

	ORect getInnerSize();

	void makeCurrentContext();
	void present(bool vsync);

	virtual void onFocus();
	virtual void onKillFocus();

private:
	void* m_handle = nullptr;
	void* m_context = nullptr;


};

