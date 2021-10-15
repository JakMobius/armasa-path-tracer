
const uint BVH_NODE_IS_LEAF    = 1u << 28u;
const uint BVH_NODE_X_POSITIVE = 1u << 29u;
const uint BVH_NODE_Y_POSITIVE = 1u << 30u;
const uint BVH_NODE_Z_POSITIVE = 1u << 31u;
const uint BVH_NODE_FLAG_MASK  = (1u << 28u) - 1u;

int bvh_traverse(int index) {
    int bvh_stack[max_stack_size];
    int bvh_stack_size = 1;

    bvh_stack[0] = 0;
    bool restore_opposite_corner = true;

    vec3 current_aabb_lower;
    vec3 current_aabb_upper;

    int traversed = 0;

    while(bvh_stack_size > 0) {
        int next_index = bvh_stack[--bvh_stack_size];
        int memory_index = index + next_index * 2;

        ivec4 node_data = texelFetch(u_index_buffer, memory_index);

        uint node_flags = uint(node_data.x);
        vec3 node_aabb_corner = uintBitsToFloat(node_data.yzw);

        vec3 opposite_aabb_corner;

        if(restore_opposite_corner) {
            opposite_aabb_corner = uintBitsToFloat(texelFetch(u_index_buffer, memory_index + 1).xyz);
            restore_opposite_corner = false;
        } else {
            // TODO: maybe use mix() here?
            opposite_aabb_corner.x = (node_flags & BVH_NODE_X_POSITIVE) == 0 ? current_aabb_lower.x : current_aabb_upper.x;
            opposite_aabb_corner.y = (node_flags & BVH_NODE_Y_POSITIVE) == 0 ? current_aabb_lower.y : current_aabb_upper.y;
            opposite_aabb_corner.z = (node_flags & BVH_NODE_Z_POSITIVE) == 0 ? current_aabb_lower.z : current_aabb_upper.z;
        }

        vec3 new_aabb_lower = min(node_aabb_corner, opposite_aabb_corner);
        vec3 new_aabb_upper = max(node_aabb_corner, opposite_aabb_corner);

        if((node_flags & BVH_NODE_IS_LEAF) != 0) {
            restore_opposite_corner = true;
            hittable_hit(int(node_flags & BVH_NODE_FLAG_MASK));
            traversed++;
            continue;
        }

        if(!aabb_hit(new_aabb_lower, new_aabb_upper)) {
            restore_opposite_corner = true;
            continue;
        }

        current_aabb_lower = new_aabb_lower;
        current_aabb_upper = new_aabb_upper;

        bvh_stack[bvh_stack_size] = next_index * 2 + 2;
        bvh_stack_size++;

        bvh_stack[bvh_stack_size] = next_index * 2 + 1;
        bvh_stack_size++;
    }

    return traversed;
}