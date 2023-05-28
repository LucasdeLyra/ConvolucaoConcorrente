#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

#define CHANNEL_NUM 3

int write() {
    int width = 800; 
    int height = 800;

    uint8_t* rgb_image;
    rgb_image = malloc(width*height*CHANNEL_NUM);

    // Write your code to populate rgb_image here

    stbi_write_png("image.png", width, height, CHANNEL_NUM, rgb_image, width*CHANNEL_NUM);

    return 0;
}
int main() {
    int width, height, bpp;
    uint8_t* red, *green, *blue, *alpha, *volta;
    uint8_t* rgb_image = stbi_load("teste.png", &width, &height, &bpp, 0);
    red = malloc( width*height );
    green = malloc( width*height );
    blue = malloc( width*height );
    alpha = malloc( width*height );
    volta = malloc( width*height*bpp );

    printf("%d\n", width);
    printf("%d\n", height);
    printf("%d\n", bpp);


    for (int i=0; i<width*height*bpp; i++){
        switch (i%bpp){
            case 0:
                red[i/bpp] = rgb_image[i];
            break;
            case 1:
                green[i/bpp] = rgb_image[i];
            break;
            case 2:
                blue[i/bpp] = rgb_image[i];
            break;
            case 3:
                alpha[i/bpp] = rgb_image[i];
            break;
        }

    }

    for (int i = 0; i<20000; i++){ 
        red[i] = 0;
        green[i] = 0;
        blue[i] = 0;
        alpha[i] = 0;
    }
    for (int i=0; i<width*height*bpp; i++){
        switch (i%bpp){
            case 0:
                volta[i] = red[i/bpp];
            break;
            case 1:
                volta[i] = green[i/bpp];
            break;
            case 2:
                volta[i] = blue[i/bpp];
            break;
            case 3:
                volta[i] = alpha[i/bpp];
            break;
        }
    }
    stbi_write_png("image2.png", width, height, bpp, volta, width*bpp);

    
    stbi_image_free(rgb_image);
    
    return 0;
}

