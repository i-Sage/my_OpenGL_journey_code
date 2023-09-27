add_subdirectory(third_party)

add_executable(main main.cxx)
target_include_directories(main PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include/)
target_link_libraries(main PRIVATE glad glfw glm)
