#include <stdio.h>
int main(){
    int altura = 5;
    int largura = 5;
    int nova_largura = largura+2;
    int nova_altura = altura+2;

    int a[altura*largura];
    int b[nova_altura*nova_largura];

    for (int i = 0; i < altura; i++){
        for (int j = 0; j < largura; j++){
            a[i*largura+j] = i*largura+j+1;
        }
    }
    /*for (int i = 0; i < altura; i++){
        for (int j = 0; j < largura; j++){
            printf("%d ", a[i*largura+j]);
        }
        printf("\n");
    }
    printf("\n");*/

    for (int i = 0; i < nova_altura; i++){
        for (int j = 0; j < nova_largura; j++){
            if (i >= 1 && j >= 1 && j < (nova_largura-1) && i < (nova_altura-1)){
                b[i*nova_largura+j] = a[(i-1)*largura+(j-1)];
            }
            else if (i == 0 && j != 0 && j != nova_largura-1) {
                b[i*nova_largura+j] = a[i*largura+j-1];
            }
            else if (i == (nova_altura-1) && j != 0 && j != nova_largura-1) {
                b[i*nova_largura+j] = a[(i-2)*largura+j-1];
            }

            if (j == 0){
                if (i == 0) {
                    b[i*nova_largura+j] = a[i*largura+j];
                }
                else if (i == (nova_altura-1)) {
                    b[i*nova_largura+j] = a[(i-2)*largura+j];
                }
                else{
                    b[i*nova_largura+j] = a[(i-1)*largura+j];
                }
                
            }

            else if (j == (nova_largura-1)){
                b[i*nova_largura+j] = b[i*nova_largura+j-1];
            }
        }
    }
    for (int i = 0; i < nova_altura; i++){
        for (int j = 0; j < nova_largura; j++){
            printf("%d ", b[i*nova_largura+j]);
        }
        printf("\n");
    }
    printf("\n");

}