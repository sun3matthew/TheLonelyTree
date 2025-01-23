#version 330 core


uniform sampler2D diffuse;
uniform sampler2D specular;
uniform sampler2D glossy;
uniform sampler2D normal;

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

uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in mat3 TBN;


out vec4 FragColor;

vec3 CalcDirLight(DirectionalLight light, vec3 norm, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction.xyz);

    vec3 ambient = light.lightingData.ambient * vec3(texture(diffuse, TexCoords));

    vec3 diffuse = light.lightingData.diffuse * vec3(texture(diffuse, TexCoords));
    diffuse *= max(dot(norm, lightDir), 0.0);

    vec3 specular = light.lightingData.specular * vec3(texture(specular, TexCoords));
    vec3 reflectDir = reflect(-lightDir, norm);
    float glossy = float(texture(glossy, TexCoords)) * 100;
    specular *= pow(max(dot(viewDir, reflectDir), 0.0), 64);

    return ambient + diffuse + specular;
    // return ambient + diffuse;
}

vec3 CalcPointLight(PointLight light, vec3 norm, vec3 FragPos, vec3 viewDir){
    vec3 lightDir = normalize(light.position - FragPos);

    vec3 ambient = light.lightingData.ambient * vec3(texture(diffuse, TexCoords));

    vec3 diffuse = light.lightingData.diffuse * vec3(texture(diffuse, TexCoords));
    diffuse *= max(dot(norm, lightDir), 0.0);

    vec3 specular = light.lightingData.specular * vec3(texture(specular, TexCoords));
    vec3 reflectDir = reflect(-lightDir, norm);
    float glossy = float(texture(glossy, TexCoords)) * 100;
    specular *= pow(max(dot(viewDir, reflectDir), 0.0), glossy);

    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * distance + light.attenuation.z * (distance * distance)); 
    
    return (ambient + diffuse + specular) * attenuation;
}

void main()
{
    // vec3 norm = normalize(Normal);
    vec3 norm = texture(normal, TexCoords).rgb;
    norm = norm * 2.0 - 1.0;   
    norm = normalize(TBN * norm); 

    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    for(int i = 0; i < numPointLights; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    

    float a = vec4(texture(diffuse, TexCoords)).a;

    if(a < 0.8){
        discard;
    }
    FragColor = vec4(result, a);
}  