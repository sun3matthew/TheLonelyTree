#version 330 core

uniform sampler2D frame_buffer;

in vec2 TexCoords;
out vec4 FragColor;

void main() {
    FragColor = texture(frame_buffer, TexCoords);
}
