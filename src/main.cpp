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

        GLFWwindow* get_window() const {
            return window;
        }

        void draw_frame() final {
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