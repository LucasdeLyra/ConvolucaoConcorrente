#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../../utils/stb_image.h"
#include "../../../utils/stb_image_write.h"
#include "../../../utils/print_txt.c"

/*
Aqui, tomamos a matriz gerada pelo programa sequencial como sendo verdadeira    
Utilize o comando abaixo para saber se há diferença entre as imagens salvas
    diff teste_media.txt teste_media_1.txt
    diff teste_media.txt teste_media_2.txt
    diff teste_media.txt teste_media_4.txt
    diff teste_media.txt teste_media_8.txt

*/

int main(){
    int average_width, average_height, average_bpp;

    uint8_t* average_image = stbi_load("../../../imagens_alteradas/concorrente/teste_media_1.png", &average_width, &average_height, &average_bpp, 0);
    save_file("teste_media_1.txt", average_width * average_height * average_bpp, average_image);

    average_image = stbi_load("../../../imagens_alteradas/concorrente/teste_media_2.png", &average_width, &average_height, &average_bpp, 0);
    save_file("teste_media_2.txt", average_width * average_height * average_bpp, average_image);

    average_image = stbi_load("../../../imagens_alteradas/concorrente/teste_media_4.png", &average_width, &average_height, &average_bpp, 0);
    save_file("teste_media_4.txt", average_width * average_height * average_bpp, average_image);

    average_image = stbi_load("../../../imagens_alteradas/concorrente/teste_media_8.png", &average_width, &average_height, &average_bpp, 0);
    save_file("teste_media_8.txt", average_width * average_height * average_bpp, average_image);

    average_image = stbi_load("../../../imagens_alteradas/sequencial/teste_media.png", &average_width, &average_height, &average_bpp, 0);
    save_file("teste_media.txt", average_width * average_height * average_bpp, average_image);
}