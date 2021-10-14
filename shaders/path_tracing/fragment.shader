precision highp float;

#include "./structures/hit_record.shader"
#include "../utils/constants.shader"

uniform vec2 u_screen_size;

uniform vec3 u_camera_position;
uniform vec3 u_camera_width_vector;
uniform vec3 u_camera_height_vector;
uniform vec3 u_camera_focus;
uniform vec3 u_background;

uniform isamplerBuffer u_index_buffer;
uniform samplerBuffer u_random_buffer;

uniform int u_entry_index;
uniform int u_random_buffer_length;
uniform int u_samples;
uniform int u_max_reflections;
uniform uint u_seed;

out vec3 color;

const int max_stack_size = 20;

HitRecord hit_record;

vec3 ray_source;
vec3 ray_direction;
vec3 inv_ray_direction;

int rand_index;

vec3 temp_color;

int memory_reads = 0;

#include "../utils/random.shader"
#include "./intersections/intersections.shader"
#include "./materials/materials.shader"

void trace_rays() {
	temp_color = vec3(1, 1, 1);
	int reflections = 0;

	do {
		hit_record.dist = infinity;
		int traversed = bvh_traverse(u_entry_index);

		if(isinf(hit_record.dist)) {
			// Didn't hit anything
//			temp_color = ray_direction;
			temp_color *= u_background;
//			temp_color = vec3(float(traversed) / 10, 0, 0);
			return;
		}

		ray_source += ray_direction * hit_record.dist;
		if(material_reflect(hit_record.material)) {
			// Hit a light source
//			temp_color = vec3(float(traversed) / 10, 0, 0);
//			 break; // uncomment to trace pixel complexity
			 return; // uncomment to generate real images
		}
		ray_source += ray_direction * epsilon;

	} while(++reflections < u_max_reflections);

	if(u_max_reflections < 0) {
		// Fast render mode, no reflections
		float color = 1 / hit_record.dist;
		temp_color = vec3(color, color, color);
	} else {

		// Reflection limit exceeded
		temp_color = vec3(0, 0, 0);
//		temp_color = ray_direction;
//		temp_color = (ray_direction + 1) * 0.5;//vec3(0, 0, 0);
	}
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
		inv_ray_direction = 1 / ray_direction;
		trace_rays();

		result_color += temp_color;
	}

	color = result_color / float(u_samples);
}
