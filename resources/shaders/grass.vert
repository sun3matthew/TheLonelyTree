#version 330 core

#define GRID_SIZE 2

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 view;
uniform mat4 projection;
uniform float time;

// out VS_OUT {
//     float randomHash;
// } vs_out;  // Output block to pass data to the next stage
// out float tiltAngle;

out float randomHash;
out float clumpColor;

out vec2 facing;
out vec3 tipPosition;
out vec3 bezierPoint;

// out vec3 position;
// out vec3 Normal;
// out vec3 FragPos;
// out vec3 BaseColor;
float random(float seed) {
    return fract(sin(seed) * 843758.5453123);
}

void main(){
    float x = int(aPos.x / GRID_SIZE) * GRID_SIZE;
    float y = int(aPos.z / GRID_SIZE) * GRID_SIZE;

    float minDist = 100000.0;
    float minX = 0;
    float minY = 0;

    // TODO Optimize
    for(int x_i = -1; x_i <= 1; x_i++){
        for(int y_i = -1; y_i <= 1; y_i++){
            float xGrid = x + x_i * GRID_SIZE;
            float yGrid = y + y_i * GRID_SIZE;
            xGrid += (random((xGrid) * (yGrid)) - 0.5) * GRID_SIZE;
            yGrid += (random(xGrid * 2) - 0.5) * GRID_SIZE;

            float currentDist = (xGrid - aPos.x)*(xGrid - aPos.x) + (yGrid - aPos.z)*(yGrid - aPos.z);
            if(currentDist < minDist){
                minDist = currentDist;
                minX = xGrid;
                minY = yGrid;
            }
        }
    }

    vec2 clumpDirection = normalize(vec2(2*random((minX) * (minY / 2)) - 1, 2*random(minX + 3.3) - 1));
    vec2 universalDirection = normalize(vec2(1,2));
    float clumpHeight = random(minX * minY) * 0.4 + 0.55 + 0.1 * (random(aPos.x * (aPos.z + 1) - 0.5));
    // float clumpHeight = 1;
    clumpColor = random((minX + 4) * minY);
    
    facing = normalize(vec2(aPos.x - minX, aPos.z - minY));
    facing = normalize((facing * 0.2 + clumpDirection * 0.3 + universalDirection * 0.4));
    // facing = clumpDirection;

    // float tilt = 0.4 + 0.2 * (1 - (sqrt(minDist) / (GRID_SIZE * 3)));
    randomHash = random((aPos.x + 3003) * (aPos.z + 23));

    float tilt = 0.4 + 0.3 * (1 - (sqrt(minDist) / (GRID_SIZE * 2)));
    tilt += (randomHash - 0.5) * 0.4;
    tilt += (sin(time + 10000 * randomHash) * 2 - 1) * 0.04 * randomHash;

    vec3 grassPosXZ = vec3(facing.x, 0, facing.y) * cos(tilt);
    float grassPosXZDist = length(grassPosXZ);
    tipPosition = vec3(grassPosXZ.x, clumpHeight * sqrt(1 - grassPosXZDist * grassPosXZDist) ,grassPosXZ.z);



    float tiltPointT = 0.82;
    float tiltAmount = 0.56;
    // tiltAmount *= clumpHeight;
    // vec3 tiltPoint = tiltPointT * tipPosition;
    // vec3 grassNormal = normalize(cross(cross(tipPosition, vec3(0, 1, 0)),tipPosition));
    // bezierPoint = tiltPoint + grassNormal * tiltAmount;
    bezierPoint = (tiltPointT * tipPosition) + (normalize(cross(cross(tipPosition, vec3(0, 1, 0)),tipPosition))) * tiltAmount;

    float clumpAmt = 0.3 * randomHash;
    gl_Position = vec4(aPos - vec3(facing.x * clumpAmt, 0, facing.y * clumpAmt), 1.0)
        +0.2 * vec4(random(aPos.x + aPos.z), 0, random(aPos.z - 2 * aPos.z), 0);

    // randomHash = 0;
    // tipPosition = vec3(1.0);
    // bezierPoint = vec3(1.0);
    // gl_Position = vec4(aPos, 1.0);
}

