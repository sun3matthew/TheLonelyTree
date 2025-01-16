#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

layout (location = 2) in vec3 aInstancedPosition; // Per-instance position


uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;
out vec3 BaseColor;

void main(){
    mat4 model = mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        aInstancedPosition.x, aInstancedPosition.y, aInstancedPosition.z, 1.0
    );

    // gl_Position = projection * view * model * vec4(aPos, 1.0);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    FragPos = vec3(model * vec4(aPos, 1.0));
    BaseColor = vec3(aInstancedPosition.x / 200, aInstancedPosition.y / 100, aInstancedPosition.z / 100);
}

