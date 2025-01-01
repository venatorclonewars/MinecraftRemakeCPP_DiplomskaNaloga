#pragma once
#include <OGL3D/OPrerequisites.h>
#include <OGL3D/Math/OVec4.h>
#include <OGL3D/Math/OVec3.h>
#include <OGL3D/Math/ORect.h>
#include <vector>
#include <glad/glad.h>

class OGraphicsEngine
{
public:
	OGraphicsEngine();
	~OGraphicsEngine();

public:
	OVertexArrayObjectPtr createVertexArrayObject(const OVertexBufferDesc& vbDesc);
	OVertexArrayObjectPtr createVertexArrayObject(const OVertexBufferDesc& vbDesc, const OIndexBufferDesc& ibDesc);
	OShaderProgramPtr createShaderProgram(const OShaderProgramDesc& desc);
	OUniformBufferPtr createUniformBuffer(const OUniformBufferDesc& desc);


public:
	void clear(const OVec4& color);
	void setFaceCulling(const OCullType& type);
	void setDepthCulling();
	void setWindingOrder(const OWindingOrder& order);
	void setViewport(const ORect& size);
	void setVertexArrayObject(const OVertexArrayObjectPtr& vao);
	void setUniformBuffer(const OUniformBufferPtr& buffer, ui32 slot);
	void setShaderProgram(const OShaderProgramPtr& program);
	void drawTriangles(const OTriangleType& triangleType, ui32 vertexCount, ui32 offset);
	void drawIndexedTriangles(const OTriangleType& triangleType, ui32 indicesCount);
	void setAntiAlliasing();
};

