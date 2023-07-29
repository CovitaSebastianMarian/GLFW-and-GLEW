#version 330 core

uniform vec2 pos;
uniform float scale;
uniform vec3 color;
void main()
{
    vec2 coord = gl_FragCoord.xy;
    
    vec4 col;
    if(distance(coord, pos) <= scale){
        col = vec4(color, 1.);
    }
    gl_FragColor = col;
} 