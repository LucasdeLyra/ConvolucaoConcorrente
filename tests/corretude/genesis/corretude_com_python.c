#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../../utils/stb_image.h"
#include "../../../utils/stb_image_write.h"
#include "../../../utils/print_txt.c"

/*
Esse é o teste para confirmar a corretude do programa sequencial em c comparando-o com o programa sequencial em python.
Como a primeira função implementada em C foi a de média, esta é a utilizada aqui. Possivelmente, futuramente rodaremos outros.

Utilize o comando abaixo para saber se há diferença entre as imagens salvas
    diff imagem_c.txt imagem_python.txt
    
*/

int main(){
    int python_width, python_height, python_bpp;
    int c_width, c_height, c_bpp;

    uint8_t* python_image = stbi_load("../../../convolucao_python/images/average0.png", &python_width, &python_height, &python_bpp, 0);
    save_file("imagem_python.txt", python_height * python_width * python_bpp, python_image);

    uint8_t* c_image = stbi_load("../../../imagens_alteradas/sequencial/teste_media.png", &c_width, &c_height, &c_bpp, 0);
    save_file("imagem_c.txt", c_width * c_height * c_bpp, c_image);

}