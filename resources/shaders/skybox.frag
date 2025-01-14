#version 330 core

in vec3 TexCoords;

out vec4 FragColor;

struct Material {
    samplerCube cube_map;
}; 

uniform Material material;

void main()
{    
    FragColor = texture(material.cube_map, TexCoords);
}