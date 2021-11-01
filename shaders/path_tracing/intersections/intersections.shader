
void hittable_hit(int index);

#include "./aabb_intersection.shader"
#include "./sphere_intersection.shader"
#include "./triangle_intersection.shader"
#include "./bvh_intersection.shader"

void hittable_hit(int index) {
    ivec4 hittable_data = texelFetch(u_index_buffer, index);
    int hittable_type = hittable_data.x & 7;

    switch(hittable_type) {
        case HITTABLE_SPHERE_TYPE:   hittable_sphere_hit(index, hittable_data);   break;
        case HITTABLE_TRIANGLE_TYPE: hittable_triangle_hit(index, hittable_data); break;
        default: return;
    }
}