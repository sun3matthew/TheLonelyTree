#version 330 core

#define INFINITY 2000.0
#define HEIGHT 200.0
#define STEPS 40
#define SUN_STEPS 4

#define W_INFINITY 100000.0
#define H 0.0001
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

float sampleDensity(vec3 samplePoint){
    float noise = texture(td_noise, vec3((samplePoint.x - time * 0.005) * 0.5, samplePoint.y, samplePoint.z * 0.5)).r;
    if (noise < 0.55){
        noise = 0.0;
    }else{
        // noise = 1.0;
        noise *= (noise - 0.55) * 84;
        
    }
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

void main(){    
    vec3 newCoords = TexCoords;

    // if (TexCoords.y < 0.0){
    //     newCoords.y = -1.0 * TexCoords.y;
    // }

    vec3 originRay = vec3(0.0, 0.0, 0.0);
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
        originRay = intersection;
        originDir = normalize(reflect(dir, normalize(n + vec3(0.0, 30.0 * (60 * length(intersection) + 1), 0.0))));
    }else{
        vec3 p1 = vec3(0.0, 0.0, 0.0);
        vec3 p2 = TexCoords * INFINITY;
        vec3 dir = normalize(p2 - p1);

        originRay = p1;
        originDir = dir;
    }

    // if (TexCoords.y > 0.0){
        // Compute intersection with y = height plane
        // vec3 p1 = viewPos;
        vec3 p1 = originRay;
        vec3 dir = originDir;
        float t = (HEIGHT - p1.y) / dir.y;
        vec3 intersection = p1 + t * dir;

        // vec3 p2 = (newCoords) * INFINITY;
        // float t = (HEIGHT) / p2.y;
        // vec3 intersection = t * p2;

        intersection /= INFINITY;



        // intersection *= 4.0;


        vec3 samplePoint = intersection;
        float stepSize = 1 / float(STEPS);
        float density = 0.0;

        float sunStepSize = 1 / float(SUN_STEPS);
        sunStepSize *= 0.08;

        vec3 backGround = vec3(233.0 / 255.0, 172.0 / 255.0, 99.0 / 255.0) * 0.7;
        // vec3 lightPos = vec3(projection * view * vec4(normalize(vec3(cos(time * 0.55), sin(time * 0.55), 0)), 1.0));
        vec3 lightPos = vec3(cos(time * 0.04), sin(time * 0.04), 0);
        // vec3 sunColor = vec3(1.0, 1.0, 1.0);

        vec3 sunColor = backGround * 2.5;
        if (sunColor.r > 1.0) sunColor.r = 1.0;
        if (sunColor.g > 1.0) sunColor.g = 1.0;
        if (sunColor.b > 1.0) sunColor.b = 1.0;

        // vec3 sunColor = vec3(0.0, 1.0, 0.0);
        float lightTransmit = 0.0;        
        for (int i = 0; i < STEPS; i++){
            vec3 sunSamplePoint = samplePoint;

            float localLightDensity = 0.0;
            for (int j = 0; j < SUN_STEPS; j++){
                localLightDensity += sampleDensity(sunSamplePoint);
                sunSamplePoint += normalize(lightPos) * sunStepSize;
            }
            localLightDensity = localLightDensity / float(SUN_STEPS);
            lightTransmit += exp(-localLightDensity) / float(STEPS);

            density += sampleDensity(samplePoint) / float(STEPS) * 4;
            samplePoint += normalize(dir) * stepSize;
        }

        float transmittance = exp(-density);

        // vec3 backGround = vec3(233.0 / 255.0, 172.0 / 255.0, 99.0 / 255.0);
        // vec3 backGround = vec3(0.0);
        // vec3 cloudColor = mix(vec3(0.1), sunColor, lightTransmit);
        vec3 cloudColor = mix(backGround * 0.6, sunColor, lightTransmit);

        vec3 color = mix(cloudColor, backGround, transmittance);

        float blendOut = 1.0 / length(intersection);
        if(blendOut > 1.0) blendOut = 1.0;

        // FragColor = vec4(color, 1.0);
        FragColor = vec4(mix(backGround, color, blendOut), 1.0);


        // FragColor = vec4(vec3(density), 1.0);
        // FragColor = vec4(backGround * (1.0 - density), 1.0);


        // intersection /= INFINITY;
        // intersection *= 1000.0;
        // float noise = texture(td_noise, vec3(intersection.x, time / 100, intersection.z)).r;
        // FragColor = vec4(vec3(noise), 1.0);

        // FragColor = vec4(vec3(intersection.x / INFINITY * 100, 1.0, intersection.z / INFINITY * 100), 1.0);

    // }else{
    //     FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    // }

    // FragColor = texture(cube_map, newCoords) * day;
}