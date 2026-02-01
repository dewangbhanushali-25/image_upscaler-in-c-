//
// Created by dew on 1/31/26.
//
#include <math.h>
#include <stdint.h>
#include <stddef.h>

#include "upsr_boundary.h"
#include "upsr_image.h"
#include "upsr_resize.h"
#include "upsr_boundary.h"


/*
 * cubic_weight x
 * computes the cubic interpolation weight
 * for a distance x
 * this is the Keys cubic kerne;
 * parameter
 * x : distance from the sample point
 * returns
 * weights contribution of that sample
 *
 */
static float cubic_weight(float x)
{
    /*
     * a = 0.5 catmull -rom spline
     * this gives good sharpness without instability
     */
    const float a = -0.5f;
    // kernel is symmetric so we use |x|
    x = fabsf(x);
    /*
     * for |x| < 1:
     * this region affects the closest samples
     */
    if (x < 1.0f) {
        return (a + 2.0f)*x*x*x - (a + 3.0f)*x*x + 1.0f;
    }
    /*
     * for 1 _< |x| <2 :
     * rhis region affects farther samples
     *
     */

    else if (x < 2.0f){
        return a*x*x*x - 5.0f*a*x*x + 8.0f*a*x - 4.0f*a;
    }//outside radius 2 -> no contribution

    else{
        return 0.0f;
    }

}

/*
 * upsr_resize bicubic
 * resizes an image using bicubic interpolation
 * uses 16 source pixels per output pixel
 * prduces sharper result than bilinear
 * slightly slower due to more math
 */
upsr_image_t* upsr_resize_bicubic(const upsr_image_t *src,int dst_w,int dst_h)
{
//   basic error handling and safety checks
    if (!src || dst_w<=0 || dst_h<=0)
        return NULL;
    /*
     * Allocate destination image
     * same number of channels as source
     */
    upsr_image_t* dst = upsr_image_create(dst_w,dst_h,src->channels);
    if (!dst)
        return NULL;
    /*
     * Scale factors
     * How destination coordinates map to source coordinate
     */
    float scale_x = (float)src->width / (float)dst_w;
    float scale_y = (float)src->height / (float)dst_h;

    // loop over every pixel in destination image
    for (int y_d = 0; y_d < dst_h; y_d++) {
        /*map destination Y to source Y (Floating point)
         *pixel-center aligned
         *
         */
        float src_y = (y_d + 0.5f) * scale_y - 0.5f;
        //Integer part of source Y
        int y_int = (int)floorf(src_y);
        for (int x_d = 0;x_d < dst_w; x_d++) {
            /*
             * map destination X to source X
             */
            float src_x = (x_d + 0.5f) * scale_x - 0.5f;
            //Integer part of source X
            int x_int = (int)floorf(src_x);
            //pointer to destination pixel 
            uint8_t *p_dst = dst->data + (y_d *dst->width + x_d) * dst->channels;
            // process wach color chaneel dependently
            for (int c = 0 ; c < dst->channels;c++) {
                /*
                 * Accumulators:
                 * sum : weight pixel sum
                 * wsum :sum of weights(for normalization)
                 */
                float sum = 0.0f;
                float wsum = 0.0f;  
                /*
                 * Iterate over 4 x4 neighourhood
                 * m ,n E {-1,0,1,2}
                 */
                for (int m = -1; m <= 2; m++) {
                    int y = y_int + m;
                    // clamp Y to image boundaries   
                    if (y < 0) y = 0;
                    if (y >= src->height) y = src->height - 1;
                    /*
                     * cubic weight in y direction
                     */
                    float wy = cubic_weight(src_y - y);
                    for (int n = -1; n <= 2; n++) {
                        int x = x_int + n;
                        // clamp X
                        /*if (x < 0) x = 0;
                        if (x >= src->width) x = src->width - 1;*/ // this is important till resize bicubic resize
                        x = upsr_clamp(x, 0, src->width - 1);
                        y = upsr_clamp(y, 0, src->height - 1);

                        //cubic weight in X direction
                        float wx = cubic_weight(src_x - x);
                        // combined 2D weight 
                        //(separable kernel)
                        float w = wx * wy;
                        // pointer to source pixel 
                        const uint8_t *p_src =
                            src->data + (y * src->width + x) * src->channels;
                        //Accumlate weighted value
                        sum += w * p_src[c];
                        wsum += w;
                    }
                }
                /*
                 * Norma;ize result 
                 * (important near boundaries)
                 */
                float value = (wsum != 0.0f) ? (sum / wsum) : 0.0f;
                // clamp to valid 8 bit range
                if (value < 0.0f)value = 0.0f;
                if (value >255.0f)value = 255.0f;
                
                // Store result (rounded)
                p_dst[c]= (uint8_t)(value + 0.5f);
                
            }
        }
    }
    return dst;
}