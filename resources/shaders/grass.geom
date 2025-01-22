#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 15) out;

uniform mat4 view;
uniform mat4 projection;
uniform float time;

uniform vec3 viewPos;
uniform vec3 worldCenter;

// uniform float size; // Size of the output triangle
// in vec3 position;

in float randomHash[];
in float clumpColor[];
in vec3 tipPosition[];
in vec2 facing[];
in vec3 bezierPoint[];
// in vec3 debugColor[];

in float perlinValue[];

out vec3 FragPos;
out vec3 BaseColor; 
out vec3 Normal;  
out vec2 TexCoords;

void main() {
    if(randomHash[0] != 0.0){
        float segmentScale[7 * 3] = float[](
            0.20, 0.30, 0.40, 0.60, 0.70, 0.80, 0.90,
            0.25, 0.45, 0.65, 0.80, 0.90, 0.00, 0.00,
            0.40, 0.65, 0.80, 0.00, 0.00, 0.00, 0.00
        );

        mat4 model = projection * view;

        float segmentSize = 1.1 + (clumpColor[0] * 0.4) + (randomHash[0] * 0.2);
        segmentSize *= 12;
        // segmentSize = 1.0;

        float grassWidth = 0.1;
        vec3 segmentPosition = vec3(gl_in[0].gl_Position); 
        float colorDarkness = 0.6 + (clumpColor[0] * 0.4) + (randomHash[0] * 0.2);
        vec3 color = vec3(0.5, 0.61, 0.24) * colorDarkness;
        // color = debugColor[0] * colorDarkness;

        vec3 grassOrt = vec3(-facing[0].y, 0, facing[0].x);

        float distToCamera = length(viewPos - segmentPosition);
        grassWidth += distToCamera / 1700 * 0.15;
        // segmentSize *= (1 + distToCamera/4000);


        // viewDir = vec3(viewDir.x, 0, viewDir.z);
        // float orthogonalCorrection = abs(dot(viewDir, normalize(grassOrt)));
        // vec3 grassOrtModified = (orthogonalCorrection) * vec3(facing[0].x, 0, facing[0].y) + (1 - orthogonalCorrection) * grassOrt;

        int lod = int(length(worldCenter - segmentPosition) / 400);
        if (lod > 2)
            lod = 2;
        

        for(int i = 0; i < 7 - lod * 2; i++){
            float t = segmentScale[i + lod * 7];
            
            vec3 viewDir = normalize(viewPos - segmentPosition);
            vec3 grassOrtModified = cross(viewDir, vec3(0, 1, 0));
            // float blend = length(grassOrtModified);
            // grassOrtModified = blend * grassOrt + (1 - blend) * grassOrtModified;
            grassOrtModified = normalize(grassOrtModified);
            // float blend = abs(dot(viewDir, vec3(0, 1, 0)));
            // if(dot(grassOrt, grassOrtModified) < 0)
            //     grassOrt = -grassOrt;
            // grassOrtModified = grassOrt;

            BaseColor = (0.5*(t) + 0.5) * color;
            // BaseColor *= (perlinValue[0] * 0.25) + 0.75;
            // BaseColor = vec3(lod / 3.0);
            vec3 dx = 2*bezierPoint[0] + 2*t*(tipPosition[0]-2*bezierPoint[0]);
            vec3 baseNormal = normalize(cross(grassOrt, dx));

            TexCoords = vec2(0, t);
            Normal = baseNormal + grassOrt / 8;
            FragPos = segmentPosition + grassOrtModified * (grassWidth + (1 - t) * grassWidth);
            gl_Position = model * vec4(FragPos, 1.0);
            EmitVertex();

            TexCoords = vec2(1, t);
            Normal = baseNormal - grassOrt / 8;
            FragPos = segmentPosition - grassOrtModified * (grassWidth + (1 - t) * grassWidth);
            gl_Position = model * vec4(FragPos, 1.0);
            EmitVertex();

            segmentPosition =  vec3(gl_in[0].gl_Position) + (t*2*bezierPoint[0] + t*t*(tipPosition[0]-2*bezierPoint[0])) * segmentSize;
        }
        vec3 dx = 2*bezierPoint[0] + 2*(tipPosition[0]-2*bezierPoint[0]);
        TexCoords = vec2(0.5, 1);
        Normal = normalize(cross(grassOrt, dx));
        FragPos = segmentPosition;
        BaseColor = color;
        gl_Position = model * (gl_in[0].gl_Position + segmentSize * vec4(tipPosition[0], 0.0));
        EmitVertex();
        
        EndPrimitive();
    }
}
