#version 410 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 in_color;
layout (location = 0) out vec4 color;

uniform mat4 u_MVP;
uniform vec4 u_light_color;
uniform vec3 u_light_pos;

void main() {
    vec4 projected = u_MVP * vec4(pos, 1.0f);
    gl_Position = projected;
    float dist = abs(1 / distance(projected.xyz, u_light_pos));
    color = vec4(in_color.xyz * dist, 1.0);
}