#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//TODO: this should be maximum display resolution when the game starts 
const char *WINDOW_TITLE = "GANGSTA";
constexpr uint_fast32_t WINDOW_WIDTH = 800;
constexpr uint_fast32_t WINDOW_HEIGHT = 600;

constexpr uint_fast32_t OPEN_GL_MAJOR_VERSION = 4;
constexpr uint_fast32_t OPEN_GL_MINOR_VERSION = 5;

float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
}; 

GLFWwindow *window;

GLuint shader(GLenum type, const char *src) {
    std::ifstream file(src);

    std::stringstream content_stream;
    content_stream << file.rdbuf();

    file.close();

    auto content = content_stream.str();
    auto code = content.c_str();

    auto id = glCreateShader(type);
    glShaderSource(id, 1, &code, nullptr);
    glCompileShader(id);

    return id;
}

GLuint shader_program(const std::initializer_list<GLuint> &shaders) {
    auto id = glCreateProgram();

    for(const auto &shader : shaders) 
        glAttachShader(id, shader);

    glLinkProgram(id);

    return id;
}

void initiation () {
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

void draw_frame() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    uint_fast32_t vao;
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    uint_fast32_t vbo;
    glGenBuffers(1, &vbo);

    uint_fast32_t ebo;
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    auto program = shader_program({
        shader(GL_VERTEX_SHADER, "../shaders/vertex.glsl"),
        shader(GL_FRAGMENT_SHADER, "../shaders/fragment.glsl"),
    });
    glUseProgram(program);

    glBindVertexArray(vao);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void shutdown() {
    glfwTerminate();
}

enum GameState {
    initialize,
    running,
    pause,
    terminate,
};

auto game_state = GameState::initialize;

void process_input() {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        game_state = GameState::terminate;
}

int main() {
    initiation();

    while(game_state != GameState::terminate) {
        switch(game_state) {
            case GameState::initialize:
                game_state = GameState::running;
                break;
            case GameState::running:
                draw_frame();
                process_input();
                break;
            case GameState::pause:
                break;            
        }
    }

    shutdown();
}