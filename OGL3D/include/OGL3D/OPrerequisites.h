#pragma once
#include <memory>
#include <sstream>
#include <iostream>
#include <stdexcept>

class OGraphicsEngine;
class OVertexArrayObject;
class OShaderProgram;
class OUniformBuffer;
class OWindow;

typedef std::shared_ptr<OGraphicsEngine> OGraphicsEnginePtr;
typedef std::shared_ptr<OVertexArrayObject> OVertexArrayObjectPtr;
typedef std::shared_ptr<OShaderProgram> OShaderProgramPtr;
typedef std::shared_ptr<OUniformBuffer> OUniformBufferPtr;
typedef std::shared_ptr<OWindow> ODisplayPtr;

typedef float f32;
typedef int i32;
typedef unsigned int ui32;

struct OVertexAttribute
{
	ui32 numElements = 0;
};

struct OVertexBufferDesc
{
	const void* vertices = nullptr;
	ui32 vertexSize = 0;
	ui32 listSize = 0;

	OVertexAttribute* attribs = nullptr;
	ui32 attributesListSize = 0;
};

struct OIndexBufferDesc
{
	const void* indices = nullptr;
	ui32 listSize = 0;
};

struct OShaderProgramDesc
{
	const wchar_t* vertexShaderFilePath;
	const wchar_t* fragmentShaderFilePath;
};

struct OUniformBufferDesc
{
	ui32 size = 0;
};

enum class OShaderType
{
	VertexShader = 0,
	FragmentShader
};

enum class OTriangleType
{
	TriangleList = 0,
	TriangleStrip
};

enum class OCullType
{
	BackFace = 0,
	FrontFace,
	Both
};

enum class OWindingOrder
{
	ClockWise = 0,
	CounterClockWise
};

//f32 textureQuad[] = 
//{
//	// Positions         // Texture Coords
//	-0.05f,  0.05f, 0.0f,  0.0f, 1.0f,  // Top-left
//	-0.05f, -0.05f, 0.0f,  0.0f, 0.0f,  // Bottom-left
//	 0.05f, -0.05f, 0.0f,  1.0f, 0.0f,  // Bottom-right
//	 0.05f,  0.05f, 0.0f,  1.0f, 1.0f   // Top-right
//};


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


#define OGL3D_ERROR(message)\
{\
std::stringstream m;\
m << "OGL3D Error: " << message << std::endl;\
throw std::runtime_error(m.str());\
}

#define OGL3D_WARNING(message)\
std::wclog << "OGL3D Warning: " << message << std::endl;


#define OGL3D_INFO(message)\
std::wclog << "OGL3D Info: " << message << std::endl;