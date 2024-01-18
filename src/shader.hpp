#pragma once

#ifndef GANGSTA_SHADER_HPP
#define GANGSTA_SHADER_HPP

#include <glad/glad.h>

namespace gangsta {
    struct Shader {
        Shader(GLenum type, const char *src);

        GLuint id;
        GLenum type;
        const char *src;
    };
}

#endif //GANGSTA_SHADER_HPP