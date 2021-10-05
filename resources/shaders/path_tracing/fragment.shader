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
const int max_stack_size = 20;

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
	//int attempts;
};

HitRecord hit_record;
vec3 ray_source;
vec3 ray_direction;
int hittable_index_stack[max_stack_size];
int hittable_child_index_stack[max_stack_size];
int stack_size;
int rand_index;

void hittable_triangle_hit(int index);
void hittable_sphere_hit(int index);
void hittable_list_hit(int index);
void hittable_hit(int index);

bool material_lambertian_reflect(bool has_light);
bool material_metal_reflect();

bool material_reflect(int index);
void trace_rays();

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

void hittable_triangle_hit(int index, ivec4 data) {
	stack_size--;
	//hit_record.attempts++;

	vec3 point_a = texelFetch(u_float_buffer, index).xyz;
	vec3 point_b = texelFetch(u_float_buffer, index + 1).xyz;
	vec3 point_c = texelFetch(u_float_buffer, index + 2).xyz;

	vec3 edge1 = point_b - point_a;
	vec3 edge2 = point_c - point_a;
	vec3 h = cross(ray_direction, edge2);
	float a = dot(edge1, h);

	if (a > -epsilon && a < epsilon) return;

	float f = 1.0 / a;
	vec3 s = ray_source - point_a;
	float u = f * dot(s, h);

	if (u < 0.0 || u > 1.0) return;

	vec3 q = cross(s, edge1);
	float v = f * dot(ray_direction, q);

	if (v < 0.0 || u + v > 1.0) return;

	float t = f * dot(edge2, q);

	if (t > epsilon && t < hit_record.dist) {
		hit_record.dist = t;
		hit_record.point = ray_source + ray_direction * t;
		hit_record.normal = normalize(cross(edge1, edge2));
		hit_record.material = data.g;

		if(dot(hit_record.normal, ray_direction) > 0) hit_record.normal = -hit_record.normal;
	}
}

void hittable_sphere_hit(int index, ivec4 data) {
	stack_size--;

	vec4 sphere_data = texelFetch(u_float_buffer, index);
	vec3 sphere_position = sphere_data.xyz;
	float sphere_radius = sphere_data.w;

	vec3 sphere_direction = ray_source - sphere_position;
	float ray_sphere_dir_proj = dot(ray_direction, sphere_direction);
	float sphere_direction_len_sq = dot(sphere_direction, sphere_direction);
	float disc = sphere_radius * sphere_radius - (sphere_direction_len_sq - ray_sphere_dir_proj * ray_sphere_dir_proj);

	if (disc < 0) return;

	disc = sqrt(disc);

	float b = -dot(ray_direction, sphere_direction);
	float d1 = b - disc;
	float d2 = b + disc;
	float d = -1;

	if (d1 > 0 && (d2 > d1 || d2 < 0)) {
		d = d1;
	} else if (d2 > 0 && (d1 > d2 || d1 < 0)) {
		d = d2;
	} else {
		return;
	}

	if(d > hit_record.dist) return;

	vec3 point = ray_source + d * ray_direction;

	hit_record.point = point;
	hit_record.dist = d;
	hit_record.normal = (point - sphere_position) / sphere_radius;
	hit_record.material = data.g;

	if(dot(hit_record.normal, ray_direction) > 0) hit_record.normal = -hit_record.normal;
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

void hittable_list_hit(int index, ivec4 data) {
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

	int children_count = data.g;

	if(current_child_index == children_count) {
		stack_size--;
		return;
	}

	hittable_child_index_stack[stack_index] = current_child_index + 1;
	int children_index;

	if(current_child_index >= 2) {
		int first_child_index = (index << 2) + 2;
		int children_pointer = first_child_index + current_child_index;
		children_index = texelFetch(u_index_buffer, children_pointer >> 2)[children_pointer & 3];
	} else {
		children_index = data[current_child_index + 2];
	}

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

	ray_direction += normalize(fuzziness * random_vec + ray_direction);
	float projection = dot(ray_direction, hit_record.normal);

	if(projection < 0) {
		ray_direction -= hit_record.normal * projection * 2;
	}

	ray_direction = normalize(ray_direction);
}

void material_light_reflect(int index) {
	vec3 material_color = texelFetch(u_float_buffer, index).rgb;
	temp_color *= material_color;
}

void material_lambertian_reflect(int index) {
	vec3 material_color = texelFetch(u_float_buffer, index).rgb;
	temp_color *= material_color;

	ray_direction = normalize(random_unit_vec3() + hit_record.normal);
}

void hittable_hit(int index) {
	ivec4 hittable_data = texelFetch(u_index_buffer, index);
	int hittable_type = hittable_data.x;

	switch(hittable_type) {
		case HITTABLE_LIST_TYPE:     hittable_list_hit(index, hittable_data);     break;
		case HITTABLE_SPHERE_TYPE:   hittable_sphere_hit(index, hittable_data);   break;
		case HITTABLE_TRIANGLE_TYPE: hittable_triangle_hit(index, hittable_data); break;
		default: stack_size--;
	}
}

bool material_reflect(int index) {
	ivec4 material_data = texelFetch(u_index_buffer, index);
	int material_type = material_data.r;

	switch(material_type) {
		case MATERIAL_METAL: 	  		material_metal_reflect(index); 	  return false;
		case MATERIAL_LAMBERTIAN: 		material_lambertian_reflect(index); return false;
		case MATERIAL_LAMBERTIAN_LIGHT: material_light_reflect(index); 	  return true;
	}

	return false;
}

void trace_rays() {
	temp_color = vec3(1, 1, 1);
	int reflections = 0;
	int tree_steps = 0;

	while(reflections++ < u_max_reflections) {

		hit_record.dist = infinity;
		hittable_index_stack[0] = u_entry_index;
		hittable_child_index_stack[0] = 0;
		stack_size = 1;

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

		ray_source += ray_direction * hit_record.dist;
		if(material_reflect(hit_record.material)) {
			// Hit a light source
//			 break; // uncomment to trace pixel complexity
			 return; // uncomment to generate real images
		}
		ray_source += ray_direction * epsilon;
	}

	// Reflection limit exceeded
//	temp_color = vec3(0, 0, float(hit_record.attempts) / 60);
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
		ray_direction = normalize(u_camera_focus + u_camera_width_vector * position.x + u_camera_height_vector * position.y);
		trace_rays();

		if(temp_color.r < 0) temp_color.r = 0;
		if(temp_color.g < 0) temp_color.g = 0;
		if(temp_color.b < 0) temp_color.b = 0;

		result_color += temp_color;
	}

	color = result_color / float(u_samples);
}
