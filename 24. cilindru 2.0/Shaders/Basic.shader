#shader vertex

#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texc;

uniform mat4 Matrix;
out vec2 ctex;

void main()
{
	gl_Position = Matrix * vec4(pos, 1);
	ctex = texc;
}

//##################################################################################################
//##################################################################################################

#shader fragment

#version 330 core

in vec2 ctex;
uniform sampler2D Texture;

void main()
{
	//gl_FragColor = vec4(0.2, 0.4, 1, 1);
	gl_FragColor = texture(Texture, ctex);
}