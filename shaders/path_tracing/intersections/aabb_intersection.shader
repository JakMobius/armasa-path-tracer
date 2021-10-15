
bool aabb_hit(vec3 aabb_lower, vec3 aabb_upper) {
    vec3 inv_ray_direction = 1 / ray_direction;
    vec3 t0v = (aabb_lower - ray_source) * inv_ray_direction;
    vec3 t1v = (aabb_upper - ray_source) * inv_ray_direction;

    float t_min = 0;
    float t_max = hit_record.dist;

    for(int i = 0; i < 3; i++) {
        float t0 = t0v[i], t1 = t1v[i], t2 = t1;

        if(inv_ray_direction[i] < 0) {
            t1 = t0;
            t0 = t2;
        }

        t_min = max(t_min, t0);
        t_max = min(t_max, t1);
    }

    return t_max > t_min;
}