#version 330 core

#define INFINITY 5000.0
#define HEIGHT 5000.0
#define STEPS 48
#define SUN_STEPS 6

#define W_INFINITY 100000.0
#define H 0.0001

#define PI 3.14159

#define CLOUD_HEIGHT 1400.0
#define CLOUD_VOLUME 1800.0
#define RADIUS 20000.0
#define OUTER_RADIUS (RADIUS + CLOUD_VOLUME)
// #define 

in vec3 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;
uniform float time; // t
// uniform vec3 lightPos;

uniform samplerCube cube_map;
uniform float day;

uniform mat4 projection;
uniform mat4 view;

uniform sampler3D td_noise;

float random(float seed) {
    return fract(sin(seed) * 843758.5453123);
}
float hash(float a, float b) {
    return sin(a * 12.9898 + b * 78.233) * 43758.5453;
}

float remap(float value, float low1, float high1, float low2, float high2) {
    return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
}
float clamp(float value){
    if (value < 0.0){
        return 0.0;
    }else if (value > 1.0){
        return 1.0;
    }
    return value;
}

float sampleDensity(vec3 samplePoint){
    float perlinWorley = texture(td_noise, vec3((samplePoint.x - time * 0.0005), samplePoint.y * 0.15, samplePoint.z)).r;
    vec4 noiseData2 = texture(td_noise, vec3((samplePoint.x * 1.5 - time * 0.0005), samplePoint.y * 0.15, samplePoint.z * 1.5));

    // SNsample= R(snr, (sng ×0.625 + snb ×0.25 + sna ×0.125)−1, 1, 0, 1)
    float noise = remap(perlinWorley, (noiseData2.g * 0.625 + noiseData2.b * 0.25 + noiseData2.a * 0.125) - 1, 1, 0, 1);

    float gc = 0.29;
    noise = clamp(remap(noise, 1 - gc, 1, 0, 1));

    // if (noise < 0.0001){
    //     noise = 0.0;
    // }else{
    //     noise = 1.0;
    // }


    // SN= SAT (R(SNsample ×SA, 1−gc ×WMc, 1, 0, 1)) ×DA 

    // noise = pow(noise, 0.2);
    // float threshold = 0.82;
    // noise = clamp(remap(noise, 0, 1, threshold, 1));

    // if (noise < threshold){
    //     noise = 0.0;
    // }else{
    // }
    // vec4 a = texture(td_noise, vec3((samplePoint.x - time * 0.0005), samplePoint.y * 0.15, samplePoint.z));
    // return a.a;
    return noise;
}

float f(float x, float y, float t){
    return (sin(sqrt(x * x + y * y) - t) + cos(sqrt((x + 100) * (x + 100) + (2 * y + 100) * (2 * y + 100)) - t) + sin(sqrt((x - 100) * (x - 100) + 0.5 * (y - 50) * (y - 50))) + cos(sqrt(2.4 * (x - 60) * (x - 60) + 1.6 * (y + 88) * (y + 88)))) / 4 + 1;
}

vec3 waveNormal(float x, float y, float t){
    float dx = (f(x + H, y, t) - f(x - H, y, t)) / (2 * H);
    float dy = (f(x, y + H, t) - f(x, y - H, t)) / (2 * H);
    return normalize(vec3(-dx, 1, -dy));
}

vec2 sphereIntersection(vec3 rayOrigin, vec3 rayDir, float sphereRadius, float remapMin, float remapMax){
    float a = dot(rayDir, rayDir);
    float b = 2.0 * dot(rayOrigin, rayDir);
    float c = dot(rayOrigin, rayOrigin) - sphereRadius * sphereRadius;
    float discriminant = b * b - 4.0 * a * c;

    if (discriminant > 0.0) {
        float t0 = (-b - sqrt(discriminant)) / (2.0 * a);
        float t1 = (-b + sqrt(discriminant)) / (2.0 * a);

        float tStart = min(t0, t1);
        if (tStart < 0.0){
            tStart = max(t0, t1);
        }

        vec3 localPos = normalize(rayOrigin + rayDir * tStart);

        float theta = atan(localPos.y, localPos.x);
        float phi = atan(localPos.y, localPos.z);
        return vec2(remap(theta, remapMin, remapMax, 0.0, 1.0), remap(phi, remapMin, remapMax, 0.0, 1.0));
    }
    return vec2(0.0);
}

void main(){    
    vec3 newCoords = TexCoords;

    vec3 originDir = vec3(0.0, 0.0, 0.0);
    if (TexCoords.y < 0.0){
        // Compute intersection with y = 0 plane
        vec3 p1 = viewPos;
        vec3 p2 = (TexCoords) * W_INFINITY; // TODO, idk if you should normalize this
        vec3 dir = p2 - p1;
        float t = -p1.y / dir.y;
        vec3 intersection = p1 + t * dir;

        vec3 n = waveNormal(intersection.x / 30, intersection.z / 30, time / 2);

        vec3 testColor = vec3(0.0, 0.0, 0.0);

        intersection /= W_INFINITY;

        // n = vec3(0.0, 1.0, 0.0);
        // compute intersection with y = 1 plane
        originDir = normalize(reflect(dir, normalize(n + vec3(0.0, 30.0 * (60 * length(intersection) + 1), 0.0))));
    }else{
        vec3 p1 = vec3(0.0, 0.0, 0.0);
        vec3 p2 = TexCoords * INFINITY;
        vec3 dir = normalize(p2 - p1);

        originDir = dir;
    }

    vec3 rayOrigin = vec3(0.0, RADIUS - CLOUD_HEIGHT, 0.0);

    float innerHorizonAngle = atan(
        rayOrigin.y, 
        sqrt(RADIUS * RADIUS - rayOrigin.y * rayOrigin.y)
    );
    float outerHorizonAngle = atan(
        rayOrigin.y,
        sqrt(OUTER_RADIUS * OUTER_RADIUS - rayOrigin.y * rayOrigin.y)
    );


    vec2 lowerIntersection = sphereIntersection(rayOrigin, originDir, RADIUS, innerHorizonAngle, PI - innerHorizonAngle);
    vec2 upperIntersection = sphereIntersection(rayOrigin, originDir, OUTER_RADIUS, outerHorizonAngle, PI - outerHorizonAngle);

    // vec4 noiseData = texture(td_noise, vec3(upperIntersection.x, time / 100, upperIntersection.y));
    // float fbmNoise = remap(noiseData.r, (noiseData.g * 0.625 + noiseData.b * 0.25 + noiseData.a * 0.125) - 1, 1, 0, 1);
    // float gc = 0.42;
    // fbmNoise = clamp(remap(fbmNoise, 1 - gc, 1, 0, 1));
    // float threshold = 0.1;
    // fbmNoise = sampleDensity(vec3(upperIntersection.x, 0.0, upperIntersection.y));
    // FragColor = vec4(vec3(fbmNoise), 1.0);
    // return;

    vec3 samplePoint = vec3(lowerIntersection.x, 0.0, lowerIntersection.y);
    // vec3 dir = normalize(vec3(upperIntersection.x, 1.0, upperIntersection.y) - samplePoint);
    vec3 dir = vec3(upperIntersection.x, 1.0, upperIntersection.y) - samplePoint;
    float stepSize = 1 / float(STEPS);
    float density = 0.0;

    float sunStepSize = 1 / float(SUN_STEPS);
    sunStepSize *= 0.02;

    vec3 backGround = vec3(79.0 / 255.0, 175.0 / 255.0, 226.0 / 255.0) * 0.7;
    // vec3 backGround = vec3(233.0 / 255.0, 172.0 / 255.0, 99.0 / 255.0) * 0.7;

    // vec3 lightPos = vec3(projection * view * vec4(normalize(vec3(cos(time * 0.55), sin(time * 0.55), 0)), 1.0));
    // vec3 lightPos = vec3(cos(time * 0.04), sin(time * 0.04), 0);
    // vec3 lightPos = vec3(cos(time * 0.4) * 1000.0, sin(time * 0.4) * 1000.0, 0.0);
    vec3 lightPos = vec3(0, OUTER_RADIUS * 2, 0.0);
    vec3 sunDir = normalize(lightPos - samplePoint);
    // sunDir = vec3(0.0, 1.0, 0.0);
    // vec3 lightPos = vec3(cos(time * 0.04), sin(time * 0.04), 0);
    // sunDir = vec3(cos(time * 0.1), sin(time * 0.1), 0.0);
    // vec3 sunColor = vec3(1.0, 1.0, 1.0);

    vec3 sunColor = backGround * 2.5;
    if (sunColor.r > 1.0) sunColor.r = 1.0;
    if (sunColor.g > 1.0) sunColor.g = 1.0;
    if (sunColor.b > 1.0) sunColor.b = 1.0;
    // 255,238,140
    sunColor = vec3(255.0 / 255.0, 248.0 / 255.0, 230.0 / 255.0);

    // vec3 sunColor = vec3(0.0, 1.0, 0.0);
    float lightTransmit = 0.0;        
    for (int i = 0; i < STEPS; i++){
        float sampledDensity = sampleDensity(samplePoint); 
        sampledDensity *= stepSize;

        if (sampledDensity > 0.0001){
            vec3 sunSamplePoint = samplePoint;
            float localLightDensity = 0.0;
            for (int j = 0; j < SUN_STEPS; j++){
                sunSamplePoint += sunDir * sunStepSize;
                localLightDensity += sampleDensity(sunSamplePoint);
            }
            localLightDensity *= sunStepSize;
            lightTransmit = lightTransmit * density + localLightDensity * (1.0 - density);
            // lightTransmit += localLightDensity * (1.0 - density);
        }

        density += sampledDensity * 16;
        samplePoint += dir * stepSize;
        if (density > 1.0){
            density = 1.0;
            // FragColor = vec4(1.0, 0.0, 0.0, 1.0);
            // return;
            break;
        }
    }

    float transmittance = exp(-density * 4);
    lightTransmit = exp(-lightTransmit * 32);

    // vec3 backGround = vec3(233.0 / 255.0, 172.0 / 255.0, 99.0 / 255.0);
    // vec3 backGround = vec3(0.0);
    // vec3 cloudColor = mix(vec3(0.1), sunColor, lightTransmit);

    // !
    vec3 cloudColor = mix(sunColor * 0.25, sunColor * 0.75, lightTransmit);

    // cloudColor = sunColor * 0.95;
    // cloudColor = sunDir;

    vec3 color = mix(cloudColor, backGround, transmittance);

    // float blendOut = 1.0 / length(intersection);
    // if(blendOut > 1.0) blendOut = 1.0;

    FragColor = vec4(color, 1.0);
    // FragColor = vec4(mix(backGround, color, blendOut), 1.0);


    // FragColor = vec4(vec3(density), 1.0);
    // FragColor = vec4(backGround * (1.0 - density), 1.0);

    // intersection /= INFINITY;
    // intersection *= 1000.0;
    // FragColor = vec4(vec3(intersection.x / INFINITY * 100, 1.0, intersection.z / INFINITY * 100), 1.0);

    // }else{
    //     FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    // }

    // FragColor = texture(cube_map, newCoords) * day;
}