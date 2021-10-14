
const int MATERIAL_LAMBERTIAN_LIGHT = 2;

void material_light_reflect(int index, ivec4 data) {
    vec3 material_color = uintBitsToFloat(texelFetch(u_index_buffer, index + 1).rgb);
    temp_color *= material_color;
}