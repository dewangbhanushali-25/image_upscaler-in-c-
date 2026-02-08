# image_upscaler-in-c
This project is a form-scratch implementation of image upscaling algorithms in C.The goal is to understand image interpolation at pixel level, ,memory layout
The goal is to understand image interpolation at the pixel level, memory layout, and the trade-offs between floating-point and fixed-point arithmetic,especially in the context of embedded and camera systems

 ## motivation
 i just wanted to practice my c skill and overall image processing understanding , i wanted a cool wallpaper for my laptop , but all my favourite wallpaper had low resolution ,I searched for some and wanted to understand the engineering  behind that , further i am gonna add some gen ai elements or kind of diffusion model to increase the resoultion of the image
 
this project deals with image resizing actually work at pixel level , including coordinate mapping , boundary mapping , boundary handling and numerical precision 
this proejct also explores fixed pint arithmetic to better understand constraints commonly found in embedded and high speed camera systems


## Features

- Nearest Neighbor interpolation
- Bilinear interpolation (floating-point)
- Bilinear interpolation (fixed-point, Q16.16)
- Bicubic interpolation
- Explicit boundary handling
- PNG / JPG image loading and saving using stb_image

## Resizing Algorithms

### Nearest Neighbor
The simplest resizing method. Each output pixel copies the value
of the closest source pixel. This method is fast but produces
blocky artifacts.

### Bilinear Interpolation
Bilinear interpolation uses four neighboring pixels to compute
each output pixel as a weighted average. This produces smoother
results but requires more computation.

### Fixed-Point Bilinear Interpolation
To avoid floating-point arithmetic, bilinear interpolation was
reimplemented using fixed-point math (Q16.16 format). This approach
is deterministic and more suitable for embedded systems where
floating-point hardware may be limited or unavailable.

### Bicubic Interpolation
Bicubic interpolation uses a 4×4 neighborhood of pixels and
higher-order interpolation to produce sharper results, at the
cost of increased computation.


## Fixed-Point Arithmetic

Fixed-point arithmetic represents fractional values using integers.
In this project, a Q16.16 format is used, where the upper 16 bits
represent the integer part and the lower 16 bits represent the
fractional part.

This approach allows interpolation to be performed without
floating-point operations, making the implementation more suitable
for real-time and embedded image processing pipelines.

## Building and Running

Requirements:
- C compiler (GCC or Clang)
- CMake

Build:
```bash
mkdir build
cd build
cmake ..
make

./test_image



---

## 8️⃣ Debugging & Lessons

This section is *huge* for credibility.

```md
## Debugging Lessons Learned

During development, several non-obvious bugs were encountered,
including issues related to build configuration, image loading,
and fixed-point arithmetic.

One notable issue resulted in smooth gradient artifacts in the output
image, which was eventually traced back to incorrect loop bounds and
image I/O integration rather than interpolation math itself.

This experience highlighted the importance of verifying assumptions
at every layer, from image loading to numerical computation.


## Limitations and Future Work

- No SIMD optimizations
- No GPU or CUDA backend
- No color space conversions
- Performance not yet benchmarked

Future work could include separable filtering, SIMD acceleration,
or integration into a real-time camera pipeline.

