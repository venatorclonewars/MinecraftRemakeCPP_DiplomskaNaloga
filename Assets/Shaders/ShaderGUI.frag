#version 410 core

in vec2 fragCoord;  // Texture coordinates from the vertex shader
out vec4 FragColor;  // Final output color

uniform sampler2D GUI0;  // The GUI texture (crosshair)

void main()
{
    FragColor = texture(GUI0, fragCoord);
}
