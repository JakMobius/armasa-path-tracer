#version 410 core
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
const float epsilon = 0.0001;
const int max_reflections = 5;
const int max_tree_depth = 16;

const int HITTABLE_LIST_TYPE = 0;
const int HITTABLE_SPHERE_TYPE = 1;
const int HITTABLE_TRIANGLE_TYPE = 2;

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

bool intersect_triangle(vec3 point_a, vec3 point_b, vec3 point_c);
bool intersect_sphere(vec3 sphere_position, float sphere_radius);

void hittable_triangle_hit(int index);
void hittable_sphere_hit(int index);
void hittable_list_hit(int index);
void hittable_hit(int index);

void trace_rays();

bool intersect_triangle(vec3 point_a, vec3 point_b, vec3 point_c) {
	vec3 edge1 = point_b - point_a;
	vec3 edge2 = point_c - point_a;
	vec3 h = cross(ray_direction, edge2);
	float a = dot(edge1, h);

	if (a > -epsilon && a < epsilon) {
		return false; // This ray is parallel to this triangle.
	}

	float f = 1.0 / a;
	vec3 s = ray_source - point_a;
	float u = f * dot(s, h);

	if (u < 0.0 || u > 1.0) return false;

	vec3 q = cross(s, edge1);
	float v = f * dot(ray_direction, q);

	if (v < 0.0 || u + v > 1.0) return false;

	// At this stage we can compute t to find out where the intersection point is on the line.

	float t = f * dot(edge2, q);

	if (t > epsilon && t < hit_record.dist) // ray intersection
	{
		hit_record.dist = t;
		hit_record.point = ray_source + ray_direction * t;
		//hit_record->mat = material;
		hit_record.normal = cross(edge1, edge2);
		hit_record.normal /= length(hit_record.normal);

//		hit_record->set_normal_orientation(ray_direction);
//		hit_record->front_hit = true;
//		hit_record->surf_x = 0;
//		hit_record->surf_y = 0;
		return true;
	} else {
		// This means that there is a line intersection but not a ray intersection.
		return false;
	}
}

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
	//	hit_record->set_normal_orientation(ray_direction);
	hit_record.point = point;
	hit_record.dist = d;
	hit_record.normal = (point - sphere_position) / sphere_radius;

	//	get_surface_coords(point, hit_record->surf_x, hit_record->surf_y);
	return true;
}

void hittable_triangle_hit(int index) {
	stack_size--;
	vec3 point_a = vec3(
		texelFetch(u_float_buffer, index + 1).r,
		texelFetch(u_float_buffer, index + 2).r,
		texelFetch(u_float_buffer, index + 3).r
	);
	vec3 point_b = vec3(
		texelFetch(u_float_buffer, index + 4).r,
		texelFetch(u_float_buffer, index + 5).r,
		texelFetch(u_float_buffer, index + 6).r
	);
	vec3 point_c = vec3(
		texelFetch(u_float_buffer, index + 7).r,
		texelFetch(u_float_buffer, index + 8).r,
		texelFetch(u_float_buffer, index + 9).r
	);

	intersect_triangle(point_a, point_b, point_c);
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

	switch(hittable_type) {
		case HITTABLE_LIST_TYPE:     hittable_list_hit(index);     break;
		case HITTABLE_SPHERE_TYPE:   hittable_sphere_hit(index);   break;
		case HITTABLE_TRIANGLE_TYPE: hittable_triangle_hit(index); break;
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
		ray_direction /= length(ray_direction);
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
	ray_direction /= length(ray_direction);

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
