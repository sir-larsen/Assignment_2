
cmake_minimum_required(VERSION 3.12)


project(assignment_2)


find_package(OpenGL REQUIRED)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)



add_subdirectory(external/glfw-3.3.2)


# (https://github.com/Perlmint/glew-cmake)
add_subdirectory(external/glew-cmake-2.1.0)


add_subdirectory(external/glm-0.9.9.8)

add_subdirectory(external/assimp-5.0.0)



# Adds the levels folder to the bin directory, making the content available for our program to read
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/levels DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)

# Adds the shader folder to the bin directory, making its content available for out program to read.
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)

# Adds the resource folder to the bin directory, making the content available for our program to read
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)



add_executable(assignment_2
	main.cpp
	src/Core/Animator.cpp
	src/Core/Animator.h
	src/Core/IndexBuffer.h
	src/Core/IndexBuffer.cpp
	src/Core/Renderer.h
	src/Core/Renderer.cpp
	src/Core/ScenarioLoader.h
	src/Core/ScenarioLoader.cpp
	src/Core/shader.h
	src/Core/VertexArray.h
	src/Core/VertexArray.cpp
	src/Core/VertexBuffer.h
	src/Core/VertexBuffer.cpp 
	src/Core/Camera.h
	src/Core/stb_image.h
	src/Core/Texture.h
	src/Core/Texture.cpp 
	src/Core/model.h 
	src/Core/AStar.h
	src/Core/AStar.cpp
	src/Core/Minimap.h
	src/Core/Minimap.cpp 
	src/Core/Framebuffer.h
	src/Core/Framebuffer.cpp
	src/Core/Renderbuffer.h
	src/Core/Renderbuffer.cpp 
	src/Maze3D/Maze3D.cpp
	src/Maze3D/Maze3D.h
	src/Maze3D/Pellet3D.h
	src/Maze3D/Pellet3D.cpp
	src/Maze3D/Ghost3D.h
	src/Maze3D/Ghost3D.cpp
	src/Maze2D/Maze.cpp
	src/Maze2D/Maze.h
	src/Maze2D/Pellets.cpp
	src/Maze2D/Pellets.h
	src/Maze2D/Pacman.cpp
	src/Maze2D/Pacman.h
	src/Maze2D/MovableObject.cpp
	src/Maze2D/MovableObject.h
	src/Maze2D/Ghost.cpp
	src/Maze2D/Ghost.h)


target_compile_definitions(assignment_2 PRIVATE GLEW_STATIC)

target_link_libraries(assignment_2
  PRIVATE
  libglew_static
  assimp
  glfw
  glm
  OpenGL::GL)
