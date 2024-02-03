#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>

#include <GLM/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>

//TODO: this should be maximum display resolution when the game starts 
const char *WINDOW_TITLE = "GANGSTA";
constexpr uint_fast32_t WINDOW_WIDTH = 800;
constexpr uint_fast32_t WINDOW_HEIGHT = 600;

constexpr uint_fast32_t OPEN_GL_MAJOR_VERSION = 4;
constexpr uint_fast32_t OPEN_GL_MINOR_VERSION = 5;

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
};

struct Camera {
    Camera() {
        position = glm::vec3(0.0f, 0.0f,  3.0f);
        front = glm::vec3(0.0f, 0.0f, -1.0f);
        up = glm::vec3(0.0f, 1.0f,  0.0f);
    }

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
};

GLFWwindow *window;
Camera camera;

double delta; 
double last_time;

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

void initiation() {
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

    glEnable(GL_DEPTH_TEST);
}

void draw_frame() {
    auto current_time = glfwGetTime();
    delta = current_time - last_time;
    last_time = current_time;

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    uint_fast32_t vao;
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    uint_fast32_t vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    auto program = shader_program({
        shader(GL_VERTEX_SHADER, "../shaders/vertex.glsl"),
        shader(GL_FRAGMENT_SHADER, "../shaders/fragment.glsl"),
    });
    glUseProgram(program);

    const auto model = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 1.0));
    auto model_location = glGetUniformLocation(program, "model"); 
    glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));

    auto view = glm::lookAt(
        camera.position, 
        camera.position + camera.front, 
        camera.up
    );
    auto view_location = glGetUniformLocation(program, "view"); 
    glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));

    const auto projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    auto projection_location = glGetUniformLocation(program, "projection"); 
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(vao);

    glDrawArrays(GL_TRIANGLES, 0, 36);

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

    constexpr auto BASE_CAMERA_SPEED = 0.05;
    auto camera_speed = static_cast<float>(BASE_CAMERA_SPEED * delta);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
        camera.position += camera_speed * camera.front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.position -= camera_speed * camera.front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.position -= glm::normalize(glm::cross(camera.front, camera.up)) * camera_speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.position += glm::normalize(glm::cross(camera.front, camera.up)) * camera_speed;
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