#include <stdio.h>
#include "upsr_image.h"
#include "upsr_io.h"
#include "upsr_resize.h"

int main()
{
    /* Load input image ONCE */
    upsr_image_t *src = upsr_load_image("diddy.png");
    if (!src) {
        printf("Failed to load image\n");
        return 1;
    }

    printf("Source: %d x %d, channels = %d\n",
           src->width, src->height, src->channels);

    /* Desired output size */
    int dst_w = src->width * 2;
    int dst_h = src->height * 2;

    /* ---------- Nearest Neighbor ---------- */
    upsr_image_t *nn =
        upsr_resize_nn(src, dst_w, dst_h);

    if (nn) {
        upsr_save_png("output_nn.png", nn);
        printf("Saved output_nn.png\n");
    }

    /* ---------- Bilinear ---------- */
    upsr_image_t *bilinear =
        upsr_resize_bilinear(src, dst_w, dst_h);

    if (bilinear) {
        upsr_save_png("output_bilinear.png", bilinear);
        printf("Saved output_bilinear.png\n");
    }

    /* ---------- Bicubic ---------- */
    upsr_image_t *bicubic =
        upsr_resize_bicubic(src, dst_w, dst_h);

    if (bicubic) {
        upsr_save_png("output_bicubic.png", bicubic);
        printf("Saved output_bicubic.png\n");
    }

    /*__--------------------bilearn_fixed-----------------*/
    upsr_image_t *bilinear_fixed = upsr_resize_bilinear_fixed(src,dst_w,dst_h);

    upsr_save_png("output_bilinear_fixed.png", bilinear_fixed);





    /* ---------- Cleanup ---------- */
    upsr_image_free(src);
    upsr_image_free(nn);
    upsr_image_free(bilinear);
    upsr_image_free(bicubic);

    printf("All resizing done.\n");
    return 0;
}
