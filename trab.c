#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

#define CHANNEL_NUM 3

    //PROCCESS
    /*for (int i = 0; i<20000; i++) { 
        input.red[i] = 0;
        input.green[i] = 0;
        input.blue[i] = 0;
        input.alpha[i] = 0;
    }*/
typedef struct imagem_rgba{
    int width, height, bpp;
    uint8_t *red, *green, *blue, *alpha;
} imagem_rgba;


int write(imagem_rgba input) {
    uint8_t* volta = malloc(input.width * input.height * input.bpp);
    for (int i = 0; i < input.height*input.width*input.bpp; i++) {
            switch (i%input.bpp) {
                case 0:
                    volta[i] = input.red[i/input.bpp];
                break;
                case 1:
                    volta[i] = input.green[i/input.bpp];
                break;
                case 2:
                    volta[i] = input.blue[i/input.bpp];
                break;
                case 3:
                    volta[i] = input.alpha[i/input.bpp];
                break;
        }
        
    }
    
    stbi_write_png("image1.png", input.width, input.height, input.bpp, volta, input.width*input.bpp);
    return 0;
}

imagem_rgba read(char* path) {
    imagem_rgba input;
    uint8_t* rgb_image = stbi_load(path, &input.width, &input.height, &input.bpp, 0);

    input.red = malloc(input.width * input.height);
    input.green = malloc(input.width * input.height);
    input.blue = malloc(input.width * input.height);
    input.alpha = malloc(input.width * input.height);
    uint8_t* volta;

    
    for (int i=0; i<input.height*input.width*input.bpp; i++) {
        switch (i%input.bpp) {
            case 0:
                input.red[i/input.bpp] = rgb_image[i];
            break;
            case 1:
                input.green[i/input.bpp] = rgb_image[i];
            break;
            case 2:
                input.blue[i/input.bpp] = rgb_image[i];
            break;
            case 3:
                input.alpha[i/input.bpp] = rgb_image[i];
            break;
        }


    }


    return input;

}

imagem_rgba padding(imagem_rgba input) {
    imagem_rgba padded;
    padded.width = input.width + 2;
    padded.height = input.height + 2;
    padded.bpp = input.bpp;

    printf("%d %d %d\n", padded.width, padded.height, padded.bpp);

    padded.red = malloc(padded.width * padded.height);
    padded.green = malloc(padded.width * padded.height);
    padded.blue = malloc(padded.width * padded.height);
    padded.alpha = malloc(padded.width * padded.height);

    for (int i = 0; i < padded.height; i++){
        for (int j = 0; j < padded.width; j++){
            if (i >= 1 && j >= 1 && j < (padded.width-1) && i < (padded.height-1)){
                padded.red[i*padded.width+j] = input.red[(i-1)*input.width+(j-1)];
                padded.green[i*padded.width+j] = input.green[(i-1)*input.width+(j-1)];
                padded.blue[i*padded.width+j] = input.blue[(i-1)*input.width+(j-1)];
                padded.red[i*padded.width+j] = input.red[(i-1)*input.width+(j-1)];

            }
            else if (i == 0 && j != 0 && j != padded.width-1) {
                padded.red[i*padded.width+j] = input.red[i*input.width+j-1];
                padded.green[i*padded.width+j] = input.green[i*input.width+j-1];
                padded.blue[i*padded.width+j] = input.blue[i*input.width+j-1];
                padded.alpha[i*padded.width+j] = input.alpha[i*input.width+j-1];
            }
            else if (i == (padded.height-1) && j != 0 && j != padded.width-1) {
                padded.red[i*padded.width+j] = input.red[(i-2)*input.width+j-1];
                padded.green[i*padded.width+j] = input.green[(i-2)*input.width+j-1];
                padded.blue[i*padded.width+j] = input.blue[(i-2)*input.width+j-1];
                padded.alpha[i*padded.width+j] = input.alpha[(i-2)*input.width+j-1];
            }

            if (j == 0){
                if (i == 0) {
                    padded.red[i*padded.width+j] = input.red[i*input.width+j];
                    padded.green[i*padded.width+j] = input.green[i*input.width+j];
                    padded.blue[i*padded.width+j] = input.blue[i*input.width+j];
                    padded.alpha[i*padded.width+j] = input.alpha[i*input.width+j];
                }
                else if (i == (padded.height-1)) {
                    padded.red[i*padded.width+j] = input.red[(i-2)*input.width+j];
                    padded.green[i*padded.width+j] = input.green[(i-2)*input.width+j];
                    padded.blue[i*padded.width+j] = input.blue[(i-2)*input.width+j];
                    padded.alpha[i*padded.width+j] = input.alpha[(i-2)*input.width+j];
                }
                else{
                    padded.red[i*padded.width+j] = input.red[(i-1)*input.width+j];
                    padded.green[i*padded.width+j] = input.green[(i-1)*input.width+j];
                    padded.blue[i*padded.width+j] = input.blue[(i-1)*input.width+j];
                    padded.alpha[i*padded.width+j] = input.alpha[(i-1)*input.width+j];
                }
                
            }

            else if (j == (padded.width-1)){
                padded.red[i*padded.width+j] = padded.red[i*padded.width+j-1];
                padded.green[i*padded.width+j] = padded.green[i*padded.width+j-1];
                padded.blue[i*padded.width+j] = padded.blue[i*padded.width+j-1];
                padded.alpha[i*padded.width+j] = padded.alpha[i*padded.width+j-1];
            }
        }
    }

    return padded;
}
//uint8_t* convolution2D(uint8_t* channel, int width, int height){

//}


int main() {
    imagem_rgba a = read("teste.png");
    imagem_rgba b = padding(a);
    write(b);
    return 1;
}

