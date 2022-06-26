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
uniform vec3 lightPos;
uniform vec3 lightColor;

void main()
{
	//vec3 lightPos = vec3(2, 2, 2);
	//vec3 lightColor = vec3(1, 1, 1);
	//vec3 objectColor = vec3(0.3, 0.5, 0.7);
	

	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;


	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 result = (ambient + diffuse);
	gl_FragColor = vec4(result, 1.0) * texture(Texture, ctex);

}