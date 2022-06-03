#shader vertex
#version 330 core

layout(location = 0) in vec3 positions;
layout(location = 1) in vec2 tcord;

out vec2 tex;
uniform mat4 Matrix;

void main()
{
	gl_Position = Matrix * vec4(positions, 1.0);
	tex = tcord;
}


#shader fragment
#version 330 core


in vec2 tex;
uniform sampler2D Texture;

void main()
{	
	//gl_FragColor = texture(Texture, tex);
	gl_FragColor = vec4(1, 0.3, 0.7, 1);
}