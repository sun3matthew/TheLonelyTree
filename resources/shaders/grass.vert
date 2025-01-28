#version 330 core

#define GRID_SIZE 16

layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;
uniform float time;

uniform sampler2D perlin_lane;

out float randomHash;
out float clumpColor;

out vec2 facing;
out vec3 tipPosition;
out vec3 bezierPoint;
out vec3 debugColor;

out float perlinValue;


float random(float seed) {
    return fract(sin(seed) * 843758.5453123);
}
float hash(float a, float b) {
    return sin(a * 12.9898 + b * 78.233) * 43758.5453;
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
            xGrid += (random(hash(xGrid, yGrid)) - 0.5) * GRID_SIZE;
            yGrid += (random(hash(xGrid * 2, yGrid)) - 0.5) * GRID_SIZE;

            float currentDist = (xGrid - aPos.x)*(xGrid - aPos.x) + (yGrid - aPos.z)*(yGrid - aPos.z);
            if(currentDist < minDist){
                minDist = currentDist;
                minX = xGrid;
                minY = yGrid;
            }
        }
    }

    vec2 universalDirection = normalize(vec2(3,1));

    ivec2 texSize = textureSize(perlin_lane, 0);
    // vec2 texCoords = vec2(aPos.x/texSize.y, aPos.z/texSize.y);
    vec2 texCoords = vec2(aPos.x/3048, aPos.z/3048) - universalDirection * (time / 12);
    float perlin = texture(perlin_lane, texCoords).r;
    perlinValue = perlin;
    // perlinValue = 0.0;

    vec2 clumpDirection = normalize(vec2(2*random(hash(minX, minY / 2)) - 1, 2*random(hash(minX + 3.3, minY)) - 1));
    float clumpHeight = random(hash(minX, minY)) * 0.4 + 0.55 + 0.1 * (random(hash(aPos.x, aPos.z + 1)) - 0.5);
    clumpHeight *= 3.4;
    clumpColor = random(hash((minX + 4), minY));
    
    float perlinTilt = pow(perlinValue + 0.3, 1.6);

    facing = normalize(vec2(aPos.x - minX, aPos.z - minY)) + 0.8 * normalize(vec2(random(hash(aPos.x, aPos.z + 1)) - 0.5, random(hash(aPos.x, aPos.z)) - 0.5));
    facing = normalize((facing * 0.2 + clumpDirection * 0.3 + perlinTilt * universalDirection * 0.05));

    randomHash = random(hash((aPos.x + 3003), (aPos.z + 23)));

    float tilt = 0.4 + 0.3 * (1 - (sqrt(minDist) / (GRID_SIZE * GRID_SIZE / 2)));
    tilt += (randomHash - 0.5) * 0.4;
    tilt += (sin(time * (0.2 + 0.3 * randomHash) + 10000 * randomHash)) * 0.01 * randomHash;
    tilt += perlinValue * 0.01;


    vec3 grassPosXZ = vec3(facing.x, 0, facing.y) * cos(tilt);
    float grassPosXZDist = length(grassPosXZ);
    tipPosition = vec3(grassPosXZ.x, clumpHeight * sqrt(1 - grassPosXZDist * grassPosXZDist) ,grassPosXZ.z);



    float tiltPointT = 0.76;
    float tiltAmount = 0.65; // TODO influence this
    bezierPoint = (tiltPointT * tipPosition) + (normalize(cross(cross(tipPosition, vec3(0, 1, 0)),tipPosition))) * tiltAmount;

    float clumpAmt = 0.40 * randomHash;
    gl_Position = vec4((1.0 - clumpAmt) * aPos + (clumpAmt) * vec3(minX, aPos.y, minY) , 1.0)
        +1.4 * vec4(random(hash(aPos.x, aPos.z)), 0, random(hash(aPos.x - 2,aPos.z)), 0);
    // gl_Position = vec4(aPos, 1.0);

    // debugColor = vec3(minDist) / (GRID_SIZE * GRID_SIZE / 2);
    


    // CULL
    // ! BAD CODE the tipPosition * X should be ~ segment height
    vec3 grassTip = gl_Position.xyz + tipPosition * 40;
    mat4 viewProjection = projection * view;
    vec4 frustumPlanes[6];
    for (int i = 0; i < 6; i++) {
        frustumPlanes[0][i] = viewProjection[i][3] + viewProjection[i][0];
        frustumPlanes[1][i] = viewProjection[i][3] - viewProjection[i][0];
        frustumPlanes[2][i] = viewProjection[i][3] + viewProjection[i][1];
        frustumPlanes[3][i] = viewProjection[i][3] - viewProjection[i][1];
        frustumPlanes[4][i] = viewProjection[i][3] + viewProjection[i][2];
        frustumPlanes[5][i] = viewProjection[i][3] - viewProjection[i][2];
    }

    for (int i = 0; i < 6; ++i) 
        frustumPlanes[i] = normalize(frustumPlanes[i]);

    float margin = 0.10;
    bool cull = false;
    for (int i = 0; i < 6; ++i) {
        if (dot(frustumPlanes[i].xyz, grassTip) + frustumPlanes[i].w < -margin) {
            cull = true;
            break;
        }
    }
    if(cull){
        randomHash = 0;
    }

}

