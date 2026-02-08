#include <stdio.h>
#include "upsr_image.h"
#include "upsr_io.h"
#include "upsr_resize.h"

int main()
{
    upsr_image_t *src = upsr_load_image("diddy.png");
    if (!src)
        return 1;

    int dst_w = src->width * 2;
    int dst_h = src->height * 2;

    upsr_image_t *nn =
        upsr_resize_nn(src, dst_w, dst_h);
    upsr_save_png("out_nn.png", nn);

    upsr_image_t *bilinear =
        upsr_resize_bilinear(src, dst_w, dst_h);
    upsr_save_png("out_bilinear.png", bilinear);

    upsr_image_t *bilinear_fixed =
        upsr_resize_bilinear_fixed(src, dst_w, dst_h);
    upsr_save_png("out_bilinear_fixed.png", bilinear_fixed);

    upsr_image_t *bicubic =
        upsr_resize_bicubic(src, dst_w, dst_h);
    upsr_save_png("out_bicubic.png", bicubic);

    upsr_image_free(src);
    upsr_image_free(nn);
    upsr_image_free(bilinear);
    upsr_image_free(bilinear_fixed);
    upsr_image_free(bicubic);

    printf("All outputs written successfully.\n");
    return 0;
}
