#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture0;
uniform vec3 materialColor;
uniform bool useTexture;

void main()
{
    vec3 color;
    if(useTexture)
        color = texture(texture0, TexCoord).rgb;
    else
        color = materialColor;
    
    gl_FragColor = vec4(color, 1.0);
}
