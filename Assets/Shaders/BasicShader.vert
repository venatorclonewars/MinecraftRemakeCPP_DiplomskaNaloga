#version 410 core 

layout (row_major) uniform UniformData
{
    mat4 world;
    mat4 projection;
    vec3 camPos;
};
uniform mat4 lightProjection; 
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 normal;

layout(location = 0) out vec2 fragTexCoord;    
layout(location = 1) out float camToDistance;    
layout(location = 2) out vec3 fragCamPos;    
layout(location = 3) out vec3 fragPos;    
layout(location = 4) out vec3 fragNormal;    

void main()
{
    vec4 pos = vec4(vertPos, 1.0) * world; 
    pos = pos * projection; 
    
    gl_Position = pos;

    fragTexCoord = texcoord; 
    camToDistance = length(camPos - vertPos);
    fragCamPos = camPos; 
    
    fragPos = vertPos;
    fragNormal = normal;
       
}
