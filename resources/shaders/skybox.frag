#version 330 core

in vec3 TexCoords;
in vec3 FragPos;

out vec4 FragColor;

uniform samplerCube cube_map;
uniform float day;

void main(){    
    vec3 newCoords = TexCoords;
    float fog = 0.0;
    float dist = 0.0;
    if(TexCoords.y < 0.0){
        newCoords.y = -TexCoords.y;
        FragColor = texture(cube_map, newCoords) * day * 0.9;
        return;
    }

    // dist = newCoords.y / 0.5;
    // fog = exp(-dist * dist * 40.0);
    // if (fog > 1.0)
    //     fog = 1.0;

    // FragColor = texture(cube_map, newCoords) + vec4(vec3(0.6) * fog, 1.0);
    FragColor = texture(cube_map, newCoords) * day;
}