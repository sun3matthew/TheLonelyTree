#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 direction;

layout (location = 2) in vec3 parentVector;
layout (location = 3) in vec3 parentDirection;

layout (location = 4) in int currentDepth;

out vec3 parentPosition;
out vec3 tiltDirection;
out vec3 parentTiltDirection;
out int depth;

void main(){
    gl_Position = vec4(aPos, 1.0);
    parentPosition = parentVector;
    tiltDirection = direction;
    parentTiltDirection = parentDirection;

    depth = currentDepth;
}

