#pragma once

#include <GL/glew.h>
#include "../gl/vertex_fragment_program.hpp"
#include "../gl/uniform.hpp"
#include "../gl/gl_texture.hpp"

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
};
}