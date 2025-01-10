#version 330 core


struct Material {
    sampler2D diffuse1;
    sampler2D specular1;
    sampler2D glossy1;
    float shininess;
}; 

struct LightingData {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
  
struct DirectionalLight {
    vec3 direction;
    LightingData lightingData;
};

struct PointLight {
    vec3 position;
    vec3 attenuation; // constant, linear, quadratic
    LightingData lightingData;
};

uniform DirectionalLight dirLight; 
#define MAX_POINT_LIGHTS 10
uniform int numPointLights;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform Material material;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

vec3 CalcDirLight(DirectionalLight light, vec3 norm, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction.xyz);

    vec3 ambient = light.lightingData.ambient * vec3(texture(material.diffuse1, TexCoords));

    vec3 diffuse = light.lightingData.diffuse * vec3(texture(material.diffuse1, TexCoords));
    diffuse *= max(dot(norm, lightDir), 0.0);

    vec3 specular = light.lightingData.specular * vec3(texture(material.specular1, TexCoords));
    vec3 reflectDir = reflect(-lightDir, norm);
    float glossy = float(texture(material.glossy1, TexCoords)) * 100;
    specular *= pow(max(dot(viewDir, reflectDir), 0.0), glossy);
    // specular *= pow(max(dot(viewDir, reflectDir), 0.0), 64);

    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 norm, vec3 FragPos, vec3 viewDir){
    vec3 lightDir = normalize(light.position - FragPos);

    vec3 ambient = light.lightingData.ambient * vec3(texture(material.diffuse1, TexCoords));

    vec3 diffuse = light.lightingData.diffuse * vec3(texture(material.diffuse1, TexCoords));
    diffuse *= max(dot(norm, lightDir), 0.0);

    vec3 specular = light.lightingData.specular * vec3(texture(material.specular1, TexCoords));
    vec3 reflectDir = reflect(-lightDir, norm);
    specular *= pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * distance + light.attenuation.z * (distance * distance)); 
    
    // float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance)); 
            // glm::vec3(1.0f, 0.09f, 0.032f),
    // attenuation = 1.0;

    return (ambient + diffuse + specular) * attenuation;
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    for(int i = 0; i < numPointLights; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    

    float a = vec4(texture(material.diffuse1, TexCoords)).a;

    if(a < 0.1){
        discard;
    }
    FragColor = vec4(result, a);
}  