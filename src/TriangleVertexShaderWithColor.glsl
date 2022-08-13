#version 410 core

layout(location = 0) in vec3 pos;
layout(location = 1) in float in_color;
layout(location = 0) out vec4 color;

uniform mat4 u_MVP;

void main() {
    vec4 projected = u_MVP * vec4(pos, 1.0f);
    gl_Position = projected;
    color = vec4(in_color,in_color,in_color, 1.0);
}