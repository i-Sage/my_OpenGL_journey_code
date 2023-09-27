#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../data_types.hxx"

namespace bit {
  inline void set_background_color(f64 R, f64 G, f64 B, f64 A) {
    glClearColor(R, G, B, A);
    glClear(GL_COLOR_BUFFER_BIT);
  }

}
