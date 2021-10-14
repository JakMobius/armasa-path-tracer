
struct HitRecord {
    float dist;
    vec3 surface_normal;
    vec3 normal;
    vec3 point;
    int material;
    bool front_hit;
    //int attempts;
};