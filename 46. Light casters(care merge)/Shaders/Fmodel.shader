#version 330 core

in vec2 TexCoords;
in vec3 normal;
in vec3 FragPos;


struct Light {
	vec3 lightPos;
	vec3 lightColor;
	vec3 viewPos;
};

uniform Light light;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;

void main()
{
	float ambientStrength = 0;
	vec3 ambient = ambientStrength * material.ambient;


	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.lightColor * (diff * material.diffuse);


	vec3 viewDir = normalize(light.viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.lightColor * (spec * material.specular);


	vec3 result = ambient + diffuse + specular;
	gl_FragColor = vec4(result, 1);


}