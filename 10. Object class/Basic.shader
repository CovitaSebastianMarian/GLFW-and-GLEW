#shader vertex
#version 330 core

layout(location = 0) in vec3 positions;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texcoord;

out vec3 fcolor;
out vec2 texc;
uniform mat4 Matrix;

void main()
{
	gl_Position = Matrix * vec4(positions, 1);
	fcolor = color;
	texc = texcoord;
}


#shader fragment
#version 330 core

in vec3 fcolor;
in vec2 texc;
uniform sampler2D Texture;
out vec4 FragColor;

void main()
{
	FragColor = texture(Texture, texc);
	//FragColor = vec4(fcolor, 1.0);
}