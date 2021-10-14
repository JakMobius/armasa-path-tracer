uniform sampler2D u_texture;
out vec4 color;
in vec2 uv;

void main( void ) {
	color = texture(u_texture, uv);
}