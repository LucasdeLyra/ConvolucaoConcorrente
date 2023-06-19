#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../../utils/stb_image.h"
#include "../../../utils/stb_image_write.h"
#include "../../../utils/print_txt.c"

/*
Utilize o comando abaixo para saber se há diferença entre as imagens salvas
    diff imagem_original.txt imagem_salvada.txt

*/

int main(){
    int width, height, bpp;

    uint8_t* original_image = stbi_load("../../../imagens_de_teste/cachorro.png", &width, &height, &bpp, 0);
    stbi_write_png("./cachorro_sem_alteracao.png", width, height, bpp, original_image, width*bpp);

    uint8_t* saved_image = stbi_load("./cachorro_sem_alteracao.png", &width, &height, &bpp, 0);

    save_file("imagem_original.txt", height * width * bpp, original_image);
    save_file("imagem_salvada.txt", height * width * bpp, saved_image);

}