#include "upsr_image.h"
#include<stdlib.h>//malloc,free
/*
 * UPSR_IMAGE_CREATE
 * ALLOCATES MEMORY FOR
 * 1 THE IMAGE STRUCTURE
 * THE RAW PIXEL BUFFER
 */

upsr_image_t* upsr_image_create(int width ,int height , int channels) {
/*
 * step 1 : validate inputs
 * negative or zero dimennsions make on sense.
 * always fail early
 *
 */
 if (width <= 0 || height <= 0 || channels <= 0)
  return NULL;

// step 2 allocate the image structure itself
 upsr_image_t *img = (upsr_image_t*) malloc(sizeof(upsr_image_t));
 if (!img)
  return NULL; // allocation failed
/*
 step 3 store metadata
*/
img ->width = width;
 img ->height = height;
 img ->channels = channels;
 /*
  * step 4 allocate pixel memory
  * why width * height * channels ?
  * reurn NULL to avoid leaks
  *
  */
 img -> data = (uint8_t*)malloc(width * height * channels);
 /*
  * if pixel allocation fails
  * free struct memory
  * return to null to avoid leaks
  *
  */

if (!img->data) {
 free(img);
 return NULL;

}
/*
 * at this point
 * img is valid
 * img ->data is valid
 * pixel values are UNINITIALIZED
 *
 */
 return img;
}
/*
 * uspr+image free
 * safely release memory
 * rule
 * who is allocate memory is responsible for freeing it
 */
void upsr_image_free(upsr_image_t *img) {
 if (!img)
  return;
 /*
  * free pixel buffer first
  * (reverse order of allocation)
  *
  */
 free(img -> data);
 /*
  * free the struct itself
  */
 free(img);
}
/*
 * upsr_image_pixel
 * converts 2d (x,y) coordinates into 1d memory address.
 * this is the most important function to understand
 */
uint8_t* upsr_image_pixel(upsr_image_t *img, int x, int y) {
  /*
   * memory layout reminder(RGB)
   * index =(row * width + column)*channels
   */
 return img->data + (y * img->width + x) * img->channels;

}