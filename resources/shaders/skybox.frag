#version 330 core

in vec3 TexCoords;

out vec4 FragColor;

uniform samplerCube cube_map;

void main(){    
    FragColor = texture(cube_map, TexCoords);
}