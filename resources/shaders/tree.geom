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

uniform float rootBranchDepth;
uniform int numNodes;
uniform int branchDepth;

// vec3 glPosition
in vec3 parentPosition[];
in vec3 tiltDirection[];
in vec3 parentTiltDirection[];
in float treeRadius[];
in float parentTreeRadius[];


out vec3 FragPos;
out vec2 TexCoords;
out vec4 FragPosLightSpace;
out vec3 Color;
out mat3 TBN;

vec3 parametricRing(vec3 position, vec3 normal, float t, float a, float b){
    t *= PI * 2;
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

mat3 calculateTBN(vec3 normal, vec3 tangent){
    vec3 bitangent = cross(normal, tangent);
    return mat3(tangent, bitangent, normal);
}

vec3 applyModel(vec3 position){
    return vec3(model * vec4(position, 1.0));
}

float random(float seed) {
    return fract(sin(seed) * 843758.5453123);
}
float hash(float a, float b) {
    return sin(a * 12.9898 + b * 78.233) * 43758.5453;
}

void main() {

    vec3 position = vec3(gl_in[0].gl_Position);

    vec2 offset = vec2(random(hash(position.x, position.z)), random(hash(position.x * 2, position.z))) * 0.6;
    float stretch = random(hash(position.x * 2, position.z)) * 0.1 + 0.1;

    float scale = 0.2;
    float rParent = scale * parentTreeRadius[0];
    float r = scale * treeRadius[0];

    float lerp = -1 / float(SUBDIVISIONS);

    vec3 lastPositionParent = applyModel(parametricRing(parentPosition[0], parentTiltDirection[0], lerp, rParent, rParent));
    vec3 lastPosition = applyModel(parametricRing(vec3(gl_in[0].gl_Position), tiltDirection[0], lerp, r, r));

    for(int i = 0; i <= SUBDIVISIONS; i++){
        lerp += 1 / float(SUBDIVISIONS);

        // Color = vec3(lerp, 1 - lerp, lerp * lerp);
        // Color = vec3(depth[0] / 100.0);
        Color = vec3(tiltDirection[0]);

        position = applyModel(parametricRing(parentPosition[0], parentTiltDirection[0], lerp, rParent, rParent));
        TBN = calculateTBN(
            normalize(position - applyModel(parentPosition[0])),
            -normalize(position - lastPositionParent)); // flip tangent
        lastPositionParent = position;
        FragPos = position;
        TexCoords = offset + vec2(lerp * stretch * 2, 0);
        FragPosLightSpace = lightSpaceMatrix * vec4(position, 1.0);
        gl_Position = projection * view * vec4(position, 1.0);
        EmitVertex();

        position = applyModel(parametricRing(vec3(gl_in[0].gl_Position), tiltDirection[0], lerp, r, r));
        TBN = calculateTBN(
            normalize(position - vec3(model * gl_in[0].gl_Position)),
            -normalize(position - lastPosition));
        lastPosition = position;
        FragPos = position;
        TexCoords = offset + vec2(lerp * stretch * 2, stretch);
        FragPosLightSpace = lightSpaceMatrix * vec4(position, 1.0);
        gl_Position = projection * view * vec4(position, 1.0);
        EmitVertex();
    }
    EndPrimitive();
}
