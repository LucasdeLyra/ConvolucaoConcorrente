#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../imports/stb_image.h"
#include "../../imports/stb_image_write.h"

int main(){
    int width, height, bpp;
    uint8_t* original_image = stbi_load("../../imagens_de_teste/cachorro.png", &width, &height, &bpp, 0);
    for (int i = 0; i < height * width * bpp; i++){
        printf("%d", original_image[i]);
    }
}