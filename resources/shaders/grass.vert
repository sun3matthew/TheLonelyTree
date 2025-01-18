#version 330 core

#define GRID_SIZE 3

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 view;
uniform mat4 projection;

// out VS_OUT {
//     float randomHash;
// } vs_out;  // Output block to pass data to the next stage
out float randomHash;

// out vec3 position;
// out vec3 Normal;
// out vec3 FragPos;
// out vec3 BaseColor;
float random(float seed) {
    return fract(sin(seed) * 843758.5453123);
}

void main(){
    int x = int(aPos.x / GRID_SIZE) * GRID_SIZE;
    int y = int(aPos.z / GRID_SIZE) * GRID_SIZE;

    float minDist = 100000.0;
    float currentDist = 0;

    // TODO Optimize
    for(int x_i = -1; x_i <= 1; x_i++){
        for(int y_i = -1; y_i <= 1; y_i++){
            float xGrid = x + x_i * GRID_SIZE;
            float yGrid = y + y_i * GRID_SIZE;
            xGrid += (random((xGrid) * (yGrid)) - 0.5) * GRID_SIZE;
            yGrid += (random(xGrid * 2) - 0.5) * GRID_SIZE;
            // float yGrid = y + y_i * GRID_SIZE + (random((x + x_i) + (y + y_i)) - 0.5) * GRID_SIZE;

            currentDist = (xGrid - aPos.x)*(xGrid - aPos.x) + (yGrid - aPos.z)*(yGrid - aPos.z);
            if(currentDist < minDist){
                minDist = currentDist;
            }
        }
    }

    randomHash = sqrt(minDist) / (GRID_SIZE * 2);

    // position = aInstancedPosition;

    // mat4 model = mat4(
    //     1.0, 0.0, 0.0, 0.0,
    //     0.0, 1.0, 0.0, 0.0,
    //     0.0, 0.0, 1.0, 0.0,
    //     aInstancedPosition.x, aInstancedPosition.y, aInstancedPosition.z, 1.0
    // );
    // mat4 model = mat4(1);

    // gl_Position = projection * view * model * vec4(aPos, 1.0);
    // gl_Position = vec4(aPos, 1.0) + vec4(random(aPos.x + aPos.z), 0, random(aPos.z - 2 * aPos.z), 0);
    gl_Position = vec4(aPos, 1.0);
    // randomHash = random(aPos.x + aPos.z);
    // Normal = mat3(transpose(inverse(model))) * aNormal;  
    // FragPos = vec3(model * vec4(aPos, 1.0));
    // BaseColor = vec3(aInstancedPosition.x / 200, aInstancedPosition.y / 100, aInstancedPosition.z / 100);
}

