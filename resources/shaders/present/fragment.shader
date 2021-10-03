#version 410 core

uniform sampler2D u_texture;
in vec2 uv;
out vec4 color;

void main( void ) {
	color = texture(u_texture, uv);
}