#include <OGL3D/Minecraft/Chunk.h>
#include <OGL3D/Graphics/OGraphicsEngine.h>
#include <OGL3D/Graphics/OUniformBuffer.h>
#include <OGL3D/Graphics/OVertexArrayObject.h>
#include <OGL3D/Math/OMat4.h>
#include <OGL3D/Math/OVec4.h>
#include <OGL3D/Minecraft/World.h>
#include <mutex>
#include <thread>
#include <memory>

Chunk::Chunk(World* world, OVec3 _chunkPos)
{
	m_world = world;
	graphicsEngine = world->graphicsEngine;
	uniform = world->uniform;
	shader = world->shader;
	chunkPos = _chunkPos;
	hasMesh = false;

	std::thread myThread(&Chunk::populateVoxelMap, this);
	myThread.detach();
	
	
}

Chunk::~Chunk()
{
}

void Chunk::populateVoxelMap()
{
	for (int x = 0; x < CHUNK_WIDTH; x++) 
	{
		for (int y = 0; y < CHUNK_HEIGHT; y++)
		{
			for (int z = 0; z < CHUNK_WIDTH; z++)
			{
				// Fill voxel map
				voxelMap[x][y][z] = m_world->getVoxel(OVec3(x, y, z) + chunkPos);
	
			}
		}
	}
	threadMeshData();
	isVoxMapPopulated = true;
}

void Chunk::editChunk(OVec3 pos, ui32 newID)
{
	int xCheck = (int)pos.x;
	int yCheck = (int)pos.y;
	int zCheck = (int)pos.z;

	xCheck -= (int)chunkPos.x;
	zCheck -= (int)chunkPos.z;

	if (newID != 0 && voxelMap[xCheck][yCheck][zCheck] != 0)
		return;

	voxelMap[xCheck][yCheck][zCheck] = newID;

	updateSurroundingChunks(xCheck, yCheck, zCheck);

	updateMeshData();

}

void Chunk::updateSurroundingChunks(int x, int y, int z) {

	OVec3 thisVoxel = OVec3(x, y, z);

	for (int p = 0; p < 6; p++) {

		OVec3 currentVoxel = thisVoxel + faceChecks[p];

		if (!isVoxelInChunk((int)currentVoxel.x, (int)currentVoxel.y, (int)currentVoxel.z)) {

			Chunk* chunk = m_world->getChunkFromVector3(currentVoxel + chunkPos);
			chunk->updateMeshData();
		}

	}

}

void Chunk::threadMeshData()
{
	
	std::thread myThread(&Chunk::createMeshData, this);
	myThread.detach();

}

void Chunk::createMeshData()
{
	threadLocked = true;
	
	m_vertices.clear();
	m_indices.clear();
	m_vertexIndex = 0;

	for (int y = 0; y < CHUNK_HEIGHT; y++)
	{
		for (int x = 0; x < CHUNK_WIDTH; x++)
		{
			for (int z = 0; z < CHUNK_WIDTH; z++)
			{
				if (blockTypes[voxelMap[x][y][z]].isSolid)
					addVoxelDataToChunk(OVec3(x, y, z));
			}
		}
	}

	std::lock_guard<std::mutex> lock(m_world->mtx);
	m_world->chunksToDraw.push(this);
	
	threadLocked = false;
}

void Chunk::updateMeshData()
{
	hasMesh = false;

	m_vertices.clear();
	m_indices.clear();
	m_vertexIndex = 0;

	for (int y = 0; y < CHUNK_HEIGHT; y++)
	{
		for (int x = 0; x < CHUNK_WIDTH; x++)
		{
			for (int z = 0; z < CHUNK_WIDTH; z++)
			{
				if (blockTypes[voxelMap[x][y][z]].isSolid)
					addVoxelDataToChunk(OVec3(x, y, z));
			}
		}
	}

	updateMesh();

	hasMesh = true;

}

void Chunk::addTexture(ui32 textureID)
{
	float y = textureID / TEXTURE_ATLAS_SIZE_IN_BLOCKS;
	float x = textureID % TEXTURE_ATLAS_SIZE_IN_BLOCKS;

	x *= NORMALIZED_BLOCK_TEXTURE_SIZE;
	y *= NORMALIZED_BLOCK_TEXTURE_SIZE;

	m_xBtm_yBtm = OVec2(x, y);
	m_xTop_yBtm = OVec2(x + NORMALIZED_BLOCK_TEXTURE_SIZE, y);
	m_xBtm_yTop = OVec2(x, y + NORMALIZED_BLOCK_TEXTURE_SIZE);
	m_xTop_yTop = OVec2(x + NORMALIZED_BLOCK_TEXTURE_SIZE, y + NORMALIZED_BLOCK_TEXTURE_SIZE);
}

void Chunk::addVoxelDataToChunk(OVec3 pos)
{
	for (int p = 0; p < 6; ++p) {
		
		if (!checkVoxel(pos + faceChecks[p]))
		{
		
			ui32 blockID = voxelMap[(int)pos.x][(int)pos.y][(int)pos.z];
			BlockType block = blockTypes[blockID];
			int textureId = block.getTextureID(p);
			addTexture(textureId);

			// Add vertices an textures
			m_vertices.push_back({ chunkPos + pos + voxelVerts[voxelTris[p][1]], m_xBtm_yBtm, faceChecks[p] }); // Top-left becomes Bottom-left
			m_vertices.push_back({ chunkPos + pos + voxelVerts[voxelTris[p][2]], m_xTop_yBtm, faceChecks[p] });  // Top-right becomes Bottom-right
			m_vertices.push_back({ chunkPos + pos + voxelVerts[voxelTris[p][3]], m_xTop_yTop, faceChecks[p] });   // Bottom-right becomes Top-right
			m_vertices.push_back({ chunkPos + pos + voxelVerts[voxelTris[p][0]], m_xBtm_yTop, faceChecks[p] });  // Bottom-left becomes Top-left

			// Add triangles
			m_indices.push_back(m_vertexIndex);
			m_indices.push_back(m_vertexIndex + 1);
			m_indices.push_back(m_vertexIndex + 2);
			m_indices.push_back(m_vertexIndex + 2);
			m_indices.push_back(m_vertexIndex + 3);
			m_indices.push_back(m_vertexIndex);

			m_vertexIndex += 4;


			/*int yPos = (int)pos.y + 1;
			bool inShade = false;

			while (yPos < CHUNK_HEIGHT)
			{
				if (voxelMap[(int)pos.x][yPos][(int)pos.z] != 0)
				{
					inShade = true;
					break;
				}
					
				yPos++;
			}*/
		}      
    }

}

bool Chunk::checkVoxel(OVec3 pos)
{
	int xCheck = (int)pos.x;
	int yCheck = (int)pos.y;
	int zCheck = (int)pos.z;

	// If position is outside of this chunk...
	if (!isVoxelInChunk(xCheck, yCheck, zCheck))
		return m_world->checkForVoxel(pos + chunkPos);

	return blockTypes[voxelMap[xCheck][yCheck][zCheck]].isSolid;
}

ui32 Chunk::getVoxelFromGlobalVector3(OVec3 pos) 
{
	int xCheck = (int)pos.x;
	int yCheck = (int)pos.y;
	int zCheck = (int)pos.z;

	xCheck -= (int)chunkPos.x;
	zCheck -= (int)chunkPos.z;

	return voxelMap[xCheck][yCheck][zCheck];
}

bool Chunk::isVoxelInChunk(int x, int y, int z) 
{

	if (x < 0 || x > CHUNK_WIDTH - 1 || y < 0 || y > CHUNK_HEIGHT - 1 || z < 0 || z > CHUNK_WIDTH - 1)
		return false;
	else return true;

}

void Chunk::updateMesh()
{
	polygonVAO->updateVertexArrayObject(m_vertices, m_indices);
}

void Chunk::createMesh()
{
	
	OVertexAttribute vertAttribs[] = 
	{
		sizeof(OVec3) / sizeof(f32), //position
		sizeof(OVec2) / sizeof(f32), //texcoord
		sizeof(OVec3) / sizeof(f32)	 //normal
	};

	polygonVAO = graphicsEngine->createVertexArrayObject(
		{
			m_vertices.data(),               // Pass raw pointer to vertex data
			sizeof(Vertex),                 // Size of each vertex
			(ui32)(m_vertices.size()),  // Number of vertices

			vertAttribs,                     // Vertex attributes list
			(sizeof(vertAttribs) / sizeof(OVertexAttribute))  // Number of attributes
		},
		{
			m_indices.data(),                // Pass raw pointer to index data
			(ui32)(m_indices.size() * sizeof(ui32))  // Size of the index buffer
		}
	);

	hasMesh = true;
}

void Chunk::render()
{
	graphicsEngine->setVertexArrayObject(polygonVAO);
	graphicsEngine->drawIndexedTriangles(OTriangleType::TriangleList, m_indices.size());

}

bool Chunk::isEditable()
{
	if (!isVoxMapPopulated || threadLocked)
		return false;

	return true;
}




