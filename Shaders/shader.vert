#version 330 core

layout(location = 0) in vec3 Pos;
layout(location = 1) in vec2 tc;

out vec2 texc;
void main() {
	gl_Position = vec4(Pos, 1);
	texc = tc;
}