#version 330 core

layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
// layout (location = 3) in vec3 aTangent;
// layout (location = 4) in vec3 aBitangent;  

out vec2 TexCoords;

void main() {
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos.xy, 0.0, 1.0);
}