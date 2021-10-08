#version 410 core

uniform sampler2D u_texture;
uniform float u_gamma;
uniform float u_brightness;
in vec2 uv;
out vec4 color;

void main( void ) {
	color = texture(u_texture, uv);

	color *= u_brightness;

	color.r = clamp(color.r, 0, 1);
	color.g = clamp(color.g, 0, 1);
	color.b = clamp(color.b, 0, 1);

	color.r = pow(color.r, u_gamma);
	color.g = pow(color.g, u_gamma);
	color.b = pow(color.b, u_gamma);
}
