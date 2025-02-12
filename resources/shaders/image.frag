#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D diffuse;
uniform vec3 color;

void main()
{    
    vec4 sampled = texture(diffuse, TexCoords);
    if (sampled.a < 0.9)
        discard;
    FragColor = vec4(color, 1.0) * sampled;
} 