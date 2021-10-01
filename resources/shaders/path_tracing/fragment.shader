#version 330 core
precision highp float;

uniform vec2 u_screen_size;

uniform vec3 u_camera_position;
uniform vec3 u_camera_width_vector;
uniform vec3 u_camera_height_vector;
uniform vec3 u_camera_focus;

uniform isamplerBuffer u_index_buffer;
uniform samplerBuffer u_float_buffer;

out vec4 color;

const float infinity = 1.0 / 0.0;
const float epsilon = 0.005;
const int max_reflections = 5;
const int max_tree_depth = 16;

struct HitRecord {
	float dist;
	vec3 normal;
	vec3 point;
};

HitRecord hit_record;
vec3 ray_source;
vec3 ray_direction;
int hittable_index_stack[max_tree_depth];
int hittable_child_index_stack[max_tree_depth];
int stack_size;

bool intersect_sphere(vec3 sphere_position, float sphere_radius);
void hittable_sphere_hit(int index);
void hittable_list_hit(int index);
void hittable_hit(int index);
void trace_rays();

bool intersect_sphere(vec3 sphere_position, float sphere_radius) {
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

void hittable_sphere_hit(int index) {

	stack_size--;

	vec3 sphere_position = vec3(
		texelFetch(u_float_buffer, index + 1).r,
		texelFetch(u_float_buffer, index + 2).r,
		texelFetch(u_float_buffer, index + 3).r
	);
	float sphere_radius = texelFetch(u_float_buffer, index + 4).r;

	intersect_sphere(sphere_position, sphere_radius);
}

void hittable_list_hit(int index) {

	int stack_index = stack_size - 1;
	int current_child_index = hittable_child_index_stack[stack_index];
	int children_count = int(texelFetch(u_index_buffer, index + 1).r);

	if(current_child_index == children_count) {
		stack_size--;
		return;
	}

	hittable_child_index_stack[stack_index] = current_child_index + 1;

	int first_child_index = index + 2;
	int children_index = int(texelFetch(u_index_buffer, first_child_index + current_child_index).r);

	hittable_index_stack[stack_size] = children_index;
	hittable_child_index_stack[stack_size] = 0;
	stack_size++;
}

void hittable_hit(int index) {
	int hittable_type = int(texelFetch(u_index_buffer, index).r);

	if(hittable_type == 0) {
		hittable_list_hit(index);
	} else if(hittable_type == 1) {
		hittable_sphere_hit(index);
	}
}

void trace_rays() {

	int reflections = 0;
	while(reflections++ < max_reflections) {

		hit_record.dist = infinity;
		hittable_index_stack[0] = 0;
		hittable_child_index_stack[0] = 0;
		stack_size = 1;

		int tree_steps = 0;
		while(stack_size > 0 && tree_steps < 16) {
			hittable_hit(hittable_index_stack[stack_size - 1]);
		}

		if(stack_size != 0) {
			// Error state
			color = vec4(1.0, 0.0, 0.0, 1.0);
			return;
		}

		if(isinf(hit_record.dist)) {
			// Didn't hit anything
			color = vec4(ray_direction.xyz, 1.0);
			return;
		}

		ray_source += ray_direction * hit_record.dist;
		ray_direction -= hit_record.normal * dot(ray_direction, hit_record.normal) * 2;
		ray_direction /= sqrt(ray_direction.x * ray_direction.x + ray_direction.y * ray_direction.y + ray_direction.z * ray_direction.z);
		ray_source += ray_direction * epsilon;
	}

	// Reflection limit exceeded
	color = vec4(0, 0, 0, 0);
}

void main( void ) {
	vec2 position = gl_FragCoord.xy / u_screen_size * 2 - vec2(1, 1);
	position.y = -position.y;

	ray_source = u_camera_position;
	ray_direction = u_camera_focus + u_camera_width_vector * position.x + u_camera_height_vector * position.y;
	ray_direction /= sqrt(ray_direction.x * ray_direction.x + ray_direction.y * ray_direction.y + ray_direction.z * ray_direction.z);

	trace_rays();

//	int index = int(gl_FragCoord.x / 100);
//	if(index > 10) index = 0;
//	index = int(texelFetch(u_index_buffer, index).r);
//
//	color = vec4(
//		texelFetch(u_float_buffer, index).r,
//		texelFetch(u_float_buffer, index).r,
//		texelFetch(u_float_buffer, index).r,
//		1.0
//	);

//	color = vec4(1.0, 0.0, 0.0, 1.0);
}
