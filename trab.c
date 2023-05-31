#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

typedef struct array2d_t{
  int height, width;
  uint8_t flex[];
} array2d_t;
#define get_array(arr2d) \
  _Generic( (arr2d),     \
            array2d_t*: (uint8_t(*)[(arr2d)->width])(arr2d)->flex)

typedef struct array2d_t_double{
  int height, width;
  double flex[];
} array2d_t_double;
#define get_array_double(arr2d) \
  _Generic( (arr2d),     \
            array2d_t_double*: (double(*)[(arr2d)->width])(arr2d)->flex)
//https://stackoverflow.com/questions/54709981/how-to-initiliaze-a-dynamic-2d-array-inside-a-struct-in-c


typedef struct imagem_rgba{
    int width, height, bpp;
    array2d_t *red, *green, *blue, *alpha;
} imagem_rgba;

imagem_rgba read(char* path) {
    imagem_rgba input;
    uint8_t* rgb_image = stbi_load(path, &input.width, &input.height, &input.bpp, 0);

    input.red = malloc( sizeof *input.red + sizeof(uint8_t[input.height][input.width]) );
    input.green = malloc( sizeof *input.green + sizeof(uint8_t[input.height][input.width]) );
    input.blue = malloc( sizeof *input.blue + sizeof(uint8_t[input.height][input.width]) );
    input.alpha = malloc( sizeof *input.alpha + sizeof(uint8_t[input.height][input.width]) );
    input.red->width = input.green->width = input.blue->width = input.alpha->width = input.width;
    input.red->height = input.green->height = input.blue->height = input.alpha->height = input.height;


    for (int i = 0; i < input.height; i++) {
        for (int j = 0; j < input.width; j++) {
            for (int k = 0; k < input.bpp; k++){
                switch (k) {
                    case 0:
                        get_array(input.red)[i][j] = rgb_image[i*input.width*input.bpp+j*input.bpp+k];
                    break;
                    case 1:
                        get_array(input.green)[i][j] = rgb_image[i*input.width*input.bpp+j*input.bpp+k];
                    break;
                    case 2:
                        get_array(input.blue)[i][j] = rgb_image[i*input.width*input.bpp+j*input.bpp+k];
                    break;
                    case 3:
                        get_array(input.alpha)[i][j] = rgb_image[i*input.width*input.bpp+j*input.bpp+k];
                    break;
                }
            }
        }
    }
    return input;
}


int write(imagem_rgba input, char *path) {
    uint8_t* volta = malloc(sizeof(uint8_t) * input.width * input.height * input.bpp);

    for (int i = 0; i < input.height; i++) {
        for (int j = 0; j < input.width; j++) {
            for (int k = 0; k < input.bpp; k++){
                switch (k) {
                    case 0:
                        volta[i*input.width*input.bpp+j*input.bpp+k] = get_array(input.red)[i][j];
                    break;
                    case 1:
                        volta[i*input.width*input.bpp+j*input.bpp+k] = get_array(input.green)[i][j];;
                    break;
                    case 2:
                        volta[i*input.width*input.bpp+j*input.bpp+k] = get_array(input.blue)[i][j];;
                    break;
                    case 3:
                        volta[i*input.width*input.bpp+j*input.bpp+k] = get_array(input.alpha)[i][j];;
                    break;
                }
            }

        }
    }     
    stbi_write_png(path, input.width, input.height, input.bpp, volta, input.width*input.bpp);
    return 0;
}


imagem_rgba padding(imagem_rgba input) {
    imagem_rgba padded;
    padded.width = input.width + 2;
    padded.height = input.height + 2;
    padded.bpp = input.bpp;

    //printf("%d %d %d\n", padded.width, padded.height, padded.bpp);

    padded.red = malloc( sizeof *padded.red + sizeof(uint8_t[padded.height][padded.width]) );
    padded.green = malloc( sizeof *padded.green + sizeof(uint8_t[padded.height][padded.width]) );
    padded.blue = malloc( sizeof *padded.blue + sizeof(uint8_t[padded.height][padded.width]) );
    padded.alpha = malloc( sizeof *padded.alpha + sizeof(uint8_t[padded.height][padded.width]) );
    padded.red->width = padded.green->width = padded.blue->width = padded.alpha->width = padded.width;
    padded.red->height = padded.green->height = padded.blue->height = padded.alpha->height = padded.height;

    for (int i = 0; i < padded.height; i++){
        for (int j = 0; j < padded.width; j++){
            if (i >= 1 && j >= 1 && j < (padded.width-1) && i < (padded.height-1)){
                get_array(padded.red)[i][j] = get_array(input.red)[i-1][j-1];
                get_array(padded.green)[i][j] = get_array(input.green)[i-1][j-1];
                get_array(padded.blue)[i][j] = get_array(input.blue)[i-1][j-1];
                get_array(padded.red)[i][j] = get_array(input.red)[i-1][j-1];

            }
            else if (i == 0 && j != 0 && j != padded.width-1) {
                get_array(padded.red)[i][j] = get_array(input.red)[i][j-1];
                get_array(padded.green)[i][j] = get_array(input.green)[i][j-1];
                get_array(padded.blue)[i][j] = get_array(input.blue)[i][j-1];
                get_array(padded.alpha)[i][j] = get_array(input.alpha)[i][j-1];
            }
            else if (i == (padded.height-1) && j != 0 && j != padded.width-1) {
                get_array(padded.red)[i][j] = get_array(input.red)[i-2][j-1];
                get_array(padded.green)[i][j] = get_array(input.green)[i-2][j-1];
                get_array(padded.blue)[i][j] = get_array(input.blue)[i-2][j-1];
                get_array(padded.alpha)[i][j] = get_array(input.alpha)[i-2][j-1];
            }

            if (j == 0){
                if (i == 0) {
                    get_array(padded.red)[i][j] = get_array(input.red)[i][j];
                    get_array(padded.green)[i][j] = get_array(input.green)[i][j];
                    get_array(padded.blue)[i][j] = get_array(input.blue)[i][j];
                    get_array(padded.alpha)[i][j] = get_array(input.alpha)[i][j];
                }
                else if (i == (padded.height-1)) {
                    get_array(padded.red)[i][j] = get_array(input.red)[i-2][j];
                    get_array(padded.green)[i][j] = get_array(input.green)[i-2][j];
                    get_array(padded.blue)[i][j] = get_array(input.blue)[i-2][j];
                    get_array(padded.alpha)[i][j] = get_array(input.alpha)[i-2][j];
                }
                else{
                    get_array(padded.red)[i][j] = get_array(input.red)[i-1][j];
                    get_array(padded.green)[i][j] = get_array(input.green)[i-1][j];
                    get_array(padded.blue)[i][j] = get_array(input.blue)[i-1][j];
                    get_array(padded.alpha)[i][j] = get_array(input.alpha)[i-1][j];
                }
                
            }

            else if (j == (padded.width-1)){
                get_array(padded.red)[i][j] = get_array(padded.red)[i][j-1];
                get_array(padded.green)[i][j] = get_array(padded.green)[i][j-1];
                get_array(padded.blue)[i][j] = get_array(padded.blue)[i][j-1];
                get_array(padded.alpha)[i][j] = get_array(padded.alpha)[i][j-1];
            }
        }
    }


    return padded;
}

array2d_t* dot_multiplication_matrix(array2d_t* channel, int kernel_dimension, double (*kernel)[kernel_dimension]) {
    uint8_t soma = 0;
    array2d_t* output_channel;
    output_channel = malloc( sizeof *output_channel + sizeof(uint8_t[channel->height][channel->width]) );

    output_channel->width = channel->width-2;
    output_channel->height = channel->height-2;

    for (int x = 0; x < channel->height; x++){
        for (int y = 0; y < channel->width; y++){
            for (int i = 0; i < kernel_dimension; i++){
                for (int j = 0; j < kernel_dimension; j++){
                    soma += kernel[i][j]*get_array(channel)[i+x][j+y];
                    
                }
            }
            //soma = soma/9;
            //printf("%f\n", soma);
            get_array(output_channel)[x][y] = soma;
            soma = 0;
        }
    }
    /*for (int i = 0; i < output_channel->height; i++){
        for (int j = 0; j < output_channel->width; j++){
            printf("%d  ", get_array(output_channel)[i][j]);
        }
        printf("\n");
    }*/
    return output_channel;
}





int main() {
    double MEDIAN_KERNEL[3][3] = {{0,0,0},{0,1,0},{0,0,0}};
    //printf("%f\n", MEDIAN_KERNEL[0][0]);
    imagem_rgba a = read("cachorro.png");
    /*imagem_rgba b = padding(a);
    array2d_t* vermelho = dot_multiplication_matrix(b.red, 3, MEDIAN_KERNEL);
    array2d_t* verde = dot_multiplication_matrix(b.green, 3, MEDIAN_KERNEL);
    array2d_t* azul = dot_multiplication_matrix(b.blue, 3, MEDIAN_KERNEL);
    array2d_t* transparencia = dot_multiplication_matrix(b.alpha, 3, MEDIAN_KERNEL);
    a.red = vermelho; a.green = verde; a.blue = azul; a.alpha = transparencia;
    
    
    for (int i = 0; i < a.red->height; i++){
        for (int j = 0; j < a.red->width; j++){
            printf("%d  ", get_array(a.red)[i][j]);
        }
        printf("\n");
    }
    array2d_t* soma = dot_multiplication_matrix(b.red, 3, MEDIAN_KERNEL);
    for (int i = 0; i < soma->height; i++){
        for (int j = 0; j < soma->width; j++){
            printf("%.0f  ", get_array(soma)[i][j]);
        }
        printf("\n");
    }*/
    
    write(a, "./imagem_nada.png");
    return 1;   
}

