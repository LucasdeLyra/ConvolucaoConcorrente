#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../../utils/stb_image.h"
#include "../../../utils/stb_image_write.h"
#include "../../../utils/print_txt.c"

/*
Utilize o comando abaixo para saber se há diferença entre as imagens salvas
    diff imagem_original.txt imagem_identidade.txt
    diff imagem_original.txt imagem_identidade_1.txt
    diff imagem_original.txt imagem_identidade_2.txt
    diff imagem_original.txt imagem_identidade_4.txt
    diff imagem_original.txt imagem_identidade_8.txt
    
*/

int main(){
    int original_width, original_height, original_bpp;
    int identity_width, identity_height, identity_bpp;

    uint8_t* original_image = stbi_load("../../../imagens_de_teste/teste.png", &original_width, &original_height, &original_bpp, 0);
    save_file("imagem_original.txt", original_height * original_width * original_bpp, original_image);

    uint8_t* identity_image = stbi_load("../../../imagens_alteradas/concorrente/teste_identidade_1.png", &identity_width, &identity_height, &identity_bpp, 0);
    save_file("imagem_identidade_1.txt", identity_width * identity_height * identity_bpp, identity_image);

    identity_image = stbi_load("../../../imagens_alteradas/concorrente/teste_identidade_2.png", &identity_width, &identity_height, &identity_bpp, 0);
    save_file("imagem_identidade_2.txt", identity_width * identity_height * identity_bpp, identity_image);

    identity_image = stbi_load("../../../imagens_alteradas/concorrente/teste_identidade_4.png", &identity_width, &identity_height, &identity_bpp, 0);
    save_file("imagem_identidade_4.txt", identity_width * identity_height * identity_bpp, identity_image);

    identity_image = stbi_load("../../../imagens_alteradas/concorrente/teste_identidade_8.png", &identity_width, &identity_height, &identity_bpp, 0);
    save_file("imagem_identidade_8.txt", identity_width * identity_height * identity_bpp, identity_image);

    identity_image = stbi_load("../../../imagens_alteradas/sequencial/teste_identidade.png", &identity_width, &identity_height, &identity_bpp, 0);
    save_file("imagem_identidade.txt", identity_width * identity_height * identity_bpp, identity_image);
}