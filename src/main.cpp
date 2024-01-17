#include <cstdint>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//TODO: this should be maximum display resolution when the game starts 
const char *WINDOW_TITLE = "GANGSTA";
constexpr uint_fast32_t WINDOW_WIDTH = 800;
constexpr uint_fast32_t WINDOW_HEIGHT = 600;

constexpr uint_fast32_t OPEN_GL_MAJOR_VERSION = 4;
constexpr uint_fast32_t OPEN_GL_MINOR_VERSION = 5;

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f,
};  

class Renderer {
    public:
        virtual void initialize() = 0;
        virtual void draw_frame() = 0;
        virtual void terminate() = 0;
};

class OpenGLRenderer : public Renderer {
    public:
        void initialize() final {
            glfwInit();

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPEN_GL_MAJOR_VERSION);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPEN_GL_MINOR_VERSION);

            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            #ifdef __APPLE__
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            #endif

            window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
            if (window == nullptr)
                std::cerr << "ERROR: GLFW failed to create a window\n";
            glfwMakeContextCurrent(window);

            if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
                std::cerr << "ERROR: failed to initialize GLAD\n";

            glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        }

        uint_fast32_t make_shader(const char *source, GLenum shader_type) {
            uint_fast32_t shader = glCreateShader(shader_type);
            glShaderSource(shader, 1, &source, nullptr);
            glCompileShader(shader);
            return shader;
        }

        uint_fast32_t make_program(const std::initializer_list<uint_fast32_t> &shaders) {
            uint_fast32_t program = glCreateProgram();

            for (const auto &shader : shaders) 
                glAttachShader(program, shader);

            glLinkProgram(program);

            // TODO: should I do that here?
            for (auto &shader : shaders)
                glDeleteShader(shader);

            return program;
        }

        GLFWwindow* get_window() const {
            return window;
        }

        void draw_frame() final {
            glUseProgram(
                make_program({
                    make_shader("shaders/vertex.glsl", GL_VERTEX_SHADER),
                    make_shader("shaders/fragment.glsl", GL_FRAGMENT_SHADER)
                })
            );

            uint_fast32_t vbo;
            glGenBuffers(1, &vbo);

            glBindBuffer(GL_ARRAY_BUFFER, vbo);

            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        void terminate() final {
            glfwTerminate();
        }
    private:
        GLFWwindow *window;
};

enum GameState {
    initialize,
    running,
    pause,
    terminate,
};

auto game_state = GameState::initialize;
OpenGLRenderer renderer;

void process_input(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        game_state = GameState::terminate;
}

int main() {
    renderer.initialize();

    while(game_state != GameState::terminate) {
        switch(game_state) {
            case initialize:
                game_state = GameState::running;
                break;
            case running:
                renderer.draw_frame();
                process_input(renderer.get_window());
                break;
            case pause:
                break;            
        }
    }

    renderer.terminate();
}