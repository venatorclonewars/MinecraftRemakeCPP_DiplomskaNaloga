#pragma once
#include <OGL3D/Minecraft/WorldData.h>
#include <OGL3D/OPrerequisites.h>
#include <OGL3D/Math/OVec3.h>
#include <OGL3D/Math/OVec2.h>
#include <vector>

class World;

class Chunk
{
public:

	Chunk(World* world, OVec3 chunkPos);
	~Chunk();

	ui32 getVoxelFromGlobalVector3(OVec3 pos);
	void editChunk(OVec3 pos, ui32 newID);
	void render();
	bool isEditable();

public:
	ui32 voxelMap[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_WIDTH]; 
	bool isVoxMapPopulated = false;
	bool hasMesh;
	std::atomic<bool> threadLocked = false; 
	OVec3 chunkPos;

public:
	OGraphicsEnginePtr graphicsEngine;
	OVertexArrayObjectPtr polygonVAO;
	OShaderProgramPtr shader;
	OUniformBufferPtr uniform;

private:
	void populateVoxelMap();
	void addVoxelDataToChunk(OVec3 pos);
	bool checkVoxel(OVec3 pos);
	bool isVoxelInChunk(int x, int y, int z);
	void updateSurroundingChunks(int x, int y, int z);
	
public:
	void threadMeshData();
	void createMeshData();
	void updateMeshData();
	void addTexture(ui32 textureID);
	void updateMesh();
	void createMesh();
	

private:
	World* m_world;

	std::vector<ui32> m_indices;
	std::vector<Vertex> m_vertices;
	std::vector<unsigned char*> m_textures;
	
	ui32 m_vertexIndex = 0;
	
	int m_textureWidth;
	int m_textureHeight;
	int m_numColCh;
	unsigned char* m_textureData;
	int m_textureId;

	OVec2 m_xBtm_yBtm;
	OVec2 m_xTop_yBtm;
	OVec2 m_xBtm_yTop;
	OVec2 m_xTop_yTop;

};

