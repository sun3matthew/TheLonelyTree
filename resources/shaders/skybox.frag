#version 330 core

#define INFINITY 100000.0

in vec3 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;
uniform float time; // t

uniform samplerCube cube_map;
uniform float day;

// f\left(x,y\right)=\frac{\left(\sin\left(\left(x^{2}+y^{2}\right)^{0.5}-t\right)+\cos\left(\left(\left(x\ +\ 100\right)^{2}+\left(2y+100\right)^{2}\right)^{0.5}-t\right)\right)+\sin\left(\left(\left(x-100\right)^{2}+0.5\left(y-50\right)^{2}\right)^{0.5}\right)+\cos\left(\left(2.4\left(x-60\right)^{2}+1.6\left(y+88\right)^{2}\right)^{0.5}\right)}{4}+1
float waveNormal(float x, float y, float t){
    // return sin(sqrt(x * x + y * y) + t);
    return (sin(sqrt(x * x + y * y) - t) + cos(sqrt((x + 100) * (x + 100) + (2 * y + 100) * (2 * y + 100)) - t) + sin(sqrt((x - 100) * (x - 100) + 0.5 * (y - 50) * (y - 50))) + cos(sqrt(2.4 * (x - 60) * (x - 60) + 1.6 * (y + 88) * (y + 88)))) / 4 + 1;
}

void main(){    
    vec3 newCoords = TexCoords;
    float fog = 0.0;
    float dist = 0.0;
    // if(TexCoords.y < 0.0){
    //     newCoords.y = -TexCoords.y;
    //     FragColor = texture(cube_map, newCoords) * day * 0.9;
    //     return;
    // }

    if (TexCoords.y < 0.0){
        // Compute intersection with y = 0 plane
        vec3 p1 = viewPos;
        vec3 p2 = (TexCoords) * INFINITY; // TODO, idk if you should normalize this
        vec3 dir = p2 - p1;
        float t = -p1.y / dir.y;
        vec3 intersection = p1 + t * dir;
        FragColor = vec4(vec3(waveNormal(intersection.x / 10, intersection.z/ 10, time)), 1.0);
        // FragColor = vec4(TexCoords.x, TexCoords.z, 0, 1.0);
        return;
    }

    // dist = newCoords.y / 0.5;
    // fog = exp(-dist * dist * 40.0);
    // if (fog > 1.0)
    //     fog = 1.0;

    // FragColor = texture(cube_map, newCoords) + vec4(vec3(0.6) * fog, 1.0);
    FragColor = texture(cube_map, newCoords) * day;
}