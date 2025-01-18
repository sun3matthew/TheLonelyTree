#version 330 core


// struct Material {
//     sampler2D diffuse1;
//     sampler2D specular1;
//     sampler2D glossy1;
// }; 

// struct LightingData {
//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;
// };
  
// struct DirectionalLight {
//     vec3 direction;
//     LightingData lightingData;
// };

// uniform DirectionalLight dirLight; 

// uniform Material material;
// uniform vec3 viewPos;

// in vec3 Normal;
// in vec3 FragPos;

in vec3 BaseColor;

out vec4 FragColor;

// vec3 CalcDirLight(DirectionalLight light, vec3 norm, vec3 viewDir){
//     vec3 lightDir = normalize(-light.direction.xyz);

//     vec3 ambient = light.lightingData.ambient * BaseColor;
//     // vec3 ambient = light.lightingData.ambient * vec3(0.5, 0.6, 0.25);

//     vec3 diffuse = light.lightingData.diffuse * BaseColor;
//     // vec3 diffuse = light.lightingData.diffuse * vec3(0.5, 0.6, 0.25);
//     diffuse *= max(dot(norm, lightDir), 0.0);

//     vec3 specular = light.lightingData.specular * vec3(1.0, 1.0, 1.0);
//     vec3 reflectDir = reflect(-lightDir, norm);
//     specular *= pow(max(dot(viewDir, reflectDir), 0.0), 64);

//     return ambient + diffuse + specular;
// }

void main()
{
    // vec3 norm = normalize(Normal);
    // vec3 viewDir = normalize(viewPos - FragPos);
    // vec3 result = CalcDirLight(dirLight, norm, viewDir);

    // vec3 result 

    FragColor = vec4(BaseColor, 1.0);
    // FragColor = vec4(1.0);
}