#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 direction;

layout (location = 2) in vec3 parentVector;
layout (location = 3) in vec3 parentDirection;

// uniform mat4 model;
// uniform mat4 view;
// uniform mat4 projection;

// uniform mat4 lightSpaceMatrix;

out vec3 tiltDirection;

void main(){
    tiltDirection = direction;
    gl_Position = vec4(aPos, 1.0);
}

