#version 330 core
precision highp float;

uniform vec2 u_screen_size;

uniform vec3 u_camera_position;
uniform vec3 u_camera_width_vector;
uniform vec3 u_camera_height_vector;
uniform vec3 u_camera_focus;

uniform samplerBuffer u_index_buffer;
uniform samplerBuffer u_float_buffer;

out vec4 color;

const float infinity = 1.0 / 0.0;
const float epsilon = 0.005;
const int max_reflections = 5;

struct HitRecord {
	float dist;
	vec3 normal;
	vec3 point;
};

HitRecord hit_record;
vec3 ray_source;
vec3 ray_direction;

bool intersect_sphere(vec3 ray_source, vec3 ray_direction, vec3 sphere_position, float sphere_radius) {
	vec3 c_o = ray_source - sphere_position;
	float rd_c_o_dot = dot(ray_direction, c_o);
	float sq_c_0_length = dot(c_o, c_o);
	float disc = sphere_radius * sphere_radius - (sq_c_0_length - rd_c_o_dot * rd_c_o_dot);

	if (disc < 0) return false;

	disc = sqrt(disc);

	float b = -dot(ray_direction, c_o);
	float d1 = b - disc;
	float d2 = b + disc;
	float d = -1;

	if (d1 > 0 && (d2 > d1 || d2 < 0)) {
		d = d1;
	} else if (d2 > 0 && (d1 > d2 || d1 < 0)) {
		d = d2;
	} else {
		return false;
	}

	if(d > hit_record.dist) return false;

	vec3 point = ray_source + d * ray_direction;
//	hit_record->mat = material;
//	hit_record->set_normal_orientation(ray.dir);
	hit_record.point = point;
	hit_record.dist = d;
	hit_record.normal = (point - sphere_position) / sphere_radius;

	//if(sq_c_0_length < sphere_radius * sphere_radius) hit_record.normal = -hit_record.normal;

//	get_surface_coords(point, hit_record->surf_x, hit_record->surf_y);
	return true;
}

void trace() {
	int i = 0;
	while(i < max_reflections) {
		hit_record.dist = infinity;

		intersect_sphere(ray_source, ray_direction, vec3(10, 0, 0), 3);
		intersect_sphere(ray_source, ray_direction, vec3(3, 3, -3), 3);
		intersect_sphere(ray_source, ray_direction, vec3(-2, 3, 0), 1);

		if(isinf(hit_record.dist)) break;

		ray_source += ray_direction * hit_record.dist;
		ray_direction -= hit_record.normal * dot(ray_direction, hit_record.normal) * 2;
		ray_direction /= sqrt(ray_direction.x * ray_direction.x + ray_direction.y * ray_direction.y + ray_direction.z * ray_direction.z);
		ray_source += ray_direction * epsilon;

		i++;
	}

	color = vec4(ray_direction.xyz, 1.0);
}

void main( void ) {
	vec2 position = gl_FragCoord.xy / u_screen_size * 2 - vec2(1, 1);
	position.y = -position.y;

	ray_source = u_camera_position;
	ray_direction = u_camera_focus + u_camera_width_vector * position.x + u_camera_height_vector * position.y;
	ray_direction /= sqrt(ray_direction.x * ray_direction.x + ray_direction.y * ray_direction.y + ray_direction.z * ray_direction.z);

	trace();
//	int index = int(gl_FragCoord.x / 100) * 3;
//	color = vec4(
//		texelFetch(u_float_buffer, index).r,
//		texelFetch(u_float_buffer, index + 1).r,
//		texelFetch(u_float_buffer, index + 2).r,
//		1.0
//	);
	//color = vec4(gl_FragCoord.xy / u_screen_size, 0.0, 1.0);
}
