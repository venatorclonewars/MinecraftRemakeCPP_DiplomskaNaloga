
#include <OGL3D/Minecraft/World.h>
#include <OGL3D/Graphics/OGraphicsEngine.h>
#include <OGL3D/Graphics/OUniformBuffer.h>
#include <OGL3D/Graphics/OVertexArrayObject.h>
#include <OGL3D/Graphics/OShaderProgram.h>
#include <OGL3D/Minecraft/Chunk.h>
#include <OGL3D/Math/OMat4.h>
#include <OGL3D/Math/OVec4.h>
#include <OGL3D/Math/OVec3.h>
#include <OGL3D/Math/OVec2.h>
#include <OGL3D/Window/OWindow.h>
#include <Windows.h>
#include <OGL3D/Minecraft/WorldData.h>


struct UniformData
{
	OMat4 world;
	OMat4 projection;
	OVec3 camPos;
};

World::World()
{

	graphicsEngine = std::make_shared<OGraphicsEngine>();
	display = std::make_shared<OWindow>();
	display->makeCurrentContext();

	graphicsEngine->setViewport(display->getInnerSize());


	uniform = graphicsEngine->createUniformBuffer({
		sizeof(UniformData)
		});

	shader = graphicsEngine->createShaderProgram(
		{
			L"Assets/Shaders/BasicShader.vert",
			L"Assets/Shaders/BasicShader.frag"
		});

	shader->setUniformBufferSlot("UniformData", 0);
	graphicsEngine->setShaderProgram(shader);

	lightPos = OVec3(20.0f, 80.0f, 20.0f);
	displaySize = display->getInnerSize();
	uniform->setLight(shader, lightPos);
	uniform->setTextureMap(shader);
	uniform->setNormalMap(shader);
	uniform->setShadowMap(shader, lightPos, displaySize.width, displaySize.height);

	shadowShader = graphicsEngine->createShaderProgram(
		{
			L"Assets/Shaders/LightShader.vert",
			L"Assets/Shaders/LightShader.frag"
		});

	shadowShader->setUniformBufferSlot("UniformData", 0);


	graphicsEngine->setShaderProgram(shadowShader);
	uniform->setShadowMap(shadowShader, lightPos, displaySize.width, displaySize.height);

	shaderGUI = graphicsEngine->createShaderProgram(
		{
			L"Assets/Shaders/ShaderGUI.vert",
			L"Assets/Shaders/ShaderGUI.frag"
		});

	graphicsEngine->setShaderProgram(shaderGUI);
	uniform->setGUI(shaderGUI);

	graphicsEngine->setUniformBuffer(uniform, 0);
	m_chunks.resize(WORLD_SIZE_IN_CHUNKS, std::vector<std::shared_ptr <Chunk>>(WORLD_SIZE_IN_CHUNKS));
}

World::~World()
{

}

void World::onCreate()
{
	generateWorld();
	player = new Player(this);



}

void World::onUpdate()
{

	std::lock_guard<std::mutex> lock(mtx);
	if (!chunksToDraw.empty())
	{
		if (chunksToDraw.front()->isEditable())
		{
			Chunk* chunk = chunksToDraw.front();
			chunksToDraw.pop();
			chunk->createMesh();
		}
	}

	player->onUpdate();
	setUniformMatrices();
	checkViewDistance();

	uniform->render(this, displaySize.width, displaySize.height);
}

void World::onQuit()
{
}

void World::generateWorld()
{
	m_renderPointX = WORLD_SIZE_IN_CHUNKS / 2;
	m_renderPointZ = WORLD_SIZE_IN_CHUNKS / 2;

	for (int x = (WORLD_SIZE_IN_CHUNKS / 2) - VIEW_DISTANCE_IN_CHUNKS; x < (WORLD_SIZE_IN_CHUNKS / 2) + VIEW_DISTANCE_IN_CHUNKS; x++)
	{
		for (int z = (WORLD_SIZE_IN_CHUNKS / 2) - VIEW_DISTANCE_IN_CHUNKS; z < (WORLD_SIZE_IN_CHUNKS / 2) + VIEW_DISTANCE_IN_CHUNKS; z++)
		{
			m_chunks[x][z] = std::make_shared<Chunk>(this, OVec3(x * CHUNK_WIDTH, 0, z * CHUNK_WIDTH));
		}
	}

}

bool World::checkForVoxel(OVec3 pos)
{
	int x = pos.x / CHUNK_WIDTH;
	int y = pos.y;
	int z = pos.z / CHUNK_WIDTH;

	if (!isVoxelInWorld(pos) || !isChunkInWorld(OVec3(x, y, z)))
		return false;

	if (m_chunks[x][z] != nullptr && m_chunks[x][z]->isEditable())
		return blockTypes[m_chunks[x][z]->getVoxelFromGlobalVector3(pos)].isSolid;

	return blockTypes[getVoxel(pos)].isSolid;

}

bool World::isVoxelInWorld(OVec3 pos)
{
	if (pos.x >= 0 && pos.x < WORLD_SIZE_IN_VOXELS && pos.y >= 0 && pos.y < CHUNK_HEIGHT && pos.z >= 0 && pos.z < WORLD_SIZE_IN_VOXELS)
		return true;
	else
		return false;
}

bool World::isChunkInWorld(OVec3 pos)
{
	if (pos.x >= 0 && pos.x < WORLD_SIZE_IN_CHUNKS && pos.y >= 0 && pos.y < CHUNK_HEIGHT && pos.z >= 0 && pos.z < WORLD_SIZE_IN_CHUNKS)
		return true;
	else
		return false;
}

ui32 World::getVoxel(OVec3 pos)
{
	int yPos = (int)pos.y;

	/* IMMUTABLE PASS */

	// If outside world, return air.
	if (!isVoxelInWorld(pos))
		return 0;

	// If bottom block of chunk, return bedrock.
	if (yPos == 0)
		return 4;

	/* BASIC TERRAIN PASS */
	int terrainHeight = (int)(2.0f * noise.get2DPerlin(OVec2(pos.x, pos.z), 0, 0.2f)) + 8.0f;

	ui32 voxelValue = 0;

	if (yPos == terrainHeight)
		voxelValue = 1;

	else if (yPos < terrainHeight && yPos > terrainHeight - 4)
		voxelValue = 2;

	else if (yPos > terrainHeight)
		return 0;

	else
		voxelValue = 6;

	/* SECOND PASS */

	if (voxelValue == 2)
	{
		for (const Lode& lode : biome.lodes)
		{
			if (yPos > lode.minHeight && yPos < lode.maxHeight)
				if (noise.get3DPerlin(pos, lode.noiseOffset, lode.scale, lode.threshold))
					voxelValue = lode.blockID;
		}

	}

	return voxelValue;
}

void World::checkViewDistance()
{
	OVec3 playerPos = player->_worldCam.getTranslation();
	int chunkX = playerPos.x / CHUNK_WIDTH;
	int chunkZ = playerPos.z / CHUNK_WIDTH;

	OVec3 coord = OVec3(chunkX, 0, chunkZ);

	int startX = coord.x - VIEW_DISTANCE_IN_CHUNKS;
	int endX = coord.x + VIEW_DISTANCE_IN_CHUNKS;
	int startZ = coord.z - VIEW_DISTANCE_IN_CHUNKS;
	int endZ = coord.z + VIEW_DISTANCE_IN_CHUNKS;
	Chunk* chunk = nullptr;

	for (int x = startX; x <= endX; x++)
	{
		for (int z = startZ; z <= endZ; z++)
		{
			OVec3 chunkCoord = OVec3(x, 0, z);

			if (!isChunkInWorld(chunkCoord))
				continue;

			//chunk = m_chunks[x][z];

			if (!m_chunks[x][z])
				m_chunks[x][z] = std::make_shared<Chunk>(this, OVec3(x * CHUNK_WIDTH, 0, z * CHUNK_WIDTH));


			m_renderPointX = coord.x;
			m_renderPointZ = coord.z;


		}
	}

}

Chunk* World::getChunkFromVector3(OVec3 pos) {

	int x = (int)(pos.x / CHUNK_WIDTH);
	int z = (int)(pos.z / CHUNK_WIDTH);

	return m_chunks[x][z].get();

}

void World::setUniformMatrices()
{

	UniformData data = { player->worldCam, player->projection, player->_worldCam.getTranslation() };
	uniform->setData(&data);
}

void World::renderGUI()
{
	graphicsEngine->setShaderProgram(shaderGUI);
	uniform->renderGUI(displaySize.width, displaySize.height);
}

void World::preRender()
{
	graphicsEngine->clear(OVec4(0.5f, 0.7f, 1.0f, 1.0f));

	//graphicsEngine->setFaceCulling(OCullType::BackFace);
	graphicsEngine->setDepthCulling();
	graphicsEngine->setWindingOrder(OWindingOrder::ClockWise);

	graphicsEngine->setUniformBuffer(uniform, 0);
	graphicsEngine->setShaderProgram(shadowShader);

	for (int x = m_renderPointX - VIEW_DISTANCE_IN_CHUNKS; x < m_renderPointX + VIEW_DISTANCE_IN_CHUNKS; x++)
	{
		for (int z = m_renderPointZ - VIEW_DISTANCE_IN_CHUNKS; z < m_renderPointZ + VIEW_DISTANCE_IN_CHUNKS; z++)
		{
			if (isChunkInWorld(OVec3(x, 0, z)) && m_chunks[x][z] && m_chunks[x][z]->hasMesh)
			{

				m_chunks[x][z]->render();

			}

		}

	}

	display->present(false);
}

void World::render()
{

	graphicsEngine->setUniformBuffer(uniform, 0);
	graphicsEngine->setShaderProgram(shader);

	for (int x = m_renderPointX - VIEW_DISTANCE_IN_CHUNKS; x < m_renderPointX + VIEW_DISTANCE_IN_CHUNKS; x++)
	{
		for (int z = m_renderPointZ - VIEW_DISTANCE_IN_CHUNKS; z < m_renderPointZ + VIEW_DISTANCE_IN_CHUNKS; z++)
		{
			if (isChunkInWorld(OVec3(x, 0, z)) && m_chunks[x][z] && m_chunks[x][z]->hasMesh)
			{

				m_chunks[x][z]->render();

			}

		}

	}

}
