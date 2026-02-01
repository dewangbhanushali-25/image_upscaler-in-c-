//
// Created by dew on 1/19/26.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "upsr_image.h"
#include "upsr_boundary.h"
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
    float scale_x = (float)src->width / (float)dst_w;
    float scale_y = (float)src->height / (float)dst_h;
/*
 * loop over destination pixels
 *
 */
for (int y_d = 0; y_d < dst_h; y_d++) {
    /* map destination y to source y (floating)
     * using pixel-center alignment
     */
    float src_y = (y_d + 0.5f) * scale_y - 0.5f;
    int y0 = (int)floorf(src_y);
    int y1 = y0 + 1;
    float dy = src_y - y0;
    /*clamp Y to valid range*/
    if (y0 < 0)y0 = 0;
    if (y1 >=src->height)y1 = src->height - 1;

    for (int x_d = 0; x_d < dst_w; x_d++) {
/*
 * map destination x to source x
 */
        float src_x = (x_d + 0.5f) * scale_x - 0.5f;

        int x0 = (int)floorf(src_x);
        int x1 = x0 + 1;
        float dx = src_x - x0;
    /*Clamp X*/
        //if (x0 <0)x0 = 0;
        //if (x1 >=src->width)x1 = src->width - 1; # uncomment this if you what to test only till resize bilinear
        x0 = upsr_clamp(x0, 0, src->width  - 1);
        x1 = upsr_clamp(x1, 0, src->width  - 1);
        y0 = upsr_clamp(y0, 0, src->height - 1);
        y1 = upsr_clamp(y1, 0, src->height - 1);

        /*
         * get pointers to the 4 neighboring source pixel
         *
         */
        const uint8_t *p00 =  src->data + (y0 * src->width + x0) * src->channels;
        const uint8_t *p10 =  src->data + (y0 * src->width + x1) * src->channels;
        const uint8_t *p01 = src->data + (y1 * src->width + x1) * src->channels;
        const uint8_t *p11 = src->data + (y1 * src->width + x0) * src->channels;
// destination pixel pointer
uint8_t *p_dst =
    dst->data + (y_d * dst->width  + x_d) * dst->channels;
        /*
         * interpolate each channels independently
         */
        for (int c =0 ; c <dst->channels; c++) {
            float top =
                (1.0f - dx)* p00[c] + dx *p10[c];
            float bottom =
                (1.0f -dx) * p01[c] + dx * p11[c];
            float value =
                (1.0f - dy) * top +dy* bottom;
            // clamp to valid 8 bit range
            if (value < 0.0f) value = 0.0f;
            if (value >255.0f) value = 255.0f;
            //store result
            p_dst[c] = (uint8_t)( value + 0.5f );



        }

    }
}
return dst;
}