#version 330 core

#define INFINITY 100000.0
#define H 0.0001

in vec3 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;
uniform float time; // t

uniform samplerCube cube_map;
uniform float day;

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
    float fog = 0.0;
    float dist = 0.0;

    if (TexCoords.y < 0.0){
        // Compute intersection with y = 0 plane
        vec3 p1 = viewPos;
        vec3 p2 = (TexCoords) * INFINITY; // TODO, idk if you should normalize this
        vec3 dir = p2 - p1;
        float t = -p1.y / dir.y;
        vec3 intersection = p1 + t * dir;

        vec3 n = waveNormal(intersection.x / 30, intersection.z / 30, time / 2);

        vec3 testColor = vec3(0.0, 0.0, 0.0);

        intersection /= INFINITY;

        // n = vec3(0.0, 1.0, 0.0);
        // compute intersection with y = 1 plane
        bool found = false;
        p1 = intersection;
        dir = normalize(reflect(dir, normalize(n + vec3(0.0, 30.0 * (60 * length(intersection) + 1), 0.0))));

        t = (1 - p1.y) / dir.y;
        intersection = p1 + t * dir;
        if (intersection.x >= -1 && intersection.x <= 1 && intersection.z >= -1 && intersection.z <= 1){
            newCoords = intersection;
            found = true;
        }

        // compute intersection with x = 1 plane
        t = (1 - p1.x) / dir.x;
        intersection = p1 + t * dir;
        if (intersection.y >= 0 && intersection.y <= 1 && intersection.z >= -1 && intersection.z <= 1 && !found){
            newCoords = intersection;
            found = true;
        }

        // compute intersection with x = -1 plane
        t = (-1 - p1.x) / dir.x;
        intersection = p1 + t * dir;
        if (intersection.y >= 0 && intersection.y <= 1 && intersection.z >= -1 && intersection.z <= 1 && !found){
            newCoords = intersection;
            found = true;
        }

        // compute intersection with z = 1 plane
        t = (1 - p1.z) / dir.z;
        intersection = p1 + t * dir;
        if (intersection.y >= 0 && intersection.y <= 1 && intersection.x >= -1 && intersection.x <= 1 && !found){
            newCoords = intersection;
            found = true;
        }

        // compute intersection with z = -1 plane
        t = (-1 - p1.z) / dir.z;
        intersection = p1 + t * dir;
        if (intersection.y >= 0 && intersection.y <= 1 && intersection.x >= -1 && intersection.x <= 1 && !found){
            newCoords = intersection;
            found = true;
        }

    }

    FragColor = texture(cube_map, newCoords) * day;
}