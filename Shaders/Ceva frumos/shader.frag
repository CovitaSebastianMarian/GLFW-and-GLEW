#version 330 core


uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

vec3 palette(float x){
    return vec3(sin(x), sin(x), sin(u_time) * sin(x));
}

void main() {
    vec2 uv = (gl_FragCoord.xy * 2.0 - u_resolution)/u_resolution.y;
    vec2 uv0 = uv;
    vec3 finalColor = vec3(0.0);
    
    for(float i = 0.0;i<3.0;i++){
        uv = fract(uv * 2.0) -0.5;
        
        float d = length(uv);
        vec3 col = palette(length(uv0) + u_time);
        //vec3 col = vec3(0.365,0.163,0.181);
        
        d = sin(d * 8. + u_time)/8.;
        d = abs(d);
        
        d = 0.02 / d;
        
        finalColor += col * d;
        
    }
    gl_FragColor = vec4(finalColor, 1.0);
}