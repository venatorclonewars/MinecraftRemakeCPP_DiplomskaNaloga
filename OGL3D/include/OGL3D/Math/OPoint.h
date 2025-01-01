#pragma once

class OPoint
{
public:
	OPoint() : m_x(0), m_y(0)
	{

	}
	OPoint(int x, int y) : m_x(x), m_y(y)
	{

	}
	OPoint(const OPoint& point) : m_x(point.m_x), m_y(point.m_y)
	{

	}
	~OPoint()
	{

	}

public:
	int m_x = 0;
	int m_y = 0;
};
