#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 vertex_color;

out vec3 pixel_color;

void main() {
	gl_Position = vec4(position, 1.0);
	pixel_color = vertex_color;
}