
// A single iteration of Bob Jenkins' One-At-A-Time hashing algorithm.
uint hash(uint x) {
    x += (x << 10u);
    x ^= (x >>  6u);
    x += (x <<  3u);
    x ^= (x >> 11u);
    x += (x << 15u);
    return x;
}

vec3 random() {
    if(rand_index >= u_random_buffer_length) rand_index = 0;
    return texelFetch(u_random_buffer, rand_index++).xyz;
}

vec3 random_unit_vec3() {
    vec3 r = random();
    float a = r.x * pi_2;
    vec2 u = vec2(cos(a), sin(a));
    float s = r.y * 2 - 1;
    float c = sqrt(1 - s * s);
    return vec3(c * u, s);
}
