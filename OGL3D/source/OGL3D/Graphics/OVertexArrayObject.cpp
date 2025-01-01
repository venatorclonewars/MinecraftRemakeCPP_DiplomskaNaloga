#include <OGL3D/Graphics/OVertexArrayObject.h>
#include <glad/glad.h>
#include <OGL3D/Minecraft/WorldData.h>

OVertexArrayObject::OVertexArrayObject(const OVertexBufferDesc& vbDesc)
{
	if (!vbDesc.listSize) OGL3D_ERROR("OVertexArrayObject | listSize is NULL");
	if (!vbDesc.vertexSize) OGL3D_ERROR("OVertexArrayObject | vertexSize is NULL");
	if (!vbDesc.vertices) OGL3D_ERROR("OVertexArrayObject | verticesList is NULL");

	glGenVertexArrays(1, &m_vertexArrayObjectId);
	glBindVertexArray(m_vertexArrayObjectId);

	glGenBuffers(1, &m_vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, vbDesc.vertexSize * vbDesc.listSize, vbDesc.vertices, GL_STATIC_DRAW);
	for (ui32 i = 0, offset = 0; i < vbDesc.attributesListSize; i++)
	{
		glVertexAttribPointer(
			i,
			vbDesc.attribs[i].numElements,
			GL_FLOAT,
			GL_FALSE,
			vbDesc.vertexSize,
			(void*)(offset)  // Use cumulative offset
		);
		glEnableVertexAttribArray(i);

		offset += vbDesc.attribs[i].numElements * sizeof(f32);  // Update cumulative offset
	}


	
	glBindVertexArray(0);

	m_vertexBufferData = vbDesc;
}

OVertexArrayObject::OVertexArrayObject(const OVertexBufferDesc& vbDesc, const OIndexBufferDesc& ibDesc):OVertexArrayObject(vbDesc)
{
	if (!ibDesc.listSize) OGL3D_ERROR("OVertexArrayObject | listSize is NULL");
	if (!ibDesc.indices) OGL3D_ERROR("OVertexArrayObject | indicesList is NULL");
	
	glBindVertexArray(m_vertexArrayObjectId);

	glGenBuffers(1, &m_elementBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibDesc.listSize, ibDesc.indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	m_vertexBufferData = vbDesc;
}

OVertexArrayObject::~OVertexArrayObject()
{
	glDeleteBuffers(1, &m_elementBufferId);
	glDeleteBuffers(1, &m_vertexBufferId);
	glDeleteVertexArrays(1, &m_vertexArrayObjectId);
}

ui32 OVertexArrayObject::getId()
{
	return m_vertexArrayObjectId;
}

ui32 OVertexArrayObject::getVertexBufferSize()
{
	return m_vertexBufferData.listSize;
}

ui32 OVertexArrayObject::getVertexSize()
{
	return m_vertexBufferData.vertexSize;
}

void OVertexArrayObject::updateVertexArrayObject(std::vector<Vertex> vertices, std::vector<ui32> indices) 
{
	glBindVertexArray(m_vertexArrayObjectId);

	// Update vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), nullptr, GL_DYNAMIC_DRAW);  // orphan buffer
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * vertices.size(), vertices.data());  // update buffer data

	// Update index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ui32) * indices.size(), nullptr, GL_DYNAMIC_DRAW);  // orphan buffer
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(ui32) * indices.size(), indices.data());  // update buffer data

	glBindVertexArray(0);
}

