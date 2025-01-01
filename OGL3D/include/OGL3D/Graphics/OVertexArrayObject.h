#pragma once
#include <OGL3D/OPrerequisites.h>
#include <vector>
#include <OGL3D/Minecraft/WorldData.h>

class OVertexArrayObject
{

public:
	OVertexArrayObject(const OVertexBufferDesc& vbDesc);
	OVertexArrayObject(const OVertexBufferDesc& vbDesc, const OIndexBufferDesc& ibDesc);
	~OVertexArrayObject();

	ui32 getId();

	ui32 getVertexBufferSize();
	ui32 getVertexSize();
	void updateVertexArrayObject(std::vector<Vertex> vertices, std::vector<ui32> indices);


private:
	ui32 m_vertexBufferId = 0;
	ui32 m_elementBufferId = 0;
	ui32 m_vertexArrayObjectId = 0;
	OVertexBufferDesc m_vertexBufferData;
};

