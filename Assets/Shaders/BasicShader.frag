#version 410 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BloomColor;

in vec2 fragTexCoord; 
in float camToDistance;
in vec3 fragCamPos;
in vec3 fragPos; 
in vec3 fragNormal;

// Textures
uniform sampler2D texture0;
uniform sampler2D normal0;
uniform sampler2D shadowMap;


// Lighting
uniform vec3 lightPos; 
uniform vec3 lightColor; 
uniform float ambientStrength;
uniform float specularStrength;
uniform float shininess;

// Matrices
uniform mat4 lightProjection;

// Fog stuff
vec3 fogColor = vec3(0.9, 0.9, 0.9); // white-gray color
float fogStart = 8.0; 
float fogEnd = 80.0;  

float calculateShadow(vec4 fragPosLight) 
{
    vec3 lightCoords = fragPosLight.xyz / fragPosLight.w;
    if (lightCoords.z > 1.0) 
        return 0.0;

    lightCoords = lightCoords * 0.5 + 0.5;
    float shadow = 0.0;
    float currentDepth = lightCoords.z;
    vec2 texSize = 1.0 / textureSize(shadowMap, 0);
    
    int shadowSample = 5;

    for (int y = -shadowSample; y <= shadowSample; y++) 
    {
        for (int x = -shadowSample; x <= shadowSample; x++) 
        {
            float closestDepth = texture(shadowMap, lightCoords.xy + vec2(x, y) * texSize).r;
            float bias = 0.0002;
            shadow += (currentDepth > closestDepth + bias) ? 1.0 : 0.0;
        }
    }
    
    return shadow / pow((shadowSample * 2 + 1), 2);
}

void main() 
{
    // Get texture color
    vec3 texColor = texture(texture0, fragTexCoord).xyz;  
    
    // Normalize the normal for proper lighting calculations
    vec3 norm = normalize(fragNormal); 
    norm = norm * 0.5 + 0.5;

   // Calculate the light direction and normalize it
    vec3 lightDir = normalize(lightPos - fragPos);

    // Calculate diffuse lighting based on the angle between normal and light direction
   float diff = max(0.8, dot(norm, lightDir));  // Remove clamping for more natural lighting

    vec3 diffuse = lightColor * diff;

      // Calculate the fragment's position in light space
    vec4 fragPosLight = lightProjection * vec4(fragPos, 1.0);

    // Calculate shadow
    float shadow = calculateShadow(fragPosLight);
   
    vec3 finalColor = texColor * lightColor * (1 - shadow);    

    // Calculate fog
    float fogFactor = clamp((fogEnd - camToDistance) / (fogEnd - fogStart), 0.0, 1.0);
    finalColor = mix(fogColor, finalColor.rgb, fogFactor);

    // Output the final fragment color
    FragColor = vec4(finalColor, 1.0);

}
