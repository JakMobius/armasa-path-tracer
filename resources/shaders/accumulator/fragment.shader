#version 410 core

uniform sampler2D u_input_texture;
uniform sampler2D u_old_texture;
uniform int u_frame;
in vec2 uv;
out vec4 color;

void main( void ) {
	color = (texture(u_input_texture, uv) + texture(u_old_texture, uv) * float(u_frame - 1)) / float(u_frame);
}
