#pragma once

#include <GL/glew.h>
#include "../gl/gl_buffer.hpp"
#include "../gl/gl_texture_buffer.hpp"
#include "../gl/vertex_fragment_program.hpp"
#include "../gl/uniform.hpp"
#include "camera_uniform_controller/camera_uniform_controller.hpp"
#include "../../scene/scene_buffer.hpp"
#include <random>

namespace Graphics {


class PathTracerComputeProgram: public Program {
    GLTexture output;
public:
    PathTracerComputeProgram();

    ~PathTracerComputeProgram() {}

    void draw() {

        int work_grp_cnt[3];

        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);

        use();

        glDispatchCompute((GLuint)500, (GLuint)500, 1);


        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);


    }

    GLTexture* get_texture() {
        return &output;
    }
};
}