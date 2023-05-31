#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"


typedef struct array2d_t{
  int height, width;
  uint8_t flex[];
} array2d_t;

typedef struct imagem {
    int tamanho;
    array2d_t *red, *green, *blue, *alpha;
} imagem;

#define get_array(arr2d) \
  _Generic( (arr2d),     \
            array2d_t*: (uint8_t(*)[(arr2d)->width])(arr2d)->flex)
//https://stackoverflow.com/questions/54709981/how-to-initiliaze-a-dynamic-2d-array-inside-a-struct-in-c

int main (void)
{
  int height;
  int width;
  int bpp;
  imagem img;
  uint8_t* rgb_image = stbi_load("./teste.png", &width, &height, &bpp, 0);
  img.red = malloc( sizeof *img.red + sizeof(uint8_t[height][width]) );
  img.red->height = height;
  img.red->width = width;
  

  for(size_t i=0; i<img.red->height; i++)
  {
    for(size_t j=0; j<img.red->width; j++)
    {
      get_array(img.red)[i][j] = 10000+i+j;
      printf("%d ", get_array(img.red)[i][j]);
    }
    printf("\n");
  }

  free(img.red);
  return 0; 
}