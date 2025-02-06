#version 330 core

// #define MAX_VERTICES 18
#define MAX_VERTICES 24
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
in vec3 parentTiltDirection[];
in vec3 parentPosition[];
in int depth[];

out vec3 FragPos;
out vec3 Normal;  
out vec2 TexCoords;
out vec4 FragPosLightSpace;
out vec3 Color;

// vec3 parametricRing(vec3 position, float t, float a, float b){
//     return position + vec3(sin(t) * a, 0, cos(t) * b);
// }
vec3 parametricRing(vec3 position, vec3 normal, float t, float a, float b){
    normal = normalize(normal);

    // vec3 reference = (abs(normal.y) < abs(normal.x)) ? vec3(0,1,0) : vec3(1,0,0);
    vec3 reference = vec3(1,0,0);

    vec3 tangent = normalize(cross(reference, normal));  
    vec3 bitangent = cross(normal, tangent);

    return position + (tangent * (cos(t) * a) + bitangent * (sin(t) * b));

    // rotation matrix
    // float cosTheta = cos(theta);
    // float sinTheta = sin(theta);
    // vec3 rotatedTangent   = tangent * cosTheta + bitangent * sinTheta;
    // vec3 rotatedBitangent = bitangent * cosTheta - tangent * sinTheta;

    // return position + (rotatedTangent * (cos(t) * a) + rotatedBitangent * (sin(t) * b));
}

float random(float seed) {
    return fract(sin(seed) * 843758.5453123);
}
float hash(float a, float b) {
    return sin(a * 12.9898 + b * 78.233) * 43758.5453;
}

void main() {

    vec3 position = vec3(gl_in[0].gl_Position);
    float density = 20;

    vec2 offset = vec2(random(hash(position.x, position.z)), random(hash(position.x * 2, position.z))) * 0.6;
    float stretch = random(hash(position.x * 2, position.z)) * 0.2 + 0.2;
    // float r = 1 / position.y;

    for(int i = 0; i <= SUBDIVISIONS; i++){
        float lerp = i / float(SUBDIVISIONS);
        float t = 2.0 * PI * lerp;

        // Color = vec3(lerp, 1 - lerp, lerp * lerp);
        // Color = vec3(depth[0] / 100.0);
        Color = vec3(tiltDirection[0]);

        // Normal = normalize(vec3(sin(t), 0, cos(t)));

        // e^{-\frac{x}{10}}
        float r = exp(depth[0] / -density) * 0.4;
        position = vec3(model * vec4(parametricRing(parentPosition[0], parentTiltDirection[0], t, r, r), 1.0));
        Normal = normalize(position - parentPosition[0]);
        FragPos = position;
        TexCoords = offset + vec2(lerp * stretch * 2, 0);
        FragPosLightSpace = lightSpaceMatrix * vec4(position, 1.0);
        gl_Position = projection * view * vec4(position, 1.0);
        EmitVertex();

        r = exp((depth[0] + 1) / -density) * 0.4;
        if (r < 0.0) r = 0.0;
        position = vec3(model * vec4(parametricRing(vec3(gl_in[0].gl_Position), tiltDirection[0], t, r, r), 1.0));
        Normal = normalize(position - vec3(gl_in[0].gl_Position));
        FragPos = position;
        TexCoords = offset + vec2(lerp * stretch * 2, stretch);
        FragPosLightSpace = lightSpaceMatrix * vec4(position, 1.0);
        gl_Position = projection * view * vec4(position, 1.0);
        EmitVertex();
    }
    EndPrimitive();
}
