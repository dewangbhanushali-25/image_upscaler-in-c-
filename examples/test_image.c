#include <stdio.h>
#include "upsr_image.h"
#include "upsr_io.h"
#include "upsr_resize.h"

//upsr_image_t * upsr_resize_nn(upsr_image_t * src, int dst_w, int dst_h);

int main()
{
    // Load input image
    upsr_image_t *src = upsr_load_image("diddy.png");
    if (!src) {
        printf("Failed to load input.jpg\n");
        return 1;
    }

    printf("Source: %d x %d, channels = %d\n",
           src->width, src->height, src->channels);

    // Upscale by 2x
    int dst_w = src->width * 2;
    int dst_h = src->height * 2;

    upsr_image_t *dst = upsr_resize_bicubic(src, dst_w, dst_h);
    if (!dst) {
        printf("Resize failed\n");
        upsr_image_free(src);
        return 1;
    }

    // Save output
    upsr_save_png("output_bicubic.png", dst);

    // Cleanup
    upsr_image_free(src);
    upsr_image_free(dst);

    printf("Saved output_nn.png\n");
    return 0;
}
