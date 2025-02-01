#version 330 core

#define MAX_VERTICES 18
#define SUBDIVISIONS MAX_VERTICES / 2 - 1
#define PI 3.1415926535

layout(points) in;
layout(triangle_strip, max_vertices = MAX_VERTICES) out;
// 8 subdivision
// (8 + 1) * 2

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

uniform float time;

in vec3 tiltDirection[];

out vec3 FragPos;
out vec3 Normal;  
out vec2 TexCoords;
out vec4 FragPosLightSpace;

vec3 parametricRing(vec3 position, float t, float a, float b){
    return position + vec3(sin(t) * a, 0, cos(t) * b);
}

void main() {
    float r = 100;

    vec3 position = vec3(gl_in[0].gl_Position);

    for(int i = 0; i <= SUBDIVISIONS; i++){
        float lerp = i / float(SUBDIVISIONS);
        float t = 2.0 * PI * lerp;

        Normal = normalize(vec3(sin(t), 0, cos(t)));

        position = vec3(model * vec4(parametricRing(vec3(gl_in[0].gl_Position), t, r, r), 1.0));
        FragPos = position;
        TexCoords = vec2(lerp, 0);
        FragPosLightSpace = lightSpaceMatrix * vec4(position, 1.0);
        gl_Position = projection * view * vec4(position, 1.0);
        EmitVertex();

        // position = vec3(model * vec4(parametricRing(vec3(gl_in[0].gl_Position) + tiltDirection[0] * 9, t, r, r), 1.0));
        position = vec3(model * vec4(parametricRing(vec3(gl_in[0].gl_Position) + vec3(0, 1, 0) * 9, t, r, r), 1.0));
        FragPos = position;
        TexCoords = vec2(lerp, 1);
        FragPosLightSpace = lightSpaceMatrix * vec4(position, 1.0);
        gl_Position = projection * view * vec4(position, 1.0);
        EmitVertex();
    }
    EndPrimitive();
}
