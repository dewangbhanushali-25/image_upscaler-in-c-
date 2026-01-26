//
// Created by dew on 1/18/26.
//

#include "upsr_resize.h"
#include <stdlib.h>
/*
 * upsr_resize_nn
 * implements nearest neighbor upsaclling
 *
 * this function :
 * creates a new (larger or smaller ) image
 * maps each destination pixel to a source pixel
 * copies pixel values directly
 */
upsr_image_t* upsr_resize_nn(const upsr_image_t *src, int dst_w,int dst_h)
{
    /*
     * safety checks
     * src must exist
     * destination size must be valid
     */

    if (!src || dst_w <= 0 || dst_h <= 0)
        return NULL;
    /*
     *create destination image
     *important
     *same number of channels as source
     *new memory allocation
     */
    upsr_image_t  *dst = upsr_image_create(dst_w,dst_h,src->channels);

    if (!dst)
        return NULL;
    /*
     * core algorithm:
     * loop over every pixel in destination image
     * for each(x_d , y_d)
     * - compute corresponding (x_s, y_s)
     * copy pixel data
     */
    for (int y_d = 0; y_d < dst_h; y_d++) {
        /*
         * map destination y to source y
         * y_d/dst_h  -> normalized vertical position
         * * src-> height -> source coordinate
         */
        int y_s = (y_d *src->height)/dst_h;
        for (int x_d = 0; x_d < dst_w; x_d++) {
            /*
             * map destination x to source x
             */
            int x_s = (x_d *src->width)/dst_w;
            /*
             * compute pointer to source pixel
             * memory layout reminder
             * index = (y * width + x) * channels
             */
            const uint8_t *p_src =
                src->data + (y_s *src -> width +x_s) * src -> channels;
            /*
             * compute pointer to Destination pixel
             */
            uint8_t *p_dst =
                dst->data + (y_d* dst->width + x_d) * dst -> channels;
            /*
             * copy all channels
             * this makes the function
             * grayscale-safe(1 channels)
             * rgb -safe (3 channels)
             * rgba-safe(4 channels)
             */
            for (int c = 0; c < src-> channels; c++) {
                p_dst[c] = p_src[c];
            }
        }
    }

return dst;
}