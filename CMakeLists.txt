cmake_minimum_required(VERSION 3.5)

project(main C)

# -------------------------------
# C standard
# -------------------------------
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# -------------------------------
# Compiler warnings (learning-friendly)
# -------------------------------
add_compile_options(-Wall -Wextra -Wpedantic)

# -------------------------------
# Include directories
# -------------------------------
include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/third_party/stb
)

# -------------------------------
# Executable (test program)
# -------------------------------
add_executable(test_image
        src/image.c
        src/io_stb.c
        src/resize_nn.c
        examples/test_image.c
        src/resize_bilinear.c
        src/resize_bicubic.c
        include/upsr_boundary.h
        examples/test.c
)
# -------------------------------
# Link math library (IMPORTANT!)
# -------------------------------
target_link_libraries(test_image m)
