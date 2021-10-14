
const int MATERIAL_LAMBERTIAN = 1;

void material_lambertian_reflect(int index, ivec4 data) {
    vec3 material_color = uintBitsToFloat(texelFetch(u_index_buffer, index + 1).rgb);
    temp_color *= material_color;

    ray_direction = normalize(hit_record.normal + random_unit_vec3());

    float projection = dot(ray_direction, hit_record.surface_normal);

    if(projection < 0) {
        ray_direction -= hit_record.surface_normal * projection * 2;
    }

    inv_ray_direction = 1 / ray_direction;
}