#version 330 core


uniform sampler2D diffuse;
uniform sampler2D specular;
uniform sampler2D glossy;
uniform sampler2D normal;

uniform sampler2D depth_buffer;

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

in vec3 FragPos;
in vec2 TexCoords;
in vec4 FragPosLightSpace;
in mat3 TBN;

out vec4 FragColor;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir){
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(depth_buffer, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);  
    bias /= 5;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(depth_buffer, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(depth_buffer, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}


vec3 CalcDirLight(DirectionalLight light, vec3 norm, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction.xyz);

    vec3 ambient = light.lightingData.ambient * vec3(texture(diffuse, TexCoords));

    vec3 diffuse = light.lightingData.diffuse * vec3(texture(diffuse, TexCoords));
    diffuse *= max(dot(norm, lightDir), 0.0);

    vec3 specular = light.lightingData.specular * vec3(texture(specular, TexCoords));
    vec3 reflectDir = reflect(-lightDir, norm);
    float glossy = float(texture(glossy, TexCoords)) * 100;
    specular *= pow(max(dot(viewDir, reflectDir), 0.0), 64);

    float shadow = ShadowCalculation(FragPosLightSpace, norm, lightDir);

    return ambient + (1.0 - shadow) * (diffuse + specular);
    // return ambient + diffuse;
}

// vec3 CalcPointLight(PointLight light, vec3 norm, vec3 FragPos, vec3 viewDir){
//     vec3 lightDir = normalize(light.position - FragPos);

//     vec3 ambient = light.lightingData.ambient * vec3(texture(diffuse, TexCoords));

//     vec3 diffuse = light.lightingData.diffuse * vec3(texture(diffuse, TexCoords));
//     diffuse *= max(dot(norm, lightDir), 0.0);

//     vec3 specular = light.lightingData.specular * vec3(texture(specular, TexCoords));
//     vec3 reflectDir = reflect(-lightDir, norm);
//     float glossy = float(texture(glossy, TexCoords)) * 100;
//     specular *= pow(max(dot(viewDir, reflectDir), 0.0), glossy);

//     float distance = length(light.position - FragPos);
//     float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * distance + light.attenuation.z * (distance * distance)); 
    
//     return (ambient + diffuse + specular) * attenuation;
// }

void main()
{
    vec3 norm = texture(normal, TexCoords).rgb;
    norm = norm * 2.0 - 1.0;   
    norm = normalize(TBN * norm); 

    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // for(int i = 0; i < numPointLights; i++)
    //     result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    

    float a = vec4(texture(diffuse, TexCoords)).a;

    if(a < 0.8){
        discard;
    }
    FragColor = vec4(result, a);
    // FragColor = vec4(norm, 1.0);

    // FragColor = vec4(texture(depth_buffer, TexCoords).rrr, 1.0);
}  