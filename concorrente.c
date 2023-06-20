#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "utils/stb_image.h"
#include "utils/stb_image_write.h"
#include "utils/timer.h"
#include "kernels.h"

int bloqueadas = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;


typedef struct array2d_t {
    int height, width;
    uint8_t flex[];
} array2d_t;
#define get_array(arr2d)    \
    _Generic( (arr2d),      \
            array2d_t*: (uint8_t(*)[(arr2d)->width])(arr2d)->flex)
//https://stackoverflow.com/questions/54709981/how-to-initiliaze-a-dynamic-2d-array-inside-a-struct-in-c


typedef struct rgba_image{
    int width, height, bpp;
    array2d_t *red, *green, *blue, *alpha;
} rgba_image;

typedef struct t_Args {
    int thread_id, NTHREADS;
    rgba_image *input_image, *output_image;
    array2d_t_float *kernel;
} t_Args;


void barreira(int nthreads) {
    pthread_mutex_lock(&x_mutex); //inicio secao critica
    if (bloqueadas == (nthreads-1)) { 
        //ultima thread a chegar na barreira
        pthread_cond_broadcast(&x_cond);
        bloqueadas=0;
    } else {
        bloqueadas++;
        pthread_cond_wait(&x_cond, &x_mutex);
    }
    pthread_mutex_unlock(&x_mutex); //fim secao critica
}




rgba_image read(char* path) {
    rgba_image input;
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
void * dot_multiplication_matrix(void* arg) {
    float sum = 0;
    uint8_t sum_uint;
    t_Args *args = (t_Args *) arg;   
    array2d_t *output_colors[3] = {args->output_image->red, args->output_image->green, args->output_image->blue};
    array2d_t *input_colors[3] = {args->input_image->red, args->input_image->green, args->input_image->blue};


    for (int c = 0; c < 3; c++){
        //printf("Thread %d começou a rodar para a cor %d\n", args->thread_id, c);
        for (int x = args->thread_id; x < output_colors[c]->height; x += args->NTHREADS) {
            for (int y = 0; y < output_colors[c]->width; y++) {
                for (int i = 0; i < args->kernel->height; i++) {
                    for (int j = 0; j < args->kernel->width; j++) {
                        sum += get_array_float(args->kernel)[i][j]*get_array(input_colors[c])[i+x][j+y];
                    }
                }
                sum_uint = (uint8_t) round(sum);
                get_array(output_colors[c])[x][y] = sum_uint;
                sum = 0;
            }
        }
        //printf("Thread %d se bloqueou na cor %d esperando as outras threads\n", args->thread_id, c);
        barreira(args->NTHREADS);
    }
    pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
    /*
        argv[1] : input path
        argv[2] : output path
        argv[3] : kernel type
        argv[4] : kernel dimensions
        argv[5] : number of threads
    */
    double start, finish, elapsed;

    pthread_t tid_sistema[atoi(argv[5])];
    t_Args *arg;

    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init (&x_cond, NULL);

    GET_TIME(start);
    rgba_image input = read(argv[1]);
    GET_TIME(finish);
    elapsed = finish - start;
    printf("%e; ", elapsed);

    GET_TIME(start);
    rgba_image padded = padding(input);
    GET_TIME(finish);
    elapsed = finish - start;
    printf("%e; ", elapsed);

    GET_TIME(start);
    array2d_t_float * kernel = call_create_kernel(atoi(argv[3]), atoi(argv[4]));
    GET_TIME(finish);
    elapsed = finish - start;
    printf("%e; ", elapsed);

    GET_TIME(start);
    for (int id = 0; id < atoi(argv[5]); id++) {
        arg = malloc(sizeof(t_Args));
        if (arg == NULL) {
            printf("--ERRO: malloc()\n"); exit(-1);
        }

        arg->thread_id = id;  
        arg->NTHREADS = atoi(argv[5]);
        arg->input_image = &padded;
        arg->output_image = &input;

        arg->kernel = kernel;
        
        if (pthread_create(&tid_sistema[id], NULL, dot_multiplication_matrix, (void*) arg)) {
            printf("--ERRO: pthread_create()\n"); exit(-1);
        }
    }
    
    for (int id=0; id < atoi(argv[5]); id++) {
        if (pthread_join(tid_sistema[id], NULL)) {
            printf("--ERRO: pthread_join() \n"); exit(-1); 
        } 
    }
    GET_TIME(finish);
    elapsed = finish - start;
    printf("%e; ", elapsed);

    
    write(input, argv[2]);
    GET_TIME(finish);
    elapsed = finish - start;
    printf("%e\n", elapsed);

    free(input);
    free(args);
    free(padded);
    free(tid_sistema);
    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&x_cond);

    return 1;   
}

// https://br.freepik.com/fotos-gratis/close-up-de-uma-abelha-amarela-polinizadora-flor-ai-generativa_41073554.htm#query=bee%204k&position=0&from_view=search&track=ais
// https://www.pexels.com/photo/adult-brown-tabby-cat-747795/
// https://www.publicdomainpictures.net/pt/view-image.php?image=148735&picture=panda-vermelho
// https://br.pinterest.com/pin/751397519065761954/