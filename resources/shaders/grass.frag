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


uniform sampler2D diffuse;
uniform sampler2D specular;
uniform sampler2D normal;

in vec3 Normal;
in vec3 BaseColor;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

vec3 CalcDirLight(DirectionalLight light, vec3 norm, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction.xyz);

    vec3 ambient = light.lightingData.ambient * BaseColor;

    vec3 diffuse = light.lightingData.diffuse * BaseColor;
    diffuse *= max(dot(norm, lightDir), 0.0);

    float specularAmount = 0.8;
    if(abs(TexCoords.x - 0.5) < 0.15)
        specularAmount = 0.1;
    vec3 specular = light.lightingData.specular * specularAmount;
    vec3 reflectDir = reflect(-lightDir, norm);
    specular *= pow(max(dot(viewDir, reflectDir), 0.0), 16);

    return ambient + diffuse * 0.6 + specular * 0.13;
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