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


struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 viewPos;
};

uniform Light light;

struct Material {
	sampler2D diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;

void main()
{

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);


	vec3 viewDir = normalize(light.viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);


	vec3 ambient = light.ambient * vec3(texture(material.diffuse, ctex));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, ctex));
	vec3 specular = light.specular * (spec * material.specular);


	vec3 result = ambient + diffuse + specular;
	gl_FragColor = vec4(result, 1.0);

}