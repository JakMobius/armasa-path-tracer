
const uint BVH_NODE_IS_LEAF    = 1u << 28u;
const uint BVH_NODE_X_POSITIVE = 1u << 29u;
const uint BVH_NODE_Y_POSITIVE = 1u << 30u;
const uint BVH_NODE_Z_POSITIVE = 1u << 31u;
const uint BVH_NODE_FLAG_MASK  = (1u << 28u) - 1u;

vec3 bvh_get_node_opposide_corner(uint flags, vec3 aabb_lower, vec3 aabb_upper) {
    vec3 result;

    // TODO: maybe use mix() here?
    result.x = (flags & BVH_NODE_X_POSITIVE) == 0 ? aabb_lower.x : aabb_upper.x;
    result.y = (flags & BVH_NODE_Y_POSITIVE) == 0 ? aabb_lower.y : aabb_upper.y;
    result.z = (flags & BVH_NODE_Z_POSITIVE) == 0 ? aabb_lower.z : aabb_upper.z;

    return result;
}

int bvh_traverse(int index) {
    vec3 initial_aabb_lower = uintBitsToFloat(texelFetch(u_index_buffer, index).xyz);
    int bvh_base_index = index + 1;

    vec3 current_aabb_lower = initial_aabb_lower;
    vec3 current_aabb_upper = vec3(0, 0, 0);

    int bvh_stack[max_stack_size];
    vec3 bvh_aabb_lower_stack[max_stack_size];
    vec3 bvh_aabb_upper_stack[max_stack_size];
    int bvh_stack_size = 1;

    bvh_aabb_upper_stack[0] = uintBitsToFloat(texelFetch(u_index_buffer, bvh_base_index).yzw);
    bvh_aabb_lower_stack[0] = initial_aabb_lower;
    bvh_stack[0] = 0;

    int traversed = 0;
    bool returned = false;

    while(bvh_stack_size > 0) {
        int next_index = bvh_stack[--bvh_stack_size];

        ivec4 node_data = texelFetch(u_index_buffer, next_index + bvh_base_index);

        uint node_flags = uint(node_data.x);
        vec3 node_aabb_corner = uintBitsToFloat(node_data.yzw);

        vec3 opposite_aabb_corner = bvh_get_node_opposide_corner(node_flags, current_aabb_lower, current_aabb_upper);

        vec3 new_aabb_lower = min(node_aabb_corner, opposite_aabb_corner);
        vec3 new_aabb_upper = max(node_aabb_corner, opposite_aabb_corner);

        if(!aabb_hit(new_aabb_lower, new_aabb_upper)) {
            current_aabb_upper = bvh_aabb_upper_stack[bvh_stack_size - 1];
            current_aabb_lower = bvh_aabb_lower_stack[bvh_stack_size - 1];
            continue;
        }

        if((node_flags & BVH_NODE_IS_LEAF) != 0) {
            hittable_hit(int(node_flags & BVH_NODE_FLAG_MASK));
            current_aabb_upper = bvh_aabb_upper_stack[bvh_stack_size - 1];
            current_aabb_lower = bvh_aabb_lower_stack[bvh_stack_size - 1];
            continue;
        }

        current_aabb_lower = new_aabb_lower;
        current_aabb_upper = new_aabb_upper;

        bvh_stack[bvh_stack_size] = next_index * 2 + 2;
        bvh_stack_size++;

        bvh_stack[bvh_stack_size] = next_index * 2 + 1;
        bvh_aabb_upper_stack[bvh_stack_size] = current_aabb_upper;
        bvh_aabb_lower_stack[bvh_stack_size] = current_aabb_lower;
        bvh_stack_size++;

        traversed++;
    }

    return traversed;
}