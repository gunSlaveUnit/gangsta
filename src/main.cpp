#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

int main() {
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