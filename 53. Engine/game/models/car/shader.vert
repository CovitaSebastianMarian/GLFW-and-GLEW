#version 330 core

layout (location = 0) in vec3 position;  // Poziția vertex-ului
layout (location = 1) in vec2 texCoord;  // Coordonatele de textură
layout (location = 2) in vec3 normal;    // Normalele vertex-ului

out vec2 TexCoord;  // Coordonatele de textură pentru fragment shader
out vec3 FragPos;   // Poziția fragmentului pentru iluminare
out vec3 Normal;    // Normalul fragmentului pentru iluminare

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(position, 1.0));  // Poziția în spațiul lumii
    Normal = mat3(transpose(inverse(model))) * normal;  // Normalizarea în spațiul lumii
    TexCoord = texCoord;

    gl_Position = projection * view * vec4(FragPos, 1.0);  // Transformarea în coordonate de clip
}
