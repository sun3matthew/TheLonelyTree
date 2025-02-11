#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 direction;
layout (location = 2) in int leafType;

out vec3 leafDirection;
out int leafTextureIndex;

void main(){
    gl_Position = vec4(aPos, 1.0);
    leafDirection = direction;
    leafTextureIndex = leafType;
}

