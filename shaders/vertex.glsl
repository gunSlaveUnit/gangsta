#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 vertex_color;

out vec3 pixel_color;

uniform vec3 offset;
uniform mat4 MVP;

void main() {
	gl_Position = MVP * vec4(position, 1.0);
	pixel_color = vertex_color;
}