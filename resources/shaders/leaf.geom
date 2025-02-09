#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

in vec3 leafDirection[];
in int leafTextureIndex[];

out vec3 FragPos;
out vec4 FragPosLightSpace;
out vec2 TexCoords;
out vec3 Color;
out mat3 TBN;

void main() {
    float scale = 1.0;
    float r = 1.0;

    // calculate normal
    vec3 normal = normalize(cross(vec3(model * vec4(leafDirection[0], 0.0)), vec3(0, 1, 0)));
    vec3 tangent = normalize(cross(normal, leafDirection[0]));
    vec3 bitangent = cross(normal, tangent);
    TBN = mat3(tangent, bitangent, normal);

    vec3 modelPosition = vec3(gl_in[0].gl_Position);
    vec3 position = vec3(0);
    Color = modelPosition;

    position = vec3(model * vec4(modelPosition, 1.0));
    FragPos = position;
    TexCoords = vec2(0, 0);
    FragPosLightSpace = lightSpaceMatrix * vec4(position, 1.0);
    gl_Position = projection * view * vec4(position, 1.0);
    EmitVertex();

    position = vec3(model * vec4(modelPosition + (leafDirection[0] * scale * 0.5) + (tangent * scale * 0.5 * r), 1.0));
    FragPos = position;
    TexCoords = vec2(1, 0);
    FragPosLightSpace = lightSpaceMatrix * vec4(position, 1.0);
    gl_Position = projection * view * vec4(position, 1.0);
    EmitVertex();

    position = vec3(model * vec4(modelPosition + (leafDirection[0] * scale * 0.5) - (tangent * scale * 0.5 * r), 1.0));
    FragPos = position;
    TexCoords = vec2(0, 1);
    FragPosLightSpace = lightSpaceMatrix * vec4(position, 1.0);
    gl_Position = projection * view * vec4(position, 1.0);
    EmitVertex();

    position = vec3(model * vec4(modelPosition + (leafDirection[0] * scale), 1.0));
    FragPos = position;
    TexCoords = vec2(1, 1);
    FragPosLightSpace = lightSpaceMatrix * vec4(position, 1.0);
    gl_Position = projection * view * vec4(position, 1.0);
    EmitVertex();

    EndPrimitive();
}
