#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
    //normal = mat3(transpose(inverse(model))) * aNormal;
    normal = aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));
}