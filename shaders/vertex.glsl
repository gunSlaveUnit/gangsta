#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 vertex_color;

out vec3 pixel_color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
	gl_Position = projection * view * model * vec4(position, 1.0);
	pixel_color = vertex_color;
}