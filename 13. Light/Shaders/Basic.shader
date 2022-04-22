#shader vertex
#version 330 core

layout(location = 0) in vec3 positions;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texcoord;
layout(location = 3) in vec3 aNormal;

out vec3 fcolor;
out vec2 texc;
uniform mat4 Matrix;
uniform vec3 lightcolor;
out vec3 light;

out vec3 Normal;
out vec3 crntPos;

void main()
{
	crntPos = vec3(Matrix * vec4(positions, 1.0));
	gl_Position = Matrix * vec4(positions, 1.0);
	fcolor = color;
	texc = texcoord;
	light = lightcolor;
	Normal = aNormal;
}


#shader fragment
#version 330 core

in vec3 light;
in vec2 texc;
in vec3 fcolor;
out vec4 FragColor;
uniform sampler2D Texture;
uniform vec3 lightPos;

in vec3 Normal;
in vec3 crntPos;

void main()
{ 
	float ambient = 0.2;
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);

	float diff = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = diff * light;

	vec3 result = ambient + diffuse;

	//FragColor = vec4(result, 1);

	FragColor = texture(Texture, texc) * vec4(result, 1);

}