#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture0;
uniform vec3 materialColor;
uniform bool useTexture;

// Parametrii pentru iluminare
uniform vec3 lightPos;   // Poziția sursei de lumină (ex. o lumină punctuală)
uniform vec3 lightColor; // Culoarea sursei de lumină
uniform vec3 viewPos;    // Poziția camerei (pentru componenta speculară)


void main()
{
    // Obține culoarea de bază: din textură, dacă este folosită, altfel din materialColor.
    vec3 baseColor = useTexture ? texture(texture0, TexCoord).rgb : materialColor;

    // Ambient: o lumină ambientală constantă
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuz: depinde de unghiul dintre normală și direcția luminii
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular: se bazează pe reflectarea luminii și poziția camerei
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Combină componentele de iluminare și aplică pe culoarea de bază
    vec3 result = (ambient + diffuse + specular) * baseColor;
    gl_FragColor = vec4(result, 1.0);
}
