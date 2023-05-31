#include <stdio.h>
#include <stdlib.h>
int main(){
    printf("%f", (1.0/9));
    int a[9];
    for (int i = 0; i<3; i++){
        for (int j = 0; j<3; j++){
            a[i][j] = i*3+j;
        }
    }
    printf("%f", a[0][0]);
}
//printf("%d\n", get_array(a.red)[0][0]);
//printf("%d\n", a.red->width == vermelho->width);
//printf("%d\n", a.red->height == vermelho->height);
/*for (int i = 0; i < a.red->height; i++){
    for (int j = 0; j < a.red->width; j++){
        printf("%d ", get_array(a.red)[i][j]);
    }
    printf("\n");
}
//array2d_t* soma = dot_multiplication_matrix(b.red, 3, MEDIAN_KERNEL);
for (int i = 0; i < soma->height; i++){
    for (int j = 0; j < soma->width; j++){
        printf("%.0d  ", get_array(soma)[i][j]);
    }
    printf("\n");
}*/