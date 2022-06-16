#shader vertex
#version 330 core

layout(location = 0) in vec3 positions;
layout(location = 1) in vec2 textures;
layout(location = 2) in vec3 normals;

uniform mat4 Matrix;

out vec2 tcoord;
out vec3 nor;


void main()
{
	gl_Position = Matrix * vec4(positions, 1.0);
	nor = normals;
	tcoord = textures;
}


#shader fragment
#version 330 core

in vec2 tcoord;
in vec3 nor;

uniform sampler2D tex;

void main()
{	
	gl_FragColor = texture(tex, tcoord);
	//gl_FragColor = vec4(0.4, 0.5, 0.8, 1);
}