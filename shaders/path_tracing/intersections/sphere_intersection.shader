
const int HITTABLE_SPHERE_TYPE = 1;

void hittable_sphere_hit(int index, ivec4 data) {

    vec4 sphere_data = uintBitsToFloat(texelFetch(u_index_buffer, index + 1));
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
    hit_record.material = data.y;
    hit_record.front_hit = dot(hit_record.normal, ray_direction) < 0;

    if(!hit_record.front_hit) hit_record.normal = -hit_record.normal;
    hit_record.surface_normal = hit_record.normal;
}