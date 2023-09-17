#include <app/app.hxx>
#include <app/data_types.hxx>
#include <app/core/helper.hxx>
#include <app/core/render.hxx>

#include <cstdlib>
#include <stdexcept>

inline u32 VAO;
inline u32 shader_program; 

namespace bit {
  App::App(u32 w, u32 h, std::string&& win_name)
    : width{w}, height{h}, name{win_name}
  {
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
    
    this->window = glfwCreateWindow(this->width, this->height, this->name.c_str(), nullptr, nullptr);
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

    f32 vertices[] = {
      0.5f, 0.5f, 0.0f,  // top right
      0.5f,-0.5f, 0.0f,  // bottom right
     -0.5f,-0.5f, 0.0f,  // bottom left 
     -0.5f, 0.5f, 0.0f   // bottom left
    };

    u32 indicies[] = {
      0, 1, 3,  // first triangle 
      1, 2, 3   // second triangle
    };

   const char* vertex_data = "#version 450 core\n"
    "layout (location = 0) in vec3 a_pos;\n"
    "void main()\n"
    "{\n"
    " gl_Position = vec4(a_pos, 1.0);\n"
    "}\0";

   const char* frag_data = "#version 450\n"
    "out vec4 frag_color;\n"
    "void main()\n"
    "{\n"
    " frag_color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";
    
    // create shaders
    // NOTE: I skipped checking for errors
    u32 vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_data, nullptr);
    glCompileShader(vertex_shader);

    u32 fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &frag_data, nullptr);
    glCompileShader(fragment_shader);

    // link shaders to create a shader program, 
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    // Delete shader sources becuse they are no longer needed.
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    
    u32 VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    
    // wireframe mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // GL_FILL to fill it with the cleared color.
  }
  
  void App::run() {
    while(!this->should_close()) {
      process_input(this->window);

      // RENDER FUNCTIONS GO HERE
      set_background_color(0.2, 0.3, 0.3, 1.0);
      glUseProgram(shader_program);

      glBindVertexArray(VAO);
      // glDrawArrays(GL_TRIANGLES, 0, 3);
      // glDrawArrays(GL_POINTS, 0, 3);
      // glDrawArrays(GL_LINE_STRIP, 0, 3);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      glfwSwapBuffers(this->window);
      glfwPollEvents();
    }
  }

}
