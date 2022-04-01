#shader vertex
#version 330 core

layout(location = 0) in vec3 positions;
layout(location = 1) in vec3 color;

out vec3 fcolor;
uniform mat4 Matrix;

void main()
{
	gl_Position = Matrix * vec4(positions, 1.0);
	fcolor = color;
}


#shader fragment
#version 330 core

in vec3 fcolor;
out vec4 outcolor;

void main()
{
	outcolor = vec4(fcolor, 1);
}