#shader vertex
#version 330 core

layout(location = 0) in vec2 positions;
layout(location = 1) in vec2 textures;

uniform mat4 Matrix;
out vec2 tcoord;

void main()
{
	gl_Position = Matrix * vec4(positions, 0, 1);
	tcoord = textures;
}


#shader fragment
#version 330 core

in vec2 tcoord;
uniform sampler2D tex;

void main()
{	

	gl_FragColor = texture(tex, tcoord);
}