#shader vertex

#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texc;
layout(location = 2) in vec3 nor;

uniform mat4 Matrix;
uniform mat4 Model;
out vec2 ctex;
out vec3 Normal;
out vec3 FragPos;


void main()
{
	gl_Position = Matrix * vec4(pos, 1);
	FragPos = vec3(Model * vec4(pos, 1));
	ctex = texc;
	Normal = nor;
}

//##################################################################################################
//##################################################################################################

#shader fragment

#version 330 core

in vec2 ctex;
in vec3 Normal;
in vec3 FragPos;
uniform sampler2D Texture;


void main()
{
	gl_FragColor = texture(Texture, ctex);
}