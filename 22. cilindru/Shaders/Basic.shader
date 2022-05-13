#shader vertex

#version 330 core

layout(location = 0) in vec3 pos;

uniform mat4 Matrix;

void main()
{
	gl_Position = Matrix * vec4(pos, 1);
}

//##################################################################################################
//##################################################################################################

#shader fragment

#version 330 core


void main()
{
	gl_FragColor = vec4(0.2, 0.4, 1, 1);
}