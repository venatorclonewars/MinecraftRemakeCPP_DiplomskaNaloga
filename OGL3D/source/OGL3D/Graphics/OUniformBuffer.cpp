
#include <OGL3D/Graphics/OUniformBuffer.h>
#include <OGL3D/Stb/stb_image.h>
#include <OGL3D/Graphics/OShaderProgram.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <OGL3D/Minecraft/World.h>

OUniformBuffer::OUniformBuffer(const OUniformBufferDesc& desc)
{
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_UNIFORM_BUFFER, m_id);
    glBufferData(GL_UNIFORM_BUFFER, desc.size, nullptr, GL_DYNAMIC_DRAW);// GL_STATIC_DRAW
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    m_size = desc.size;

}

OUniformBuffer::~OUniformBuffer()
{
    glDeleteBuffers(1, &m_id);
}

void OUniformBuffer::setData(void* data)
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_id);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, m_size, data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

ui32 OUniformBuffer::getId()
{
    return m_id;
}

void OUniformBuffer::setLight(const OShaderProgramPtr& program, OVec3 lightPos)
{
    GLuint shaderProgram = program->getId();

    OVec3 lightColor(1.0f, 1.0f, 1.0f);
    float ambientStrength = 0.1f;
    float specularStrength = 0.5f;
    float shininess = 32.0f;

    glUniform3f(glGetUniformLocation(shaderProgram, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(glGetUniformLocation(shaderProgram, "lightColor"), lightColor.x, lightColor.y, lightColor.z);
    glUniform1f(glGetUniformLocation(shaderProgram, "ambientStrength"), ambientStrength);
    glUniform1f(glGetUniformLocation(shaderProgram, "specularStrength"), specularStrength);
    glUniform1f(glGetUniformLocation(shaderProgram, "shininess"), shininess);
}

void OUniformBuffer::setTextureMap(const OShaderProgramPtr& program)
{
    int textureWidth;
    int textureHeight;
    int numColCh;

    unsigned char* textureBytes = stbi_load("Assets/Textures/Blocks.png", &textureWidth, &textureHeight, &numColCh, 0);

    GLuint textureID;
    glGenTextures(1, &m_textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    GLenum format = (numColCh == 3) ? GL_RGB : GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, textureWidth, textureHeight, 0, format, GL_UNSIGNED_BYTE, textureBytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glUniform1i(glGetUniformLocation(program->getId(), "texture0"), 0);


    stbi_image_free(textureBytes);

}

void OUniformBuffer::setNormalMap(const OShaderProgramPtr& program)
{
    int textureWidth;
    int textureHeight;
    int numColCh;

    unsigned char* normalBytes = stbi_load("Assets/Textures/BlocksNormal2.jpg", &textureWidth, &textureHeight, &numColCh, 0);

    glGenTextures(1, &m_normalID);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_normalID);


    GLenum format = (numColCh == 3) ? GL_RGB : GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, textureWidth, textureHeight, 0, format, GL_UNSIGNED_BYTE, normalBytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Set normal map parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glUniform1i(glGetUniformLocation(program->getId(), "normal0"), 1);

    stbi_image_free(normalBytes);

}

void OUniformBuffer::setShadowMap(const OShaderProgramPtr& program, OVec3 lightPos, int width, int height)
{
    // Generate the framebuffer object for shadow mapping  
    glGenFramebuffers(1, &m_shadowFBO);

    // Generate the depth texture
    glGenTextures(1, &m_shadowID);
    glBindTexture(GL_TEXTURE_2D, m_shadowID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_shadowMapWidth, m_shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    f32 clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

    // Bind the framebuffer and attach the depth texture to it
    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowID, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glm::mat4 lightOrtho = glm::ortho(-300.0f, 300.0f, -300.0f, 300.0f, 1.0f, 200.0f);
    glm::mat4 lightView = glm::lookAt(1.0f * glm::vec3(-lightPos.x, lightPos.y, -lightPos.z), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightProjection = lightOrtho * lightView;
    glUniformMatrix4fv(glGetUniformLocation(program->getId(), "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_shadowID);
    glUniform1i(glGetUniformLocation(program->getId(), "shadowMap"), 2);

}

void OUniformBuffer::setGUI(const OShaderProgramPtr& program)
{
    int crosshairWidth;
    int crosshairHeight;
    int numColCh;

    unsigned char* crosshairBytes = stbi_load("Assets/Textures/CrosshairWhite.png", &crosshairWidth, &crosshairHeight, &numColCh, 0);

    glGenTextures(1, &m_GUI);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, m_GUI);

    GLenum format = (numColCh == 3) ? GL_RGB : GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, crosshairWidth, crosshairHeight, 0, format, GL_UNSIGNED_BYTE, crosshairBytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glUniform1i(glGetUniformLocation(program->getId(), "GUI0"), 3);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Enable standard transparency


    stbi_image_free(crosshairBytes);
}

void OUniformBuffer::renderGUI(int width, int height)
{
    float aspectRatio = (float)width / (float)height;
    
    // Adjust the quad’s vertices to respect the aspect ratio.
    f32 textureQuad[] =
    {
        // First triangle (Top-left, Bottom-left, Bottom-right)
        -0.05f,  0.05f * aspectRatio, 0.0f,  0.0f, 0.0f,  // Top-left
        -0.05f, -0.05f * aspectRatio, 0.0f,  0.0f, 1.0f,  // Bottom-left
         0.05f, -0.05f * aspectRatio, 0.0f,  1.0f, 1.0f,  // Bottom-right

         // Second triangle (Top-left, Bottom-right, Top-right)
         -0.05f,  0.05f * aspectRatio, 0.0f,  0.0f, 0.0f,  // Top-left
          0.05f, -0.05f * aspectRatio, 0.0f,  1.0f, 1.0f,  // Bottom-right
          0.05f,  0.05f * aspectRatio, 0.0f,  1.0f, 0.0f   // Top-right
    };


    ui32 quadVAO, quadVBO, quadEBO;

    // Generate and bind VAO
    glGenVertexArrays(1, &quadVAO);
    glBindVertexArray(quadVAO);

    // Generate and bind VBO
    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureQuad), textureQuad, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Bind texture
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, m_GUI);

    // Draw the quad
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
}


void OUniformBuffer::render(World* world, int width, int height)
{
    
    // first render to depth map
    glViewport(0, 0, m_shadowMapWidth, m_shadowMapHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    world->preRender();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // render scene as normal with shadow mapping (using depth map)
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    world->render();
    glBindTexture(GL_TEXTURE_2D, m_shadowID);

    world->renderGUI();
}
