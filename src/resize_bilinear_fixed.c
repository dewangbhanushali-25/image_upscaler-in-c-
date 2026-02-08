#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "upsr_image.h"
#include "upsr_boundary.h"

#define FP_SHIFT 16
#define FP_ONE   (1 << FP_SHIFT)
#define FP_HALF  (1 << (FP_SHIFT - 1))

upsr_image_t* upsr_resize_bilinear_fixed(const upsr_image_t *src, int dst_w, int dst_h)
{
    if (!src || dst_w <= 0 || dst_h <= 0)
        return NULL;

    upsr_image_t *dst = upsr_image_create(dst_w, dst_h, src->channels);
    if (!dst)
        return NULL;

    // Fixed-point scale factors (16.16)
    int32_t scale_x = ((int32_t)src->width << FP_SHIFT) / dst_w;
    int32_t scale_y = ((int32_t)src->height << FP_SHIFT) / dst_h;

    for (int y_d = 0; y_d < dst_h; y_d++) {
        // Map destination y to source y: src_y = (y_d + 0.5) * scale - 0.5
        // In fixed point: ((y_d << 16) + FP_HALF) * scale >> 16 - FP_HALF
        int32_t src_y_fp = (int32_t)(((int64_t)(y_d * FP_ONE + FP_HALF) * scale_y) >> FP_SHIFT) - FP_HALF;

        int y0 = src_y_fp >> FP_SHIFT;
        int y1 = y0 + 1;
        int32_t dy = src_y_fp & (FP_ONE - 1); // Fractional part

        y0 = upsr_clamp(y0, 0, src->height - 1);
        y1 = upsr_clamp(y1, 0, src->height - 1);

        for (int x_d = 0; x_d < dst_w; x_d++) {
            int32_t src_x_fp = (int32_t)(((int64_t)(x_d * FP_ONE + FP_HALF) * scale_x) >> FP_SHIFT) - FP_HALF;

            int x0 = src_x_fp >> FP_SHIFT;
            int x1 = x0 + 1;
            int32_t dx = src_x_fp & (FP_ONE - 1); // Fractional part

            x0 = upsr_clamp(x0, 0, src->width - 1);
            x1 = upsr_clamp(x1, 0, src->width - 1);

            const uint8_t *p00 = src->data + (y0 * src->width + x0) * src->channels;
            const uint8_t *p10 = src->data + (y0 * src->width + x1) * src->channels;
            const uint8_t *p01 = src->data + (y1 * src->width + x0) * src->channels; // Fixed p01 index
            const uint8_t *p11 = src->data + (y1 * src->width + x1) * src->channels; // Fixed p11 index

            uint8_t *p_dst = dst->data + (y_d * dst->width + x_d) * dst->channels;

            for (int c = 0; c < dst->channels; c++) {
                // Fixed point linear interpolation:
                // val = p00 * (1-dx) + p10 * dx
                int32_t top = (p00[c] * (FP_ONE - dx) + p10[c] * dx) >> FP_SHIFT;
                int32_t bottom = (p01[c] * (FP_ONE - dx) + p11[c] * dx) >> FP_SHIFT;

                int32_t value = (top * (FP_ONE - dy) + bottom * dy) >> FP_SHIFT;

                // Rounding and Clamping
                // value = (value + (1 >> 0)) // Fixed point doesn't need float-style 0.5 round here
                if (value < 0) value = 0;
                if (value > 255) value = 255;

                p_dst[c] = (uint8_t)value;
            }
        }
    }
    return dst;
}
/*
 * 16.16 fixed point format
 * key logic chances
 * scaling :instead of float , we use int32_t shifted by 16
 * interploation : the weighhts (dx,dy) become integers between 0 and 65536
 * precision : we must be careful with multilications ; multiplying two 16.16 numbers results in 32.32 number , so we must shifet back dow,

 */e