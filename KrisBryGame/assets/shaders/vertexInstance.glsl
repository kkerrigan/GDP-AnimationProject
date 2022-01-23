#version 330 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in mat4 vInstanceMatrix; // 4 x vec4
layout(location = 5) in vec4 vColour;

out vec4 color;

uniform mat4 projection;
uniform mat4 view;

void main() {

	gl_Position = projection * view * vInstanceMatrix * vec4(vPosition, 1.0);
	color = vColour;
	
	return;
}