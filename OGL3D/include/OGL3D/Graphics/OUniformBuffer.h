
#pragma 
#include <OGL3D/OPrerequisites.h>
#include <glad/glad.h>
#include <OGL3D/Graphics/OShaderProgram.h>
#include <OGL3D/Math/OVec3.h>
#include <OGL3D/Math/OMat4.h>
class World;
class OUniformBuffer
{
public:
	OUniformBuffer(const OUniformBufferDesc& desc);
	~OUniformBuffer();

	void setData(void* data);

	ui32 getId();
	void setLight(const OShaderProgramPtr& program, OVec3 lightPos);
	void setTextureMap(const OShaderProgramPtr& program);
	void setNormalMap(const OShaderProgramPtr& program);
	void setShadowMap(const OShaderProgramPtr& program, OVec3 lightPos, int width, int height);
	void setGUI(const OShaderProgramPtr& program);
	void renderGUI(int width, int height);
	void render(World* world, int width, int height);

private:
	GLuint m_textureID;
	GLuint m_normalID;
	GLuint m_shadowID;
	GLuint m_shadowFBO;
	GLuint m_GUI;
	GLuint m_GUI_VBO;

	ui32 m_id = 0;
	ui32 m_size = 0;

	ui32 m_shadowMapWidth = 2048;
	ui32 m_shadowMapHeight = 2048;
};
