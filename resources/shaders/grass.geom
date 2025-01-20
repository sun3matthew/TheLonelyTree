#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 15) out;

uniform mat4 view;
uniform mat4 projection;
uniform float time;

uniform vec3 viewPos;

// uniform float size; // Size of the output triangle
// in vec3 position;

in float randomHash[];
in float clumpColor[];
in vec3 tipPosition[];
in vec2 facing[];
in vec3 bezierPoint[];

in float perlinValue[];

out vec3 FragPos;
out vec3 BaseColor; 
out vec3 Normal;  

void main() {
    float segmentScale[7] = float[](0.2, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9);

    mat4 model = projection * view;

    float segmentSize = 1.1 + (clumpColor[0] * 0.4) + (randomHash[0] * 0.2);
    segmentSize *= 12;
    // segmentSize = 1.0;
    float grassWidth = 0.1;
    vec3 segmentPosition = vec3(gl_in[0].gl_Position); 
    float colorDarkness = 0.6 + (clumpColor[0] * 0.4) + (randomHash[0] * 0.2);
    vec3 color = vec3(0.5, 0.61, 0.24) * colorDarkness;

    vec3 grassOrt = vec3(-facing[0].y, 0, facing[0].x);


    // viewDir = vec3(viewDir.x, 0, viewDir.z);
    // float orthogonalCorrection = abs(dot(viewDir, normalize(grassOrt)));
    // vec3 grassOrtModified = (orthogonalCorrection) * vec3(facing[0].x, 0, facing[0].y) + (1 - orthogonalCorrection) * grassOrt;

    for(int i = 0; i < 7; i++){
        float t = segmentScale[i];
        segmentPosition =  vec3(gl_in[0].gl_Position) + (t*2*bezierPoint[0] + t*t*(tipPosition[0]-2*bezierPoint[0])) * segmentSize;
        
        vec3 viewDir = normalize(viewPos - segmentPosition);
        vec3 grassOrtModified = cross(viewDir, vec3(0, 1, 0));
        // float blend = length(grassOrtModified);
        // grassOrtModified = blend * grassOrt + (1 - blend) * grassOrtModified;
        grassOrtModified = normalize(grassOrtModified);
        // float blend = abs(dot(viewDir, vec3(0, 1, 0)));
        // if(dot(grassOrt, grassOrtModified) < 0)
        //     grassOrt = -grassOrt;
        // grassOrtModified = grassOrt + grassOrtModified;

        BaseColor = (0.5*(t) + 0.5) * color;
        BaseColor *= (perlinValue[0] * 0.25) + 0.75;
        // BaseColor = vec3(orthogonalCorrection);
        vec3 dx = 2*bezierPoint[0] + 2*t*(tipPosition[0]-2*bezierPoint[0]);
        Normal = normalize(cross(grassOrt, dx));

        FragPos = segmentPosition + grassOrtModified * (grassWidth + (1 - t));
        gl_Position = model * vec4(FragPos, 1.0);
        EmitVertex();

        FragPos = segmentPosition - grassOrtModified * (grassWidth + (1 - t));
        gl_Position = model * vec4(FragPos, 1.0);
        EmitVertex();
    }
    BaseColor = color;
    gl_Position = model * (gl_in[0].gl_Position + segmentSize * vec4(tipPosition[0], 0.0));
    EmitVertex();
    
    EndPrimitive();
}
