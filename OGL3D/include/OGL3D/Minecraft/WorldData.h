#pragma once
#include <OGL3D/Math/OVec3.h>
#include <OGL3D/Math/OVec2.h>
#include <OGL3D/Minecraft/BlockType.h>
#include <OGL3D/Minecraft/BiomeAttributes.h>

struct Vertex
{
    OVec3 position;
    OVec2 texcoord;
    OVec3 normal;
};

static const ui32 CHUNK_HEIGHT = 20;
static const ui32 CHUNK_WIDTH = 16;
static const ui32 WORLD_SIZE_IN_CHUNKS = 10;
static const ui32 WORLD_SIZE_IN_VOXELS = WORLD_SIZE_IN_CHUNKS * CHUNK_WIDTH;
static const int TEXTURE_ATLAS_SIZE_IN_BLOCKS = 4;
static const float NORMALIZED_BLOCK_TEXTURE_SIZE = 1.0f / (float)TEXTURE_ATLAS_SIZE_IN_BLOCKS;

static const int VIEW_DISTANCE_IN_CHUNKS = 3;

static const OVec3 voxelVerts[8] =
{
    // front face
    OVec3(0.0f, 0.0f, 0.0f),  
    OVec3(0.0f, 1.0f, 0.0f),  
    OVec3(1.0f, 1.0f, 0.0f),  
    OVec3(1.0f, 0.0f, 0.0f),  

    // back face
    OVec3(1.0f, 0.0f, 1.0f),  
    OVec3(1.0f, 1.0f, 1.0f),  
    OVec3(0.0f, 1.0f, 1.0f),  
    OVec3(0.0f, 0.0f, 1.0f) 
};


static const ui32 voxelTris[6][4] =
{ 
    { 0, 1, 2, 3 }, // Front face
    
    { 4, 5, 6, 7 }, // Back face
   
    { 1, 6, 5, 2 }, // Top face
   
    { 7, 0, 3, 4 },  // Bottom face
 
    { 3, 2, 5, 4 }, // Right face

    { 7, 6, 1, 0 }  // Left face
};


static const BlockType blockTypes[7] = 
{
    {0, false, 0, 0, 0, 0, 0, 0}, // Air
    {1, true, 2, 2, 7, 1, 2, 2},   // Grass
    {2, true, 1, 1, 1, 1, 1, 1},   // Dirt
    {3, true, 8, 8, 8, 8, 8, 8},   // Stone #2
    {4, true, 9, 9, 9, 9, 9, 9},   // Bedrock
    {5, true, 3, 3, 3, 3, 3, 3},   // Cole
    {6, true, 0, 0, 0, 0, 0, 0}   // Stone #1
    
};

static const OVec3 faceChecks[6] =
{
    OVec3(0.0f, 0.0f, -1.0f), // Front face
    OVec3(0.0f, 0.0f, 1.0f),  // Back face
    OVec3(0.0f, 1.0f, 0.0f),  // Top face
    OVec3(0.0f, -1.0f, 0.0f), // Bottom face
    OVec3(1.0f, 0.0f, 0.0f),  // Right face
    OVec3(-1.0f, 0.0f, 0.0f)  // Left face
};

static const BiomeAttributes biome =
{
    BiomeAttributes(
        std::string("Default"),                 // Biome name
        5,                                      // Solid ground height
        8,                                      // Terrain height
        0.25f,                                  // Terrain scale
        std::vector<Lode>({
            Lode("Cole", 5, 1, 255, 0.8f, 0.2f, 0.0f),
            Lode("Stone #1",6, 1, 255, 0.9f, 0.1f, 0.0f)
        })
    )
};