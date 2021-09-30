#version 330 core

uniform vec2 u_screen_size;
out vec4 color;

void main( void ) {
	color = vec4(gl_FragCoord.xy / u_screen_size, 0.0, 1.0);
}