#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "timer.h"
#include <pthread.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "imports/stb_image.h"
#include "imports/stb_image_write.h"

int NTHREADS = 4;

typedef struct array2d_t {
  int height, width;
  uint8_t flex[];
} array2d_t;
#define get_array(arr2d) \
  _Generic( (arr2d),     \
            array2d_t*: (uint8_t(*)[(arr2d)->width])(arr2d)->flex)

typedef struct array2d_t_float {
  int height, width;
  float flex[];
} array2d_t_float;
#define get_array_float(arr2d) \
  _Generic( (arr2d),     \
            array2d_t_float*: (float(*)[(arr2d)->width])(arr2d)->flex)
//https://stackoverflow.com/questions/54709981/how-to-initiliaze-a-dynamic-2d-array-inside-a-struct-in-c


typedef struct imagem_rgba{
    int width, height, bpp;
    array2d_t *red, *green, *blue, *alpha;
} imagem_rgba;

typedef struct t_Args {
    int idThread, kernel_dimension;
    array2d_t *channel, *output_channel;
    array2d_t_float *kernel;
    char *color;
} t_Args;


imagem_rgba read(char* path) {
    imagem_rgba input;
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

void * dot_multiplication_matrix(void* arg) {
    float soma = 0;
    uint8_t soma_uint;
    t_Args *args = (t_Args *) arg;   


    //array2d_t* output_channel;
    //output_channel = malloc( sizeof *output_channel + sizeof(uint8_t[channel->height][channel->width]) );
    //output_channel->width = channel->width-2;
    //output_channel->height = channel->height-2;
    //printf("%d\n", args->channel->height);
    //printf("%d\n", args->channel->width);
    for (int x = args->idThread; x < args->output_channel->height; x += NTHREADS) {
        for (int y = 0; y < args->output_channel->width; y++) {
            for (int i = 0; i < args->kernel_dimension; i++) {
                for (int j = 0; j < args->kernel_dimension; j++) {
                    //printf("THREAD[%d][%c] ACESSOU O ITEM KERNEL[%d][%d] E CHANNEL[%d][%d]\n", args->idThread, args->color, i, j, i+x, j+y);
                    soma += get_array_float(args->kernel)[i][j]*get_array(args->channel)[i+x][j+y];
                    
                }
            }

            //if (x==0 && y==0)printf("%f\n", soma);
            soma_uint = (uint8_t)soma;
            //if (x==0 && y==0)printf("%d\n", soma_uint);
            get_array(args->output_channel)[x][y] = soma_uint;
            //if (x==0 && y==0)printf("%d\n", get_array(output_channel)[x][y]);
            soma = 0;
        }
    }
    /*for (int i = 0; i < output_channel->height; i++){
        for (int j = 0; j < output_channel->width; j++){
            printf("%d  ", get_array(output_channel)[i][j]);
        }
        printf("\n");
    }*/
    //return output_channel;
    pthread_exit(NULL);
}

array2d_t_float * create_median_kernel(int dimension) {
    array2d_t_float *median_kernel = malloc( sizeof *median_kernel + sizeof(float[dimension][dimension]) );
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            get_array_float(median_kernel)[i][j] = 1.0/(dimension*dimension);
        }
    }
    return median_kernel;
}



int main() {
    pthread_t tid_sistema[4];
    t_Args *arg;

    imagem_rgba input = read("./imagens_de_teste/teste.png");
    imagem_rgba padded = padding(input);

    for (int i = 0; i < 3; i++){
        for (int thread_id = 0; thread_id < NTHREADS; thread_id++) {
            arg = malloc(sizeof(t_Args));
            if (arg == NULL) {
                printf("--ERRO: malloc()\n"); exit(-1);
            }

            arg->idThread = thread_id;
            arg->kernel_dimension = 3;
            switch (i) {
                case 0:
                    arg->color = "r";
                    arg->channel = padded.red;
                    arg->output_channel = input.red;
                break;
                case 1:
                    arg->color = "g";
                    arg->channel = padded.green;
                    arg->output_channel = input.green;
                break;
                case 2:
                    arg->color = "b";
                    arg->channel = padded.blue;
                    arg->output_channel = input.blue;
                break;
            }

            arg->kernel = create_median_kernel(3);
            if (pthread_create(&tid_sistema[thread_id], NULL, dot_multiplication_matrix, (void*) arg)) {
                printf("--ERRO: pthread_create()\n"); exit(-1);
            }
        }
        
        for (int thread_id=0; thread_id<NTHREADS; thread_id++) {
            if (pthread_join(tid_sistema[thread_id], NULL)) {
                printf("--ERRO: pthread_join() \n"); exit(-1); 
            } 
        }
    }





    //array2d_t* vermelho = dot_multiplication_matrix(padded.red, 3, MEDIAN_KERNEL, input.red);
    //array2d_t* verde = dot_multiplication_matrix(b.green, 3, MEDIAN_KERNEL);
    //array2d_t* azul = dot_multiplication_matrix(b.blue, 3, MEDIAN_KERNEL);
    //input.red = vermelho; //input.green = verde; input.blue = azul;

    
    write(input, "./imagens_alteradas/teste_borrado.png");
    return 1;   
}

