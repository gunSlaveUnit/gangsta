#version 450 core

in vec3 pixel_color;
out vec4 final_color;

void main() {
	final_color = vec4(pixel_color, 1.0f);
}