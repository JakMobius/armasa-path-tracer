#pragma once

#include <GL/glew.h>
#include "../gl/vertex_fragment_program.hpp"
#include "../gl/uniform.hpp"
#include "../gl/gl_texture.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <sstream>

namespace Graphics {

class TextureProgram : public VertexFragmentProgram {
    Uniform texture_uniform;
    GLBuffer<float>* vertex_buffer;
    GLTexture* texture;

public:
    TextureProgram();

    ~TextureProgram() { delete vertex_buffer; }

    void draw() {
        use();

        bind_vao();

        glActiveTexture(GL_TEXTURE0);
        texture->bind();
        texture_uniform.set1i(0);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        GLException::check();

        unbind_vao();
    }

    void set_texture(GLTexture* p_input_texture) { texture = p_input_texture; }

    void load_image(const char* path, int frames) {

    }

    void take_screenshot(int seed, int frames) {

        int width = 0;
        int height = 0;

        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

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