#version 330 core

#define INFINITY 100000.0
#define HEIGHT 10000.0
#define STEPS 80
// #define 

in vec3 TexCoords;

out vec4 FragColor;

// uniform vec3 viewPos;
uniform float time; // t
// uniform vec3 lightPos;

uniform samplerCube cube_map;
uniform float day;

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
    float noise = texture(td_noise, vec3(samplePoint.x + time * 0.02, samplePoint.y, samplePoint.z)).r;
    if (noise < 0.5){
        noise = 0.0;
    }else{
        // noise = 1.0;
        noise *= 2.0;
        
    }
    return noise;
}

void main(){    
    vec3 newCoords = TexCoords;

    if (TexCoords.y < 0.0){
        newCoords.y = -1.0 * TexCoords.y;
    }

    // if (TexCoords.y > 0.0){
        // Compute intersection with y = height plane
        // vec3 p1 = viewPos;
        // vec3 p1 = vec3(0.0, 0.0, 0.0);
        // vec3 p2 = (TexCoords) * INFINITY;
        // vec3 dir = p2 - p1;
        // float t = (HEIGHT - p1.y) / dir.y;
        // vec3 intersection = p1 + t * dir;

        vec3 p2 = (newCoords) * INFINITY;
        float t = (HEIGHT) / p2.y;
        vec3 intersection = t * p2;

        intersection /= INFINITY;
        // intersection *= 4.0;


        vec3 samplePoint = intersection;
        float stepSize = 1 / float(STEPS);
        float density = 0.0;
        vec3 lightPos = normalize(vec3(1.0, 1.0, 1.0));
        for (int i = 0; i < STEPS; i++){
            vec3 samplePointLight = samplePoint;


            density += sampleDensity(samplePoint);
            samplePoint += normalize(p2) * stepSize;
        }

        density /= float(STEPS);
        density *= 2;
        // density = remap(density, 0.3, 1.0, 0.0, 0.1);

        float transmittance = exp(-density);

        vec3 backGround = vec3(233.0 / 255.0, 172.0 / 255.0, 99.0 / 255.0);
        vec3 cloudColor = vec3(0.9, 0.9, 0.9);

        backGround = mix(cloudColor, backGround, transmittance);

        FragColor = vec4(backGround, 1.0);


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