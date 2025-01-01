#pragma once
#include <OGL3D/OPrerequisites.h>

struct OVec3
{
public:
	OVec3()
	{

	}
	OVec3(f32 x, f32 y, f32 z) : x(x), y(y), z(z)
	{

	}
	OVec3 operator+(OVec3 v)
	{
		return OVec3(x + v.x, y + v.y, z + v.z);
	}

	OVec3 operator-(OVec3 v)
	{
		return OVec3(x - v.x, y - v.y, z - v.z);
	}

	OVec3 operator*(float scalar) 
	{
		return OVec3(x * scalar, y * scalar, z * scalar);
	}

	OVec3 operator+(float scalar)
	{
		return OVec3(x + scalar, y + scalar, z + scalar);
	}

	OVec3 operator*(OVec3 v)
	{
		return OVec3(x * v.x, y * v.y, z * v.z);
	}

	bool operator==(OVec3 v)
	{
		if (x != v.x && y != v.y && z != v.z)
			return false;

		return true;
	}

	bool operator!=(OVec3 v)
	{
		if (x != v.x || y != v.y || z != v.z)
			return true;

		return false;
	}

	f32 dot(OVec3 v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	f32 magnitude()
	{
		return std::sqrt(x * x + y * y + z * z);
	}

	// Function to normalize the vector (make it unit length)
	void normalize()
	{
		f32 mag = magnitude();
		if (mag > 0.0f)  // To avoid division by zero
		{
			x /= mag;
			y /= mag;
			z /= mag;
		}
	}

public:
	f32 x = 0;
	f32 y = 0;
	f32 z = 0;
};

