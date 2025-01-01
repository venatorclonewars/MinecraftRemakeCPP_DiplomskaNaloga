
#pragma once
#include <memory>
#include <OGL3D/OPrerequisites.h>
#include <chrono>
#include <OGL3D/Minecraft/Chunk.h>
#include <OGL3D/Minecraft/Player.h>
#include <OGL3D/InputSystem/OInputListener.h>
#include <OGL3D/Math/OMat4.h>
#include <OGL3D/Math/ORect.h>
#include <unordered_map>
#include <OGL3D/Math/ONoise.h>
#include <queue>
#include <mutex>

class OWindow;
class Player;

class World
{
public:
	World();
	~World();

	void generateWorld();
	bool checkForVoxel(OVec3 pos);
	bool isVoxelInWorld(OVec3 pos);
	bool isChunkInWorld(OVec3 chunkPos);
	ui32 getVoxel(OVec3 pos);
	Chunk* getChunkFromVector3(OVec3 chunkPos);;
	void checkViewDistance();
	void createChunks();

public:
	virtual void onCreate();
	virtual void onUpdate();
	virtual void onQuit();
	void renderGUI();
	void preRender();
	void render();
	void setUniformMatrices();

public:
	ONoise noise;
	OGraphicsEnginePtr graphicsEngine;
	OVertexArrayObjectPtr polygonVAO;
	OShaderProgramPtr shader;
	OShaderProgramPtr shadowShader;
	OShaderProgramPtr shaderGUI;
	OUniformBufferPtr uniform;
	ODisplayPtr display;

	Player* player = nullptr;
	std::queue<Chunk*> chunksToDraw;
	std::mutex mtx;

	OVec3 lightPos;
	ORect displaySize;
private:
	//Chunk* m_chunks[WORLD_SIZE_IN_CHUNKS][WORLD_SIZE_IN_CHUNKS] = { nullptr };
	std::vector<std::vector<std::shared_ptr <Chunk>>> m_chunks;
	int m_renderPointX;
	int m_renderPointZ;


};
