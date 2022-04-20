#shader vertex
#version 330 core

layout(location = 0) in vec3 positions;
layout(location = 1) in vec4 color;

out vec4 fcolor;

void main()
{
	gl_Position = vec4(positions, 1);
	fcolor = color;
}


#shader fragment
#version 330 core

in vec4 fcolor;
out vec4 FragColor;


void main()
{
	FragColor = fcolor;
}