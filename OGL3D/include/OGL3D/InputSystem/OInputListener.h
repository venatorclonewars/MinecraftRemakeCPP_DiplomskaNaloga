#pragma once
#include <OGL3D/Math/OPoint.h>

class OInputListener
{
public:
	OInputListener()
	{

	}
	~OInputListener()
	{

	}

	// KEYBOARD pure virtual callback functions
	virtual void onKeyDown(int key) = 0;
	virtual void onKeyUp(int key) = 0;

	// MOUSE pure virtual callback functions
	virtual void onMouseMove(const OPoint& mousePos) = 0;
	
	virtual void onLeftMouseUp(const OPoint& deltaMousePos) = 0;
	virtual void onLeftMouseDown(const OPoint& deltaMousePos) = 0;
	
	virtual void onRightMouseUp(const OPoint& deltaMousePos) = 0;
	virtual void onRightMouseDown(const OPoint& deltaMousePos) = 0;

};
