
bool aabb_hit(vec3 aabb_lower, vec3 aabb_upper) {
    vec3 inv_ray_direction = 1 / ray_direction;
    vec3 f = (aabb_lower - ray_source) * inv_ray_direction;
    vec3 n = (aabb_upper - ray_source) * inv_ray_direction;

    float t_min = 0;
    float t_max = hit_record.dist;

    vec3 tmax = max(f, n);
    vec3 tmin = min(f, n);

    float t1 = min(tmax.x, min(tmax.y, tmax.z));
    float t0 = max(tmin.x, max(max(tmin.y, tmin.z), t_max));

    return t1 >= t0;
}