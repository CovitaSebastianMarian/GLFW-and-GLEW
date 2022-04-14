#shader vertex

#version 330 core

layout(location = 0) in vec3 positions;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 TextCoord;

out vec2 fTextCoord;
out vec3 u_color;
uniform mat4 Matrix;

void main()
{
	gl_Position = Matrix * vec4(positions, 1);
	u_color = color;
	fTextCoord = TextCoord;
}

//##################################################################################################
//##################################################################################################

#shader fragment
#version 330 core

in vec2 fTextCoord;
in vec3 u_color;
out vec4 color;
uniform sampler2D Texture;
out vec4 FragColor;

void main()
{
	FragColor = texture(Texture, fTextCoord);
	//color = vec4(u_color, 1);
}