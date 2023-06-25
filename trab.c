#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "utils/stb_image.h"
#include "utils/stb_image_write.h"
#include "utils/timer.h"
#include "kernels.h"







typedef struct array2d_t{
  int height, width;
  uint8_t flex[];
} array2d_t;
#define get_array(arr2d) \
  _Generic( (arr2d),     \
            array2d_t*: (uint8_t(*)[(arr2d)->width])(arr2d)->flex)
//https://stackoverflow.com/questions/54709981/how-to-initiliaze-a-dynamic-2d-array-inside-a-struct-in-c


typedef struct rgba_image{
    int width, height, bpp;
    array2d_t *red, *green, *blue, *alpha;
} rgba_image;


rgba_image read(char* path) {
    rgba_image input;
    uint8_t* rgb_image = stbi_load(path, &input.width, &input.height, &input.bpp, 0);
    //stbi_write_png("sem_alteracao.png", input.width, input.height, input.bpp, rgb_image, input.width*input.bpp);


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

rgba_image read_greyscale(char* path) {
    rgba_image input;
    uint8_t* rgb_image = stbi_load(path, &input.width, &input.height, &input.bpp, 0);
    //stbi_write_png("sem_alteracao.png", input.width, input.height, input.bpp, rgb_image, input.width*input.bpp);

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
                        get_array(input.red)[i][j] = round(rgb_image[i*input.width*input.bpp+j*input.bpp+k]*0.299);
                    break;
                    case 1:
                        get_array(input.red)[i][j] += round(rgb_image[i*input.width*input.bpp+j*input.bpp+k]*0.587);
                    break;
                    case 2:
                        get_array(input.red)[i][j] += round(rgb_image[i*input.width*input.bpp+j*input.bpp+k]*0.114);
                    break;
                    case 3:
                        get_array(input.alpha)[i][j] = rgb_image[i*input.width*input.bpp+j*input.bpp+k];
                    break;
                }
            }
        }
    }
    input.bpp = 1;
    return input;
}


int write(rgba_image input, char *path) {
    uint8_t* full_image = malloc(sizeof(uint8_t) * input.width * input.height * input.bpp);

    for (int i = 0; i < input.height; i++) {
        for (int j = 0; j < input.width; j++) {
            for (int k = 0; k < input.bpp; k++){
                switch (k) {
                    case 0:
                        full_image[i*input.width*input.bpp+j*input.bpp+k] = get_array(input.red)[i][j];
                    break;
                    case 1:
                        full_image[i*input.width*input.bpp+j*input.bpp+k] = get_array(input.green)[i][j];;
                    break;
                    case 2:
                        full_image[i*input.width*input.bpp+j*input.bpp+k] = get_array(input.blue)[i][j];;
                    break;
                    case 3:
                        full_image[i*input.width*input.bpp+j*input.bpp+k] = get_array(input.alpha)[i][j];;
                    break;
                }
            }

        }
    }     
    stbi_write_png(path, input.width, input.height, input.bpp, full_image, input.width*input.bpp);
    return 0;
}


rgba_image padding(rgba_image input) {
    rgba_image padded;
    padded.width = input.width + 2;
    padded.height = input.height + 2;
    padded.bpp = input.bpp;


    padded.red = malloc( sizeof *padded.red + sizeof(uint8_t[padded.height][padded.width]) );
    padded.green = malloc( sizeof *padded.green + sizeof(uint8_t[padded.height][padded.width]) );
    padded.blue = malloc( sizeof *padded.blue + sizeof(uint8_t[padded.height][padded.width]) );
    //padded.alpha = malloc( sizeof *padded.alpha + sizeof(uint8_t[padded.height][padded.width]) );
    padded.red->width = padded.green->width = padded.blue->width = padded.width;
    padded.red->height = padded.green->height = padded.blue->height = padded.height;
    //padded.alpha->width = padded.width;
    //padded.alpha->height = padded.height;

    for (int i = 0; i < padded.height; i++){
        for (int j = 0; j < padded.width; j++){
            if (i >= 1 && j >= 1 && j < (padded.width-1) && i < (padded.height-1)){
                get_array(padded.red)[i][j] = get_array(input.red)[i-1][j-1];
                get_array(padded.green)[i][j] = get_array(input.green)[i-1][j-1];
                get_array(padded.blue)[i][j] = get_array(input.blue)[i-1][j-1];
                //get_array(padded.red)[i][j] = get_array(input.red)[i-1][j-1];

            }
            else if (i == 0 && j != 0 && j != padded.width-1) {
                get_array(padded.red)[i][j] = get_array(input.red)[i][j-1];
                get_array(padded.green)[i][j] = get_array(input.green)[i][j-1];
                get_array(padded.blue)[i][j] = get_array(input.blue)[i][j-1];
                //get_array(padded.alpha)[i][j] = get_array(input.alpha)[i][j-1];
            }
            else if (i == (padded.height-1) && j != 0 && j != padded.width-1) {
                get_array(padded.red)[i][j] = get_array(input.red)[i-2][j-1];
                get_array(padded.green)[i][j] = get_array(input.green)[i-2][j-1];
                get_array(padded.blue)[i][j] = get_array(input.blue)[i-2][j-1];
                //get_array(padded.alpha)[i][j] = get_array(input.alpha)[i-2][j-1];
            }

            if (j == 0){
                if (i == 0) {
                    get_array(padded.red)[i][j] = get_array(input.red)[i][j];
                    get_array(padded.green)[i][j] = get_array(input.green)[i][j];
                    get_array(padded.blue)[i][j] = get_array(input.blue)[i][j];
                    //get_array(padded.alpha)[i][j] = get_array(input.alpha)[i][j];
                }
                else if (i == (padded.height-1)) {
                    get_array(padded.red)[i][j] = get_array(input.red)[i-2][j];
                    get_array(padded.green)[i][j] = get_array(input.green)[i-2][j];
                    get_array(padded.blue)[i][j] = get_array(input.blue)[i-2][j];
                    //get_array(padded.alpha)[i][j] = get_array(input.alpha)[i-2][j];
                }
                else{
                    get_array(padded.red)[i][j] = get_array(input.red)[i-1][j];
                    get_array(padded.green)[i][j] = get_array(input.green)[i-1][j];
                    get_array(padded.blue)[i][j] = get_array(input.blue)[i-1][j];
                    //get_array(padded.alpha)[i][j] = get_array(input.alpha)[i-1][j];
                }
                
            }

            else if (j == (padded.width-1)){
                get_array(padded.red)[i][j] = get_array(padded.red)[i][j-1];
                get_array(padded.green)[i][j] = get_array(padded.green)[i][j-1];
                get_array(padded.blue)[i][j] = get_array(padded.blue)[i][j-1];
                //get_array(padded.alpha)[i][j] = get_array(padded.alpha)[i][j-1];
            }
        }
    }


    return padded;
}

//aqui fica diferente
void dot_multiplication_matrix(rgba_image *output_image, rgba_image *input_image, array2d_t_float *kernel, int kernel_type) {
    float sum_kernel_1 = 0, sum_kernel_2 = 0;
    uint8_t sum_uint;
    array2d_t *output_colors[3] = {output_image->red, output_image->green, output_image->blue};
    array2d_t *input_colors[3] = {input_image->red, input_image->green, input_image->blue};


    for (int c = 0; c < 3; c++){
        for (int x = 0; x < output_colors[c]->height; x++) {
            for (int y = 0; y < output_colors[c]->width; y++) {
                for (int i = 0; i < kernel->height; i++) {
                    for (int j = 0; j < kernel->width; j++) {
                        sum_kernel_1 += get_array_float(kernel)[i][j]*get_array(input_colors[c])[i+x][j+y];
                        if (kernel_type >= 2) { 
                            sum_kernel_2 += get_array_float(kernel)[j][i]*get_array(input_colors[c])[i+x][j+y];
                        }
                        
                    }
                }
                if (kernel_type >= 2) { //Este if teoricamente não é necessário, mas não queria piorar mais o arredondamento nos casos em que não é necessário
                    sum_kernel_1 = sqrt(pow(sum_kernel_1, 2) + pow(sum_kernel_2, 2)); 
                }
                sum_uint = (uint8_t) round(sum_kernel_1);
                get_array(output_colors[c])[x][y] = sum_uint;
                sum_kernel_1 = 0;
                sum_kernel_2 = 0;
            }
        }
    }


}





int main(int argc, char *argv[]) {
    /*
        argv[1] : input path
        argv[2] : output path
        argv[3] : kernel type
        argv[4] : kernel dimensions
    */
    double start, finish, elapsed;
    rgba_image input;

    GET_TIME(start);
    if (atoi(argv[3]) >= 2) {
       input = read_greyscale(argv[1]);
    }
    else {
        input = read(argv[1]);
    }
    GET_TIME(finish);
    elapsed = finish - start;
    printf("%e;", elapsed);


    GET_TIME(start);
    rgba_image padded = padding(input);
    GET_TIME(finish);
    elapsed = finish - start;
    printf("%e;", elapsed);

    GET_TIME(start);
    array2d_t_float *kernel = call_create_kernel(atoi(argv[3]), atoi(argv[4]));
    GET_TIME(finish);
    elapsed = finish - start;
    printf("%e;", elapsed);


    GET_TIME(start);
    dot_multiplication_matrix(&input, &padded, kernel, atoi(argv[3]));
    GET_TIME(finish);
    elapsed = finish - start;
    printf("%e;", elapsed);

    GET_TIME(start);
    write(input, argv[2]);
    GET_TIME(finish);
    elapsed = finish - start;
    printf("%e\n", elapsed);



    return 1;
}

