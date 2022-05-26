#shader vertex
#version 330 core

layout(location = 0) in vec3 positions;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 tcord;

out vec3 fcolor;
out vec2 tex;
uniform mat4 Matrix;

void main()
{
	gl_Position = Matrix * vec4(positions, 1.0);
	fcolor = color;
	tex = tcord;
}


#shader fragment
#version 330 core


in vec3 fcolor;
in vec2 tex;
uniform sampler2D Texture;

void main()
{	
	gl_FragColor = vec4(fcolor, 1);
	//gl_FragColor = texture(Texture, tex) * vec4(fcolor, 0.5);
}