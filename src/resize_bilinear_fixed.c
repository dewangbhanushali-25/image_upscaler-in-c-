//
// Created by dew on 2/4/26.
//
#include <stdint.h>
#include "upsr_resize.h"
#include "upsr_image.h"
#include "upsr_boundary.h"
#include <stddef.h>
/*
 * FIXED POINT BILINEAR resize(q16.16)
 * No floats
 * determistic
 * embedded-friendly
 */
#define FP_SHIFT 16
#define FP_ONE (1 << FP_SHIFT)

upsr_image_t* upsr_resize_bilinear_fixed(const upsr_image_t *src,
                                         int dst_w,
                                         int dst_h)
{
    if (!src || dst_w <= 0 || dst_h <= 0)
        return NULL;

    upsr_image_t* dst =
        upsr_image_create(dst_w, dst_h, src->channels);
    if (!dst)
        return NULL;

    int scale_x = (src->width  << FP_SHIFT) / dst_w;
    int scale_y = (src->height << FP_SHIFT) / dst_h;

    for (int y_d = 0; y_d < dst_h; y_d++) {

        // pixel-center aligned source Y (fixed-point)
        int src_y_fp =
            ((((y_d << FP_SHIFT) + (FP_ONE >> 1)) * scale_y) >> FP_SHIFT)
            - (FP_ONE >> 1);

        int y0 = src_y_fp >> FP_SHIFT;
        int dy_fp = src_y_fp - (y0 << FP_SHIFT);

        y0 = upsr_clamp(y0, 0, src->height - 1);
        int y1 = upsr_clamp(y0 + 1, 0, src->height - 1);

        if (dy_fp < 0) dy_fp = 0;
        if (dy_fp > FP_ONE) dy_fp = FP_ONE;

        for (int x_d = 0; x_d < dst_w; x_d++) {

            int src_x_fp =
                ((((x_d << FP_SHIFT) + (FP_ONE >> 1)) * scale_x) >> FP_SHIFT)
                - (FP_ONE >> 1);

            int x0 = src_x_fp >> FP_SHIFT;
            int dx_fp = src_x_fp - (x0 << FP_SHIFT);

            x0 = upsr_clamp(x0, 0, src->width - 1);
            int x1 = upsr_clamp(x0 + 1, 0, src->width - 1);

            if (dx_fp < 0) dx_fp = 0;
            if (dx_fp > FP_ONE) dx_fp = FP_ONE;

            uint8_t *p_dst =
                dst->data + (y_d * dst->width + x_d) * dst->channels;

            const uint8_t *p00 =
                src->data + (y0 * src->width + x0) * src->channels;
            const uint8_t *p10 =
                src->data + (y0 * src->width + x1) * src->channels;
            const uint8_t *p01 =
                src->data + (y1 * src->width + x0) * src->channels;
            const uint8_t *p11 =
                src->data + (y1 * src->width + x1) * src->channels;

            for (int c = 0; c < dst->channels; c++) {

                int32_t top =
                    ((FP_ONE - dx_fp) * p00[c] +
                      dx_fp * p10[c]) >> FP_SHIFT;

                int32_t bottom =
                    ((FP_ONE - dx_fp) * p01[c] +
                      dx_fp * p11[c]) >> FP_SHIFT;

                int32_t value =
                    ((FP_ONE - dy_fp) * top +
                      dy_fp * bottom) >> FP_SHIFT;

                if (value < 0) value = 0;
                if (value > 255) value = 255;

                p_dst[c] = (uint8_t)value;
            }
        }
    }

    return dst;
}
