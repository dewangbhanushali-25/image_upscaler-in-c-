
cmake_minimum_required(VERSION 3.16)
project(image_upscaler C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(test_image
        src/image.c
        src/io_stb.c
        src/resize_nn.c
        src/resize_bilinear.c
        src/resize_bilinear_fixed.c
        src/resize_bicubic.c
        examples/test_image.c
)

target_include_directories(test_image PRIVATE
        include
        third_party/stb
)

target_link_libraries(test_image m)
