#shader vertex

#version 330 core

layout(location = 0) in vec3 pos;

uniform float size;
uniform mat4 Matrix;

void main()
{
	gl_Position = Matrix * vec4(size * pos.x, size * pos.y, size * pos.z, 1);
}

//##################################################################################################
//##################################################################################################

#shader fragment

#version 330 core

uniform vec4 color;

void main()
{

	gl_FragColor = color;
}