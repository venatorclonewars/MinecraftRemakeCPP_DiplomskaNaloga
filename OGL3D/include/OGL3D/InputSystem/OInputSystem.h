#pragma once
#include <OGL3D/InputSystem/OInputListener.h>
#include <map>
#include <OGL3D/Math/OPoint.h>

class OInputSystem
{
public:
	OInputSystem();
	~OInputSystem();

	void onUpdate();

	void addListener(OInputListener* listener);
	void removeListener(OInputListener* listener);

	void setCursorPosition(const OPoint& pos);
	void showCursor(bool show);
	
public:
	static OInputSystem* get();
private:
	std::map<OInputListener*, OInputListener*> m_mapListener;
	unsigned char m_keysState[256] = {};
	unsigned char m_oldKeysState[256] = {};

	OPoint m_oldMousePos;
	bool m_firstTime = true;
};

