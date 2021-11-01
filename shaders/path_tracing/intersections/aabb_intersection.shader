
bool aabb_hit(vec3 aabb_lower, vec3 aabb_upper) {
    vec3 f = (aabb_lower - ray_source) * inv_ray_direction;
    vec3 n = (aabb_upper - ray_source) * inv_ray_direction;

    vec3 tmax = max(f, n);
    vec3 tmin = min(f, n);

    float t1 = min(tmax.x, min(tmax.y, tmax.z));
    float t0 = max(tmin.x, max(tmin.y, tmin.z));

    if(t1 > hit_record.dist) t1 = hit_record.dist;

    return t1 >= t0 && max(t1, t0) > 0;
}