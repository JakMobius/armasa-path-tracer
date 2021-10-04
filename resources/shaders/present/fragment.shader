#version 410 core

uniform sampler2D u_texture;
uniform float u_frames;
uniform float u_gamma;
uniform float u_brightness;
in vec2 uv;
out vec4 color;

void main( void ) {
	color = texture(u_texture, uv) / u_frames;

	//	if(color.r > 1) color.r = 1;
	//	if(color.g > 1) color.g = 1;
	//	if(color.b > 1) color.b = 1;

	color.r = pow(color.r, u_gamma);
	color.g = pow(color.g, u_gamma);
	color.b = pow(color.b, u_gamma);

	color *= u_brightness;
}
