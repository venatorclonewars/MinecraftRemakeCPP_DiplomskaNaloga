#pragma once
#include <string>
#include <vector>

class Lode;

class BiomeAttributes
{
public:
    std::string biomeName;
    int solidGroundHeight;
    int terrainHeight;
    float terrainScale;

    std::vector<Lode> lodes;

    BiomeAttributes(const std::string& biomeName, int solidHeight, int terrainHeight, float terrainScale, const std::vector<Lode>& lodes)
        : biomeName(biomeName), solidGroundHeight(solidHeight), terrainHeight(terrainHeight), terrainScale(terrainScale), lodes(lodes) {}
};

class Lode
{
public:
    std::string nodeName;
    int blockID;
    int minHeight;
    int maxHeight;
    float scale;
    float threshold;
    float noiseOffset;

    Lode(const std::string& nodeName, int blockID, int minHeight, int maxHeight, float scale, float threshold, float noiseOffset)
        : nodeName(nodeName), blockID(blockID), minHeight(minHeight), maxHeight(maxHeight), scale(scale), threshold(threshold), noiseOffset(noiseOffset) {}

};
