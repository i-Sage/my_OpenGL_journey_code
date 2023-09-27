#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <shader.hxx>
#include <data_types.hxx>


// INFO: process all input: query GLFW whether relevant keys are pressed/released 
// this frame and react accordingly
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

// INFO: whenever the window size changed (by OS or user resize) this callback
// function executes
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}


auto main() -> int {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  constexpr u32 SCR_WIDTH  = 600;
  constexpr u32 SCR_HEIGHT = 350;

  auto *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);

  if (window == nullptr) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // INFO: glad: load all OpenGL function pointers
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  bit::Shader ourShader
  (
    "/home/sage/Desktop/projects/computer_graphics_and_simulations/my_OpenGL_journey_code/shaders/vs.glsl",
    "/home/sage/Desktop/projects/computer_graphics_and_simulations/my_OpenGL_journey_code/shaders/fs.glsl"
  );

  f32 vertices[] = {
    // positions           // colors          // texture coords
    0.5f, 0.5f, 0.0f,     1.0f, 0.0f, 0.0f,   1.0f, 1.0f,       // top right 
    0.5f,-0.5f, 0.0f,     0.0f, 1.0f, 0.0f,   1.0f, 0.0f,       // bottom right
   -0.5f,-0.5f, 0.0f,     0.0f, 0.0f, 1.0f,   0.0f, 0.0f,       // bottom left
   -0.5f, 0.5f, 0.0f,     1.0f, 1.0f, 0.0f,   0.0f, 1.0f,       // top right
  };
 
  u32 indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
  };
  
  u32 texture_1;
  glGenTextures(1, &texture_1);
  glBindTexture(GL_TEXTURE_2D, texture_1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  i32 width;
  i32 height;
  i32 nr_channels;
  stbi_set_flip_vertically_on_load(true);

  u8 *data = stbi_load("/home/sage/Desktop/projects/computer_graphics_and_simulations/my_OpenGL_journey_code/textures/container.jpg",
                       &width, &height, &nr_channels, 0);

  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
                 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cerr << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);


  u32 texture_2;
  glGenTextures(1, &texture_2);
  glBindTexture(GL_TEXTURE_2D, texture_2);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  data = stbi_load("/home/sage/Desktop/projects/computer_graphics_and_simulations/my_OpenGL_journey_code/textures/awesomeface.png",
                       &width, &height, &nr_channels, 0);

  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cerr << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  u32 VAO;
  u32 VBO;
  u32 EBO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glBindVertexArray(VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  
  ourShader.use();
  
  ourShader.setInt("texture_1", 0);
  ourShader.setInt("texture_2", 1);

  // INFO: render loop
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_1);
 
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_2);

    ourShader.use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  glfwTerminate();
}
