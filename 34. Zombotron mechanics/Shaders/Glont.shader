#shader vertex
#version 330 core

layout(location = 0) in vec2 positions;

uniform mat4 Matrix;


void main()
{
	gl_Position = Matrix * vec4(positions, 1, 1);

}


#shader fragment
#version 330 core



uniform sampler2D tex;

void main()
{
	gl_FragColor = vec4(255, 210, 0, 1);
}