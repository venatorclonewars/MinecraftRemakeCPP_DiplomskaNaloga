cmake_minimum_required(VERSION 3.10)

# Define the project name
project(MyGLMProject)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Specify the path to GLM
# Option 1: If GLM is in a subdirectory
add_subdirectory(glm)

# Option 2: If GLM is installed via a package manager, find it
# find_package(glm REQUIRED)

# Add the executable target
add_executable(MyExecutable src/main.cpp)

# Link GLM to your executable
# If using add_subdirectory, this will automatically link GLM
target_link_libraries(MyExecutable glm)
