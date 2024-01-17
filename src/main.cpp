#include <cstdint>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//TODO: this should be maximum display resolution when the game starts 
const char *WINDOW_TITLE = "GANGSTA";
constexpr uint_fast32_t WINDOW_WIDTH = 800;
constexpr uint_fast32_t WINDOW_HEIGHT = 600;

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

            auto window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
            if (window == nullptr)
                std::cerr << "ERROR: GLFW failed to create a window\n";
            glfwMakeContextCurrent(window);

            if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
                std::cerr << "ERROR: failed to initialize GLAD\n";

            glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        }

        void draw_frame() final {
            
        }

        void terminate() final {
            glfwTerminate();
        }
};

enum GameState {
    initialize,
    running,
    pause,
    terminate,
};

auto game_state = GameState::initialize;
OpenGLRenderer renderer;

int main() {
    renderer.initialize();

    while(game_state != GameState::terminate) {
        switch(game_state) {
            case initialize:
                game_state = GameState::running;
                break;
            case running:
                break;
            case pause:
                break;            
        }
    }
}