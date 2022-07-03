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
uniform vec3 viewPos;
uniform vec3 objectColor;

void main()
{
	float ambientStrength = 0;
	vec3 ambient = ambientStrength * lightColor;


	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;


	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;


	vec3 result = (ambient + diffuse + specular) * objectColor;
	gl_FragColor = vec4(result, 0.1);
}