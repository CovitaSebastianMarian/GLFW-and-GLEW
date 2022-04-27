#shader vertex
#version 330 core

layout(location = 0) in vec3 positions;
layout(location = 1) in vec2 color;

out vec2 fcolor;
uniform mat4 Matrix;

void main()
{
	gl_Position = Matrix * vec4(positions, 1.0);
	fcolor = color;
}


#shader fragment
#version 330 core


in vec2 fcolor;
uniform sampler2D Texture;

void main()
{	
	//gl_FragColor = vec4(fcolor, 1);
	gl_FragColor = texture(Texture, fcolor);
}