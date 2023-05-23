#version 330 core


in vec2 texc;
uniform sampler2D Texture;
void main() {	

	gl_FragColor = texture(Texture, texc);

}