#pragma once

#ifndef GANGSTA_SHADER_PROGRAM_HPP
#define GANGSTA_SHADER_PROGRAM_HPP

#include <initializer_list>

#include <glad/glad.h>

#include "shader.hpp"

namespace gangsta {
    struct ShaderProgram {
        ShaderProgram(const std::initializer_list<gangsta::Shader> &shaders);
        void use();

        GLuint id;
    };
}

#endif //GANGSTA_SHADER_PROGRAM_HPP