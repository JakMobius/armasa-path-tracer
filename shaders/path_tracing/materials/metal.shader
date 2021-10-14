
const int MATERIAL_METAL = 0;

void material_metal_reflect(int index, ivec4 data) {
    vec4 material_color = uintBitsToFloat(texelFetch(u_index_buffer, index + 1));
    float fuzziness = material_color.a;

    temp_color *= material_color.rgb;
    ray_direction = reflect(ray_direction, hit_record.normal);

    vec3 random_vec = random();

    ray_direction += normalize(fuzziness * random_vec + ray_direction);
    float projection = dot(ray_direction, hit_record.surface_normal);

    if(projection < 0) {
        ray_direction -= hit_record.surface_normal * projection * 2;
    }

    ray_direction = normalize(ray_direction);
    inv_ray_direction = 1 / ray_direction;
}