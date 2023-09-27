#include <app/app.hxx>
#include <app/core/helper.hxx>
#include <app/core/render.hxx>
#include <app/core/shader.hxx>
#include <app/data_types.hxx>
#include <array>
#include <cmath>
#include <cstdlib>
#include <stdexcept>

inline u32 VAO;
inline u32 shader_program;
inline bit::Shader
    shader("/home/sage/Desktop/projects/computer_graphics_and_simulations/"
           "my_OpenGL_journey_code/app/shaders/shader.vs",
           "/home/sage/Desktop/projects/computer_graphics_and_simulations/"
           "my_OpenGL_journey_code/app/shaders/shader.fs");

namespace bit {
App::App(u32 w, u32 h, std::string &&win_name)
    : width{w}, height{h}, name{win_name} {
  init_window();
}

App::~App() {
  glfwDestroyWindow(this->window);
  glfwTerminate();
}

void App::init_window() {
  if (!glfwInit()) {
    throw std::runtime_error("Failed to initialize glfw");
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  this->window = glfwCreateWindow(this->width, this->height, this->name.c_str(),
                                  nullptr, nullptr);
  if (!this->window) {
    glfwTerminate();
    throw std::runtime_error("Failed to create window object");
  }
  glfwMakeContextCurrent(this->window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    glfwTerminate();
    throw std::runtime_error("Failed to initialize GLAD");
  }
  glViewport(0, 0, this->width, this->height);
  glfwSetFramebufferSizeCallback(this->window, framebuffer_size_callback);

  f32 vertices[] = {// positions      // colors
                    0.5f - 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -0.5f,
                    -0.5f,       0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                    0.5f,        0.0f, 0.0f, 0.0f, 1.0f};

  u32 VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and
  // then configure vertex attributes(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void App::run() {
  while (!this->should_close()) {
    process_input(this->window);

    // RENDER FUNCTIONS GO HERE
    set_background_color(0.2, 0.3, 0.3, 1.0);
    glBindVertexArray(VAO);
    shader.use();
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(this->window);
    glfwPollEvents();
  }
}

} // namespace bit
