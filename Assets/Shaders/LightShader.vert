#version 410 core

uniform mat4 lightProjection;

layout(location = 0) in vec3 vertPos;

void main()
{
	gl_Position = lightProjection * (vec4(vertPos, 1.0));
}