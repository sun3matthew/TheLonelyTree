#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 15) out;

uniform mat4 view;
uniform mat4 projection;

// uniform float size; // Size of the output triangle
// in vec3 position;

in float randomHash[];
in float clumpColor[];
in vec3 tipPosition[];
in vec2 facing[];
in vec3 bezierPoint[];

out vec3 FragPos;
out vec3 BaseColor; 
out vec3 Normal;  

void main() {
    // mat4 model = mat4(1);
    // model = projection * view * model;
    mat4 model = projection * view;

    float segmentSize = 1.1 + (clumpColor[0] * 0.4) + (randomHash[0] * 0.2);
    segmentSize *= 9;
    float grassWidth = 0.15;
    vec3 segmentPosition = vec3(gl_in[0].gl_Position); 
    float colorDarkness = 0.6 + (clumpColor[0] * 0.4) + (randomHash[0] * 0.2);
    vec3 color = vec3(0.5, 0.61, 0.24) * colorDarkness;

    vec3 grassOrt = vec3(-facing[0].y, 0, facing[0].x);
    for(int i = 0; i < 7; i++){
        float t = i / 7.0;
        segmentPosition =  vec3(gl_in[0].gl_Position) + (t*2*bezierPoint[0] + t*t*(tipPosition[0]-2*bezierPoint[0])) * segmentSize;

        // BaseColor = (0.5*(t) + 0.5) * color;
        BaseColor = (0.5*(t) + 0.5) * color;
        vec3 dx = 2*bezierPoint[0] + 2*t*(tipPosition[0]-2*bezierPoint[0]);
        // Normal = normalize(cross(dx, grassOrt));
        Normal = normalize(cross(grassOrt, dx));
        // Normal = normalize(cross(grassOrt, dx));

        FragPos = segmentPosition + grassOrt * grassWidth;
        gl_Position = model * vec4(FragPos, 1.0);
        // gl_Position = model * (vec4(segmentPosition, 1.0) + vec4(grassOrt,0) * grassWidth);
        EmitVertex();

        FragPos = segmentPosition - grassOrt * grassWidth;
        gl_Position = model * vec4(FragPos, 1.0);
        // gl_Position = model * (vec4(segmentPosition, 1.0) - vec4(grassOrt,0) * grassWidth);
        EmitVertex();

    }
    BaseColor = color;
    // gl_Position = model * (vec4(segmentPosition, 1.0));
    gl_Position = model * (gl_in[0].gl_Position + segmentSize * vec4(tipPosition[0], 0.0));
    EmitVertex();
    
    EndPrimitive();
}
