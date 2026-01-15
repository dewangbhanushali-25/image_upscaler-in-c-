//
// Created by dew on 1/14/26.
//

#ifndef UPSR_IO_H
#define UPSR_IO_H
#include "upsr_image.h"
/*
 * load an image file
 * return
 * pointer to upsr_image_t on success
 * null on failure
 *
 * important
 * returned image is owned by you
 * you must free it qith upsr_image_free()
 *
 */

upsr_image_t* upsr_load_image(const char *filename);
/*
 * save an image as PNG
 *
 * returns
 * non-zero op success
 * 0 on failure
 *
 *
 */
int upsr_save_png(const char *filename, const upsr_image_t *img);

#endif //UPSR_IO_H