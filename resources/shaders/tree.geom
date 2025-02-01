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

    vec3 reference = (abs(normal.y) < abs(normal.x)) ? vec3(0,1,0) : vec3(1,0,0);

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

void main() {

    vec3 position = vec3(gl_in[0].gl_Position);
    // float r = 1 / position.y;

    for(int i = 0; i <= SUBDIVISIONS; i++){
        float lerp = i / float(SUBDIVISIONS);
        float t = 2.0 * PI * lerp;

        // Color = vec3(lerp, 1 - lerp, lerp * lerp);
        Color = vec3(lerp, 1 - lerp, lerp * lerp);

        Normal = normalize(vec3(sin(t), 0, cos(t)));

        float r = (10 - parentPosition[0].y) / 40;
        if (r < 0.0) r = 0.0;
        position = vec3(model * vec4(parametricRing(parentPosition[0], parentTiltDirection[0], t, r, r), 1.0));
        FragPos = position;
        TexCoords = vec2(lerp, 0);
        FragPosLightSpace = lightSpaceMatrix * vec4(position, 1.0);
        gl_Position = projection * view * vec4(position, 1.0);
        EmitVertex();

        r = (10 - gl_in[0].gl_Position.y) / 40;
        if (r < 0.0) r = 0.0;
        position = vec3(model * vec4(parametricRing(vec3(gl_in[0].gl_Position), tiltDirection[0], t, r, r), 1.0));
        FragPos = position;
        TexCoords = vec2(lerp, 1);
        FragPosLightSpace = lightSpaceMatrix * vec4(position, 1.0);
        gl_Position = projection * view * vec4(position, 1.0);
        EmitVertex();
    }
    EndPrimitive();
}
