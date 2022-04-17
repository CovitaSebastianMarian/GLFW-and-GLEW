#shader vertex

#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texc;


uniform mat4 Matrix;
out vec2 texcoord;

void main()
{
	gl_Position = Matrix * vec4(pos, 1);
	texcoord = texc;
}

//##################################################################################################
//##################################################################################################

#shader fragment

#version 330 core

in vec2 texcoord;
out vec4 FragColor;
uniform sampler2D tex;

void main()
{

	FragColor = texture(tex, texcoord);
}