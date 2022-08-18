#version 410 core

layout (location = 0) in vec4 color;
layout (location = 0) out vec4 out_color;

void main() {
    //    color = vec4(1.0, 0.0, 0.0, 1.0);
    out_color = color;
}