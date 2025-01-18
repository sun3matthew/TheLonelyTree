#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 view;
uniform mat4 projection;

// uniform float size; // Size of the output triangle
// in vec3 position;

in float randomHash[];

out vec3 BaseColor;  // Example output for the fragment shader

void main() {
    // mat4 model = mat4(1);
    // model = projection * view * model;
    mat4 model = projection * view;

    float segmentSize = 0.5;
    float grassWidth = 0.2;
    vec3 grassDirection = vec3(0, 1.0, 0);
    vec3 segmentPosition = vec3(gl_in[0].gl_Position); 
    for(int i = 0; i < 1; i++){
        BaseColor = vec3(i / 7.0, 0.0, 0.0); // Red

        gl_Position = model * (vec4(segmentPosition, 1.0) + vec4(grassWidth, 0, 0.0, 0.0));
        EmitVertex();

        gl_Position = model * (vec4(segmentPosition, 1.0) - vec4(grassWidth, 0, 0.0, 0.0));
        EmitVertex();

        segmentPosition += grassDirection * segmentSize;
    }
    // BaseColor = vec3(1, 0.0, 0.0); // Red
    BaseColor = vec3(randomHash[0], 0.0, 0.0); // Red
    gl_Position = model * (vec4(segmentPosition, 1.0));
    EmitVertex();
    
    EndPrimitive();
}
