#include "shader_program.hpp"

namespace gangsta {
    ShaderProgram::ShaderProgram(const std::initializer_list<gangsta::Shader> &shaders) {
        id = glCreateProgram();

        for(const auto &shader : shaders) 
            glAttachShader(id, shader.id);

        glLinkProgram(id);
    }

    void ShaderProgram::use() {
        glUseProgram(id);
    }
}