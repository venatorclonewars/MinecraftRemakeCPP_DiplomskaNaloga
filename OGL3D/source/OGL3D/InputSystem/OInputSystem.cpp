#include <OGL3D/InputSystem/OInputSystem.h>
#include <OGL3D/OPrerequisites.h>
#include <Windows.h>

OInputSystem::OInputSystem()
{
}

OInputSystem::~OInputSystem()
{
}

void OInputSystem::onUpdate()
{
	POINT currentMousePos = {};
	::GetCursorPos(&currentMousePos);

	if (m_firstTime)
	{
		m_oldMousePos = OPoint(currentMousePos.x, currentMousePos.y);
		m_firstTime = false;
	}

	if (currentMousePos.x != m_oldMousePos.m_x || currentMousePos.y != m_oldMousePos.m_y)
	{ 
		// MOUSE move event
		std::map<OInputListener*, OInputListener*>::iterator it = m_mapListener.begin();
		while (it != m_mapListener.end())
		{
			it->second->onMouseMove(OPoint(currentMousePos.x, currentMousePos.y));
			++it;
		}
	}

	m_oldMousePos = OPoint(currentMousePos.x, currentMousePos.y);

	if (::GetKeyboardState(m_keysState))
	{
		for(ui32 i = 0; i < 256; i++)
		{
			// KEY is down
			if (m_keysState[i] & 0x80)
			{
				std::map<OInputListener*, OInputListener*>::iterator it = m_mapListener.begin();
				while (it != m_mapListener.end())
				{
					if (i == VK_LBUTTON)
					{
						if (m_keysState[i] != m_oldKeysState[i])
							it->second->onLeftMouseDown(OPoint(currentMousePos.x, currentMousePos.y));
							
					}

					else if (i == VK_RBUTTON)
					{
						if (m_keysState[i] != m_oldKeysState[i])
							it->second->onRightMouseDown(OPoint(currentMousePos.x, currentMousePos.y));

					}
					else
						it->second->onKeyDown(i);

					++it;
				}
			}
			else // KEY is up
			{
				if (m_keysState[i] != m_oldKeysState[i])
				{
					std::map<OInputListener*, OInputListener*>::iterator it = m_mapListener.begin();
					
					if (i == VK_LBUTTON)
					{
							it->second->onLeftMouseUp(OPoint(currentMousePos.x, currentMousePos.y));
					}

					else if (i == VK_RBUTTON)
					{
						it->second->onRightMouseUp(OPoint(currentMousePos.x, currentMousePos.y));
					}

					else
						it->second->onKeyUp(i);
				}
			}
		}

		// Store current key states to old key states buffer
		::memcpy(m_oldKeysState, m_keysState, sizeof(unsigned char)*256);
	}
}

void OInputSystem::addListener(OInputListener* listener)
{
	m_mapListener.insert(std::make_pair<OInputListener*, OInputListener*>(std::forward<OInputListener*>(listener), std::forward<OInputListener*>(listener)));
}

void OInputSystem::removeListener(OInputListener* listener)
{
	std::map<OInputListener*, OInputListener*>::iterator it = m_mapListener.find(listener);
	if(it != m_mapListener.end())
	{
		m_mapListener.erase(it);
	}
}

void OInputSystem::setCursorPosition(const OPoint& pos)
{
	::SetCursorPos(pos.m_x, pos.m_y);
}

void OInputSystem::showCursor(bool show)
{
	::ShowCursor(show);
}

OInputSystem* OInputSystem::get()
{
	static OInputSystem system;
	return &system;
}
