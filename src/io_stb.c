//
// Created by dew on 1/14/26.
//
// stb implementationn macros:
// these must appear in exactly in exactly one .c file
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"

#include "upsr_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

upsr_image_t* upsr_load_image(const char *filename)
{
    int width , height , channels;
    /*
     * stbi_load:
     * reads compressed file(jpg/png/etc)
     * decodes it
     * allocates raw pixel buffer
     * returns uint8_t*
     *
     */
    unsigned char *raw = stbi_load(filename, &width, &height, &channels, 0);

if(!raw) {
    printf("ERROR :FAILED to load %s\n",filename);
    return NULL;

}
/*
 * CREATE OUR IMAGE STRUCTURE
 * THIS IS WHERE OWNERSHIP TRASNSFER TO US
 *
 */
    upsr_image_t *img = upsr_image_create(width,height,channels);
    if (!img) {
        stbi_image_free(raw);
        return NULL;
    }
    /*
     * COPY PIXEL DATA
     * WHY COPY
     * STB OWNS RAW MONEY
     * WE WANNT FULL CONTROL
     *
     */
int total = width *height *channels;
    for (int i=0; i<total; i++) {
        img->data[i] = raw[i];
    }
    /*
     * freestb memory After copying
     *
     */
    stbi_image_free(raw);
    return img;

}
int upsr_save_png(const char *filename,const upsr_image_t *img) {

    /*
     * Stride = bytess per row
     * width*channels(uint8_t=1 byte)
     */
    int stride = img->width*img->channels;
    return stbi_write_png(filename,img->width,img->height,img->channels,img->data,stride);
}