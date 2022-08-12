#version 410 core

layout(location = 0) in vec3 pos;
layout(location = 0) out vec4 color;

uniform mat4 u_projection;

void main() {
    vec4 projected = u_projection * vec4(pos, 1.0f);
    gl_Position = projected;
    if (projected.z == 0) {
        color = vec4(1, 1, 1, 1);
    } else {
        color = vec4(1 - (projected.zzz + 1) / 2, 1);
    }
}