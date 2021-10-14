
const int HITTABLE_LIST_TYPE = 0;

// Format:
// x: 29 high bits = children count, 3 low bits = 0b000 (hittable list type)

void hittable_list_hit(int index, ivec4 data) {
    int stack_index = stack_size - 1;
    int current_child_index = hittable_child_index_stack[stack_index];

    // As it's required to store two child indices somehow
    // in a single ivec4, we should pack children count
    // in the same field with hittable type.

    int children_count = data.x >> 3;

    // If we've entered the hittable list, check its aabb
    if(current_child_index == 0) {
        int offset = children_count / 4;
        vec3 aabb_lower = uintBitsToFloat(texelFetch(u_index_buffer, index + offset + 1).xyz);
        vec3 aabb_upper = uintBitsToFloat(texelFetch(u_index_buffer, index + offset + 2).xyz);

        if(!aabb_hit(aabb_lower, aabb_upper)) {
            stack_size--;
            return;
        }
    }

    if(current_child_index == children_count) {
        stack_size--;
        return;
    }

    hittable_child_index_stack[stack_index] = current_child_index + 1;
    int children_index;

    // It's quite important that first two children
    // does not require additional texelFetch call,
    // because BFS splts the world into a binary
    // tree, so this check saves a lot of texelFetch calls

    if(current_child_index >= 3) {
        // But if this node is wider, it's required to
        // read additional texels

        //		discard;

        int first_child_index = index * 4 + 1;
        int children_pointer = first_child_index + current_child_index;
        children_index = texelFetch(u_index_buffer, children_pointer / 4)[children_pointer % 4];
    } else {
        children_index = data[current_child_index + 1];
    }

    hittable_index_stack[stack_size] = children_index;
    hittable_child_index_stack[stack_size] = 0;
    stack_size++;
}