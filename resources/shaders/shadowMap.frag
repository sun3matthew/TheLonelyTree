#version 330 core

uniform sampler2D diffuse;

in vec2 TexCoords;

void main() {
    vec4 FragColor = texture(diffuse, TexCoords);
    if(FragColor.a < 0.1)
        discard;
}  