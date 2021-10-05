// New

#version 410 core
precision highp float;

uniform vec2 u_screen_size;

uniform vec3 u_camera_position;
uniform vec3 u_camera_width_vector;
uniform vec3 u_camera_height_vector;
uniform vec3 u_camera_focus;

uniform isamplerBuffer u_index_buffer;
uniform samplerBuffer u_float_buffer;
uniform samplerBuffer u_random_buffer;
uniform samplerBuffer u_random_unit_buffer;

uniform int u_entry_index;
uniform int u_random_buffer_length;
uniform int u_samples;
uniform int u_max_reflections;
uniform uint u_seed;

layout(location = 0) out vec3 color;
out vec3 temp_color;

const float pi = 3.1415926536;
const float pi_2 = 6.2831853072;

const float infinity = 1.0 / 0.0;
const float epsilon = 0.0001;
const int max_stack_size = 16;

const int HITTABLE_LIST_TYPE = 0;
const int HITTABLE_SPHERE_TYPE = 1;
const int HITTABLE_TRIANGLE_TYPE = 2;

const int MATERIAL_METAL = 0;
const int MATERIAL_LAMBERTIAN = 1;
const int MATERIAL_LAMBERTIAN_LIGHT = 2;

struct HitRecord {
	float dist;
	vec3 normal;
	vec3 point;
	int material;
};

HitRecord hit_record;
vec3 ray_source;
vec3 ray_direction;
int hittable_index_stack[max_stack_size];
int hittable_child_index_stack[max_stack_size];
int stack_size;

bool intersect_triangle(vec3 point_a, vec3 point_b, vec3 point_c);
bool intersect_sphere(vec3 sphere_position, float sphere_radius);

void hittable_triangle_hit(int index);
void hittable_sphere_hit(int index);
void hittable_list_hit(int index);
void hittable_hit(int index);

bool material_lambertian_reflect(bool has_light);
bool material_metal_reflect();

bool material_reflect(int index);
void trace_rays();

uniform float time;
out vec4 fragment;

//uniform uint u_rand_index;
int rand_index;

// A single iteration of Bob Jenkins' One-At-A-Time hashing algorithm.
uint hash(uint x) {
	x += (x << 10u);
	x ^= (x >>  6u);
	x += (x <<  3u);
	x ^= (x >> 11u);
	x += (x << 15u);
	return x;
}

vec3 random() {
	if(rand_index >= u_random_buffer_length) rand_index = 0;
	return texelFetch(u_random_buffer, rand_index++).xyz;
}

vec3 random_unit_vec3() {
	vec3 r = random();
	float a = r.x * pi_2;
	vec2 u = vec2(cos(a), sin(a));
	float s = r.y * 2 - 1;
	float c = sqrt(1 - s * s);
	return vec3(c * u, s);
}

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
		hit_record.normal = cross(edge1, edge2);
		hit_record.normal /= length(hit_record.normal);

		if(dot(hit_record.normal, ray_direction) > 0) hit_record.normal = -hit_record.normal;

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

	//	hit_record->set_normal_orientation(ray_direction);
	hit_record.point = point;
	hit_record.dist = d;
	hit_record.normal = (point - sphere_position) / sphere_radius;

	if(dot(hit_record.normal, ray_direction) > 0) hit_record.normal = -hit_record.normal;

	//	get_surface_coords(point, hit_record->surf_x, hit_record->surf_y);
	return true;
}

void hittable_triangle_hit(int index) {
	stack_size--;

	vec3 point_a = texelFetch(u_float_buffer, index).xyz;
	vec3 point_b = texelFetch(u_float_buffer, index + 1).xyz;
	vec3 point_c = texelFetch(u_float_buffer, index + 2).xyz;

	if(intersect_triangle(point_a, point_b, point_c)) {
		hit_record.material = texelFetch(u_index_buffer, index).g;
	}
}

void hittable_sphere_hit(int index) {
	stack_size--;

	vec4 sphere_data = texelFetch(u_float_buffer, index).rgba;

	if(intersect_sphere(sphere_data.xyz, sphere_data.w)) {
		hit_record.material = texelFetch(u_index_buffer, index).g;
	}
}

bool aabb_hit(vec3 aabb_lower, vec3 aabb_upper) {
	vec3 invDv = 1 / ray_direction;
	vec3 t0v = (aabb_lower - ray_source) * invDv;
	vec3 t1v = (aabb_upper - ray_source) * invDv;
	float t_min = 0;
	float t_max = hit_record.dist;

	for(int i = 0; i < 3; i++) {
		float t0 = t0v[i], t1 = t1v[i], t2 = t1;

		if(invDv[i] < 0) {
			t1 = t0;
			t0 = t2;
		}

		t_min = max(t_min, t0);
		t_max = min(t_max, t1);
	}

	return t_max > t_min;
}

void hittable_list_hit(int index) {
	int stack_index = stack_size - 1;
	int current_child_index = hittable_child_index_stack[stack_index];

	// If we've entered the hittable list, check its aabb
	if(current_child_index == 0) {
		vec3 aabb_lower = texelFetch(u_float_buffer, index).xyz;
		vec3 aabb_upper = texelFetch(u_float_buffer, index + 1).xyz;

		if(!aabb_hit(aabb_lower, aabb_upper)) {
			stack_size--;
			return;
		}
	}

	int children_count = texelFetch(u_index_buffer, index).g;

	if(current_child_index == children_count) {
		stack_size--;
		return;
	}

	hittable_child_index_stack[stack_index] = current_child_index + 1;

	int first_child_index = (index << 2) + 2;
	int children_pointer = first_child_index + current_child_index;
	int children_index = texelFetch(u_index_buffer, children_pointer >> 2)[children_pointer & 3];

	hittable_index_stack[stack_size] = children_index;
	hittable_child_index_stack[stack_size] = 0;
	stack_size++;
}

void material_metal_reflect(int index) {
	vec4 material_color = texelFetch(u_float_buffer, index);
	float fuzziness = material_color.a;

	temp_color *= material_color.rgb;
	ray_direction -= hit_record.normal * dot(ray_direction, hit_record.normal) * 2;

	vec3 random_vec = random();

	ray_direction += fuzziness * random_vec;

	ray_direction /= length(ray_direction);
	float projection = dot(ray_direction, hit_record.normal);

	if(projection < 0) {
		ray_direction -= hit_record.normal * projection * 2;
	}

	ray_direction /= length(ray_direction);
}

bool material_lambertian_reflect(int index, bool has_light) {
	vec3 material_color = texelFetch(u_float_buffer, index).rgb;

	temp_color *= material_color;

	if(has_light) return true;

	ray_direction = random_unit_vec3();
	float projection = dot(ray_direction, hit_record.normal);
	if(projection < 0) ray_direction = -ray_direction;
	ray_direction /= length(ray_direction);

	return false;
}

void hittable_hit(int index) {
	int hittable_type = texelFetch(u_index_buffer, index).r;

	switch(hittable_type) {
		case HITTABLE_LIST_TYPE:     hittable_list_hit(index);     break;
		case HITTABLE_SPHERE_TYPE:   hittable_sphere_hit(index);   break;
		case HITTABLE_TRIANGLE_TYPE: hittable_triangle_hit(index); break;
		default: stack_size--;
	}
}

bool material_reflect(int index) {
	int material_type = texelFetch(u_index_buffer, index).r;

	switch(material_type) {
		case MATERIAL_METAL: 	  		material_metal_reflect(index); 				return false;
		case MATERIAL_LAMBERTIAN: 		material_lambertian_reflect(index, false);  return false;
		case MATERIAL_LAMBERTIAN_LIGHT: material_lambertian_reflect(index, true); 	return true;
	}

	return false;
}

void trace_rays() {
	temp_color = vec3(1, 1, 1);
	int reflections = 0;
	while(reflections++ < u_max_reflections) {

		hit_record.dist = infinity;
		hittable_index_stack[0] = u_entry_index;
		hittable_child_index_stack[0] = 0;
		stack_size = 1;

		int tree_steps = 0;
		while(stack_size > 0) {
			if(stack_size >= max_stack_size) break;
			hittable_hit(hittable_index_stack[stack_size - 1]);
			tree_steps++;
		}

		if(stack_size != 0) {
			// Error state
			temp_color = vec3(1, 0, 0);
			return;
		}

		if(isinf(hit_record.dist)) {
			// Didn't hit anything
	//		temp_color *= ray_direction;
			temp_color = vec3(0, 0, 0);
			return;
		}

//		temp_color = vec3(0, 0, float(tree_steps) / 5000);
//		return;

		ray_source += ray_direction * hit_record.dist;
		if(material_reflect(hit_record.material)) {
			// Hit a light source
			return;
		}
		ray_direction /= length(ray_direction);
		ray_source += ray_direction * epsilon;
	}

	// Reflection limit exceeded
	temp_color = vec3(0, 0, 0);
}

void main( void ) {
	rand_index = abs(int(hash(hash(uint(gl_FragCoord.x)) ^ uint(gl_FragCoord.y) ^ u_seed))) % u_random_buffer_length;

	vec2 pixel_size = vec2(1, 1) / u_screen_size * 2;
	vec2 shift = vec2(random().xy) * pixel_size;

	vec2 position = gl_FragCoord.xy * pixel_size + shift - vec2(1, 1);

	vec3 result_color = vec3(0, 0, 0);

	for(int i = 0; i < u_samples; i++) {
		if(i > 0) rand_index = abs(int(hash(rand_index))) % u_random_buffer_length;

		ray_source = u_camera_position;
		ray_direction = u_camera_focus + u_camera_width_vector * position.x + u_camera_height_vector * position.y;
		ray_direction /= length(ray_direction);
		trace_rays();

		if(temp_color.r < 0) temp_color.r = 0;
		if(temp_color.g < 0) temp_color.g = 0;
		if(temp_color.b < 0) temp_color.b = 0;

		result_color += temp_color;
	}

	color = result_color / float(u_samples);
}
