#pragma once
#include <OGL3D/OPrerequisites.h>

class ORect
{
public:
	ORect(){}
	ORect(i32 width, i32 height) : width(width), height(height) {}
	ORect(i32 left, i32 top, i32 width, i32 height) : left(left), top(top), width(width), height(height) {}
	ORect(const ORect& rect) : left(rect.left), top(rect.top), width(rect.width), height(rect.height) {}

public:
	int left = 0;
	int top = 0;
	int width = 0;
	int height = 0;
	
};
