#include <app/app.hxx>
#include <app/data_types.hxx>
#include <app/core/helper.hxx>
#include <app/core/render.hxx>

#include <cstdlib>
#include <stdexcept>

inline u32 VAOs[2];
inline u32 shader_program_1; 
inline u32 shader_program_2; 

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

    // f32 vertices[] = {
    //   0.5f, 0.5f, 0.0f,
    //   0.5f,-0.5f, 0.0f,
    //  -0.5f,-0.5f, 0.0f
    // };
    // f32 vertices_1[] = {
    //  -0.5f, 0.5f, 0.0f,
    //  -0.5f,-0.5f, 0.0f,
    //   0.5f, 0.5f, 0.0f
    // };
    float vertices[] = {
        -0.9f, -0.5f, 0.0f,  // left 
        -0.0f, -0.5f, 0.0f,  // right
        -0.45f, 0.5f, 0.0f,  // top 
    };
    float vertices_1[] = {
        0.0f, -0.5f, 0.0f,  // left
        0.9f, -0.5f, 0.0f,  // right
        0.45f, 0.5f, 0.0f   // top 
    };
    //
    // u32 indicies[] = {
    //   0, 1, 3,  // first triangle 
    //   1, 2, 3   // second triangle
    // };

   const char* vertex_data = "#version 450 core\n"
    "layout (location = 0) in vec3 a_pos;\n"
    "void main()\n"
    "{\n"
    " gl_Position = vec4(a_pos, 1.0);\n"
    "}\0";

   const char* frag_data_1 = "#version 450\n"
    "out vec4 frag_color;\n"
    "void main()\n"
    "{\n"
    " frag_color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

    const char* frag_data_2 = "#version 450\n"
    "out vec4 frag_color;\n"
    "void main()\n"
    "{\n"
    " frag_color = vec4(0.0f, 1.0f, 1.0f, 1.0f);\n"
    "}\0";
    
    // create shaders
    // NOTE: I skipped checking for errors
    u32 vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_data, nullptr);
    glCompileShader(vertex_shader);

    u32 fragment_shader_1 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_1, 1, &frag_data_1, nullptr);
    glCompileShader(fragment_shader_1);

    u32 fragment_shader_2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_2, 1, &frag_data_2, nullptr);
    glCompileShader(fragment_shader_2);

    // link shaders to create a shader program, 
    shader_program_1 = glCreateProgram();
    glAttachShader(shader_program_1, vertex_shader);
    glAttachShader(shader_program_1, fragment_shader_1);
    glLinkProgram(shader_program_1);

    shader_program_2 = glCreateProgram();
    glAttachShader(shader_program_2, vertex_shader);
    glAttachShader(shader_program_2, fragment_shader_2);
    glLinkProgram(shader_program_2);

    // Delete shader sources becuse they are no longer needed.
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader_1);
    glDeleteShader(fragment_shader_2);
    
    unsigned int VBOs[2];
    glGenVertexArrays(2, VAOs); // we can also generate multiple VAOs or buffers at the same time
    glGenBuffers(2, VBOs);
    // first triangle setup
    // --------------------
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// Vertex attributes stay the same
    glEnableVertexAttribArray(0);
    // glBindVertexArray(0); // no need to unbind at all as we directly bind a different VAO the next few lines
    // second triangle setup
    // ---------------------
    glBindVertexArray(VAOs[1]);	// note that we bind to a different VAO now
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);	// and a different VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_1), vertices_1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // because the vertex data is tightly packed we can also specify 0 as the vertex attribute's stride to let OpenGL figure it out
    glEnableVertexAttribArray(0);
    // glBindVertexArray(0); // not really necessary as well, but beware of calls that could affect VAOs while this one is bound (like binding element buffer objects, or enabling/disabling vertex attributes)
    // wireframe mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // GL_FILL to fill it with the cleared color.
  }
  
  void App::run() {
    while(!this->should_close()) {
      process_input(this->window);

      // RENDER FUNCTIONS GO HERE
      set_background_color(0.2, 0.3, 0.3, 1.0);
      glUseProgram(shader_program_2);

      glBindVertexArray(VAOs[0]);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      // glDrawArrays(GL_POINTS, 0, 3);
      // glDrawArrays(GL_LINE_STRIP, 0, 3);
      // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      glBindVertexArray(VAOs[1]);
      glDrawArrays(GL_TRIANGLES, 0, 3);

      glfwSwapBuffers(this->window);
      glfwPollEvents();
    }
  }

}
