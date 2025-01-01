#pragma once
#include <OGL3D/OPrerequisites.h>

struct OVec4
{
public:
	OVec4() 
	{

	}
	OVec4(f32 x, f32 y, f32 z, f32 w) : x(x), y(y), z(z), w(w)
	{

	}

	void cross(OVec4& v1, OVec4& v2, OVec4& v3)
	{
		this->x = v1.y * (v2.z * v3.w - v3.z * v2.w) - v1.z * (v2.y * v3.w - v3.y * v2.w) + v1.w * (v2.y * v3.z - v2.z * v3.y);
		this->y = -(v1.x * (v2.z * v3.w - v3.z * v2.w) - v1.z * (v2.x * v3.w - v3.x * v2.w) + v1.w * (v2.x * v3.z - v3.x * v2.z));
		this->z = v1.x * (v2.y * v3.w - v3.y * v2.w) - v1.y * (v2.x * v3.w - v3.x * v2.w) + v1.w * (v2.x * v3.y - v3.x * v2.y);
		this->w = -(v1.x * (v2.y * v3.z - v3.y * v2.z) - v1.y * (v2.x * v3.z - v3.x * v2.z) + v1.z * (v2.x * v3.y - v3.x * v2.y));
	}


public:
	f32 x = 0;
	f32 y = 0;
	f32 z = 0;
	f32 w = 0;
};

