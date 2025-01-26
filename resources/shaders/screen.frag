#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D frame_buffer;
uniform sampler2D depth_buffer;

uniform vec3 dirLightDirection;     // Light position in view space
uniform mat4 projection;        // Inverse projection matrix
uniform mat4 invProjection;     // Inverse projection matrix

uniform float near;
uniform float far;

// Constants
const int MAX_STEPS = 15;
const float STEP_SIZE = 0.1;

float getDepth(vec2 uv) {
    return texture(depth_buffer, uv).r * 2.0 - 1.0; // -1.0 to 1.0
}

vec3 screenToView(vec2 uv, float depth) {
    vec4 clipSpace = vec4((uv * 2.0 - 1.0), depth, 1.0);
    vec4 viewSpace = invProjection * clipSpace;
    return viewSpace.xyz / viewSpace.w;
}

vec3 viewToScreen(vec3 viewPos) {
    vec4 clipPos = projection * vec4(viewPos, 1.0);
    vec3 projCoords = clipPos.xyz / clipPos.w;
    return vec3(projCoords.xy * 0.5 + 0.5, projCoords.z);
}

void main() {
    float currentDepth = getDepth(TexCoords);
    vec3 currentViewPos = screenToView(TexCoords, currentDepth); // current fragment position in view space

    vec3 lightDir = normalize(dirLightDirection);
    vec3 rayPos = currentViewPos;

    float shadow = 0.0;
    for (int i = 0; i < MAX_STEPS; ++i) {
        rayPos += lightDir * STEP_SIZE;

        vec3 uv = viewToScreen(rayPos);

        if (uv.x < 0.0 || uv.y < 0.0 || uv.x > 1.0 || uv.y > 1.0) {
            break;
        }

        float sceneDepth = getDepth(uv.xy);
        float rayDepth = uv.z;
        if (rayDepth > sceneDepth) {
            shadow = 0.5;
            break;
        }
    }

    // FragColor = vec4(texture(frame_buffer, TexCoords).rgb * (1.0 - shadow), 1.0);
    // FragColor = vec4(vec3(currentDepth), 1.0);
    FragColor = vec4(vec3(1-shadow).xyz, 1.0);
    // FragColor = texture(frame_buffer, TexCoords);
}
