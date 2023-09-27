#pragma once 

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

#include "data_types.hxx"

namespace bit {

  class App {
    public:
      App(u32 w, u32 h, std::string&& win_name);
     ~App();
      App(const App&)              = delete;
      App& operator=(const App&)   = delete;
      inline bool should_close() { return glfwWindowShouldClose(this->window); }
      void run();

    private:
      u32 width {0};
      u32 height{0};
      std::string name;

      GLFWwindow* window = nullptr;
      void init_window();
  };

}
