//
// Created by dew on 1/19/26.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "upsr_image.h"
/*
 * upsr_resize_bilinear
 * ------------------------------\
 * performs bilinear interpolation
 *
 * compares to nearest neighor:
 * uses 4 source pixels insteas of 1
 * smoother result
 * slightly slower
 */
upsr_image_t* upsr_resize_bilinear(const upsr_image_t *src,int dst_w,int dst_h)
{
/*safety checks*/
    if (!src || dst_w<=0 || dst_h<=0)
        return NULL;
    /*
     * create destination image
     */
    upsr_image_t *dst =
        upsr_image_create(dst_w,dst_h,src->channels);
    if (!dst)
        return NULL;
    /*
     * scales factors:
     * how much dfestination space maps to spurce space
     *
     */
    float scales_x = (float)src->width / (float)dst_w;
    float scales_y = (float)src->height / (float)dst_h;
/*
 * loop over destination pixels
 *
 */
for (int y_d = 0; y_d < dst_h; y_d++) {
    /* map destination y to source y (floating)
     * using pixel-center alignment
     */
    float src_y = (y_d + 0.5f) * scales_y - 0.5f;
    int y0 = (int)floorf(src_y);
    int y1 = y0 + 1;
    float dy = src_y - y0;
    /*clamp Y to valid range*/
    if (y0 < 0)y0 = 0;
    if (y1 >=src->height)y1 = src->height - 1;

    for (int x_d = 0; x_d < dst_w; x_d++) {

    }
}

}