#version 410 core

layout(location = 0) in vec2 a_position;

void main( void ) {
	gl_Position = vec4(a_position.xy, 1.0, 1.0);
}
