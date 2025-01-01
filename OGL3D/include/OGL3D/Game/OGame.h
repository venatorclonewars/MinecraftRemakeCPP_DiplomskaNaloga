#pragma once
class OGame
{
public:
	OGame();
	~OGame();


	void run();
	void quit();

protected:
	bool m_isRunning = true;
};

