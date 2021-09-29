#version 330 core

layout(location = 0) in vec2 a_position;
out vec4 vertex_position;

void main( void ) {
	vertex_position = vec4(a_position.xy, 1.0, 1.0);
}
