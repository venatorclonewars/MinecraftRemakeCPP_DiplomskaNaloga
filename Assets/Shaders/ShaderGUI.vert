#version 410 core

layout (location = 0) in vec3 vertPos;  // Vertex position of the quad
layout (location = 1) in vec2 texCoord;  // Texture coordinates

out vec2 fragCoord;  


void main()
{
    
    //Scaling if needed
    vec3 pos = vertPos * 0.55;
    gl_Position = vec4(pos, 1.0);

    fragCoord = texCoord;
}
