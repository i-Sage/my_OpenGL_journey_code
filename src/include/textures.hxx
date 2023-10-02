#ifndef _TEXTURE_HXX_
#define _TEXTURE_HXX_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.hxx"
#include "data_types.hxx"

namespace bit {
  
  [[nodiscard]]
  inline auto generate_texture(std::string &&path) {
    u32 texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    i32 width;
    i32 height;
    i32 nr_channels;
    stbi_set_flip_vertically_on_load(true);
    u8 *data = stbi_load(path.c_str(), &width, &height, &nr_channels, 0);

    if (data) {
      if (path.ends_with("png")) {
        glTexImage2D(GL_TEXTURE_2D, 
                     0,
                     GL_RGBA,
                     width,
                     height,
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     data);
        glGenerateMipmap(GL_TEXTURE_2D);
      } else {
        glTexImage2D(GL_TEXTURE_2D, 
                     0,
                     GL_RGB,
                     width,
                     height,
                     0,
                     GL_RGB,
                     GL_UNSIGNED_BYTE,
                     data);
        glGenerateMipmap(GL_TEXTURE_2D);
      }
    } else {
      glfwTerminate();
      throw std::runtime_error("Failed to load texture at: " + std::move(path));
    }
    stbi_image_free(data);
    return texture;
  }
} // namespace bit

#endif
