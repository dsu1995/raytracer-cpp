#version 330

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform mat4 M2;
in vec3 position;

void main() {
	gl_Position = P * V * M * M2 * vec4(position, 1.0);
}
