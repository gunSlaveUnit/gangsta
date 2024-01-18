#include "shader.hpp"

namespace gangsta {
    Shader::Shader(GLenum type, const char *src) {
        id = glCreateShader(type);
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);
    }
}