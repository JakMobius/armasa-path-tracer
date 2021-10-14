
#include "./dielectric.shader"
#include "./lambertian.shader"
#include "./light.shader"
#include "./metal.shader"

bool material_reflect(int index) {
	ivec4 material_data = texelFetch(u_index_buffer, index);
	int material_type = material_data.r;

	switch(material_type) {
		case MATERIAL_METAL: 	  		material_metal_reflect(index, material_data); 	    return false;
		case MATERIAL_LAMBERTIAN: 		material_lambertian_reflect(index, material_data);  return false;
		case MATERIAL_LAMBERTIAN_LIGHT: material_light_reflect(index, material_data); 	    return true;
		case MATERIAL_DIELECTRIC:       material_dielectric_reflect(index, material_data);  return false;
	}

	return false;
}