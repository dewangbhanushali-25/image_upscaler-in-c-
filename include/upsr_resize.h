//
// Created by dew on 1/18/26.
//

#ifndef MAIN_UPSR_RESIZE_H
#define MAIN_UPSR_RESIZE_H
#include "upsr_image.h"
/*
 * nearest neighour resize
 *
 * src : source image (read-only)
 * dst_w,dst_h : desires output sizr
 *
 * returns :
 * newly allocated image on success
 * null on failure
 */

upsr_image_t* upsr_resize_bilinear(const upsr_image_t *src,int dst_w,int dst_h);

upsr_image_t* upsr_resize_bicubic(const upsr_image_t *src, int dst_w, int dst_h);
upsr_image_t* upsr_resize_nn(const upsr_image_t *src, int dst_w, int dst_h);



#endif //MAIN_UPSR_RESIZE_H