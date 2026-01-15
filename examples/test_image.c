//
// Created by dew on 1/14/26.
//
#include <stdio.h>
#include "upsr_image.h"
#include "upsr_io.h"
#include <math.h>
int main()
{
    upsr_image_t *img = upsr_load_image("input.png");
    if (!img) return 1;

    printf("loaded : %d * %d ,channels = %d\n",img->width, img->height, img->channels);
    /*
     * simple operation:
     * invert colors(very visible)
     */

    int total = img->width*img->height*img->channels;
    for (int i=0;i<total;i++) {
        img->data[i]=255 - img->data[i];
    }
upsr_save_png("output.png",img);
    upsr_image_free(img);
    return 0;



}