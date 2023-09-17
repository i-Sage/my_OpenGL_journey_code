#include <app/app.hxx>
#include <iostream>
#include <cstdlib>

auto main() -> int {
  try {
    bit::App{650, 350, "Hello OpenGL"}.run();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl; 
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
