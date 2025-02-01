#version 330 core

struct LightingData {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
  
struct DirectionalLight {
    vec3 direction;
    LightingData lightingData;
};

uniform DirectionalLight dirLight; 
uniform vec3 viewPos;

uniform sampler2D depth_buffer;
uniform sampler2D diffuse;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in vec4 FragPosLightSpace;
in vec3 Color;

out vec4 FragColor;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir){
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(depth_buffer, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);  
    bias /= 5;
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;  

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}


vec3 CalcDirLight(DirectionalLight light, vec3 norm, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction.xyz);

    // vec3 BaseColor = Color;
    vec3 BaseColor = vec3(texture(diffuse, TexCoords));

    vec3 ambient = light.lightingData.ambient * BaseColor;

    vec3 diffuse = light.lightingData.diffuse * BaseColor;
    diffuse *= max(dot(norm, lightDir), 0.0);

    float specularAmount = 0.8;
    if(abs(TexCoords.x - 0.5) < 0.12){
        specularAmount = 0.3;
    }
    vec3 specular = light.lightingData.specular * specularAmount;
    vec3 reflectDir = reflect(-lightDir, norm);
    specular *= pow(max(dot(viewDir, reflectDir), 0.0), 16);
    specular = vec3(0.0);

    float shadow = ShadowCalculation(FragPosLightSpace, norm, lightDir);

    return ambient + (1.0 - shadow) * (diffuse * 0.6 + specular * 0.13);
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    FragColor = vec4(result, 1.0);
    // FragColor = vec4(BaseColor, 1.0);
    // FragColor = vec4(1.0);
}