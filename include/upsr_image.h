//
// Created by dew on 1/11/26.
//

#ifndef UPSR_IMAGE_H
#define UPSR_IMAGE_H
/*
 * why this headers exist exist
 * this file defines what an image is in your library .
 * every algothrim (rize, filter , rotate) will depend on this
 *
 * this file is to tell what image is
 * so no logic
 * contain no file format knowledge

 */

#include <stdint.h> // for uint8_t (excpicitlt 8 bit unsigned type)
/*
 * upsr_image_t
 * this struct represents an image after it has been decoded from a file format  like png , jpeg etc
 * once load its just raw pixels in  memory .
 */

typedef struct { // struct is the structure  and type struct is used for aliasing  like a can give and name to this structure so i dont have to write struct all the time

 int width; //pixel of pixels horizontall
 int height;//number of pixel verticall
 /*
  * channels
  * 1 = grayscale
  * 3 = rgb
  * 4 = RGBA(red, green and blue and alpha here)
  *  */
    int channels;
    /*
     * data:
     * pointer to a contiguous block of memory
     * layout (rgb example)
     * row 0  rgb rgb rgb
     * row 1
     * row 2
     *  total_size = width*height* channels bytes
     *
     *
     */
    uint8_t *data;
} upsr_image_t;
/*
 * allocate an image in memory
 * this is function
 * allocates the struct
 * allocatess pixel memory
 * does not initialize pixel values
 */
upsr_image_t *upsr_image_create(int width, int height, int channels);
/*
 * free image memory safely
 * always use this instead of free().
 *
 */
void upsr_image_free(upsr_image_t *image);



/*
 * GET POINTER TO PIXEL AT (X,Y).
 *
 * returns pointer to the First channel of that pixel.
 * example for rgb
 * p[0]= R
 * p[1]=G
 * p[2]=B
 *
 */
uint8_t* upsr_image_pixel(upsr_image_t *image,int x , int y);

#endif //UPSR_IMAGE_H