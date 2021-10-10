#pragma once

#include "../gl/vertex_fragment_program.hpp"
#include "../gl/uniform.hpp"
#include "../gl/gl_texture.hpp"
#include "bounded_program.hpp"
#include <GL/glew.h>
#include <SFML/Graphics/Texture.hpp>
#include <sstream>
namespace Graphics {

class TracerPostProcessingProgram : public BoundedProgram {
    Uniform texture_uniform;
    Uniform gamma_uniform;
    Uniform brightness_uniform;
    GLTexture* texture;

    float gamma = 0.7;
    float brightness = 3.0;

public:
    TracerPostProcessingProgram();

    void set_texture(GLTexture* p_input_texture) { texture = p_input_texture; }

    void draw() {
        use();

        bind_vao();

        glActiveTexture(GL_TEXTURE0);
        texture->bind();
        texture_uniform.set1i(0);
        brightness_uniform.set1f(brightness);
        gamma_uniform.set1f(gamma);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        GLException::check();

        unbind_vao();
    }

    void set_gamma(float p_gamma) { gamma = p_gamma; }
    float get_gamma() { return gamma; }
    void set_brightness(float p_brightness) { brightness = p_brightness; }
    float get_brightness() { return brightness; }

    void take_screenshot(int seed, int frames) {

        int width = 0, height = 0;
        int miplevel = 0;

        glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &width);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &height);

        sf::Texture buffer_texture;

        buffer_texture.create(width, height);

        void* buffer = calloc(width * height, 4);
        texture->bind();
        glGetTexImage((GLenum)texture->get_target(), 0, (GLenum)GLTextureFormat::rgba, (GLenum)GLTextureType::type_unsigned_byte, buffer);
        buffer_texture.update((uint8_t*)buffer);
        free(buffer);

        std::stringstream screenshot_name;
        screenshot_name << "screenshot-" << seed << "-" << frames << ".png";

        std::string name = screenshot_name.str();

        auto image = buffer_texture.copyToImage();
        image.flipVertically();
        image.saveToFile(name);

        std::cout << "Screenshot saved as '" << name << "'\n";
    }
};

}