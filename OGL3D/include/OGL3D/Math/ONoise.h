#pragma once
#include <OGL3D/Math/OVec2.h>
#include <OGL3D/Minecraft/WorldData.h>
#include <SimplexNoise.h>

class ONoise
{
public:
	
	static float get2DPerlin(OVec2 position, float offset, float scale) 
	{

		return SimplexNoise::noise((position.x + 0.1f) / CHUNK_WIDTH * scale + offset, (position.y + 0.1f) / CHUNK_WIDTH * scale + offset);

	}

    static bool get3DPerlin(OVec3 position, float offset, float scale, float threshold) 
    {

        // https://www.youtube.com/watch?v=Aga0TBJkchM Carpilot on YouTube

        float x = (position.x + offset + 0.1f) * scale;
        float y = (position.y + offset + 0.1f) * scale;
        float z = (position.z + offset + 0.1f) * scale;

        float AB = SimplexNoise::noise(x, y);
        float BC = SimplexNoise::noise(y, z);
        float AC = SimplexNoise::noise(x, z);
        float BA = SimplexNoise::noise(y, x);
        float CB = SimplexNoise::noise(z, y);
        float CA = SimplexNoise::noise(z, x);

        if ((AB + BC + AC + BA + CB + CA) / 6.0f > threshold)
            return true;

        else
            return false;

    }
};

