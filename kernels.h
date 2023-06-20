//https://www.youtube.com/watch?v=bYdlKtONMm0&ab_channel=SlideHunt
//https://stackoverflow.com/questions/54709981/how-to-initiliaze-a-dynamic-2d-array-inside-a-struct-in-c

#ifndef kernels
typedef struct array2d_t_float {
    int height, width;
    float flex[];
} array2d_t_float;
#define get_array_float(arr2d) \
  _Generic( (arr2d),     \
            array2d_t_float*: (float(*)[(arr2d)->width])(arr2d)->flex)

void print_float_square_matrix(array2d_t_float *array, int matrix_dimension) {
    for (int i = 0; i < matrix_dimension; i++) {
        for (int j = 0; j < matrix_dimension; j++) {
            printf("%f  ", get_array_float(array)[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

array2d_t_float * create_average_kernel(int dimension) {
    array2d_t_float *median_kernel = malloc( sizeof *median_kernel + sizeof(float[dimension][dimension]) );
    median_kernel->height = median_kernel->width = dimension;
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            get_array_float(median_kernel)[i][j] = 1.0/(dimension*dimension);
        }
    }
    return median_kernel;
}

array2d_t_float * create_identity_kernel(int dimension) {
    array2d_t_float *identity_kernel = malloc( sizeof *identity_kernel + sizeof(float[dimension][dimension]) );
    identity_kernel->height = identity_kernel->width = dimension;
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            if ( ( i == j ) && ( j == ceil(dimension / 2) ) ) {
                get_array_float(identity_kernel)[i][j] = 1.0;
            }
            else {
                get_array_float(identity_kernel)[i][j] = 0;
            }
        }
    }
    return identity_kernel;
}

array2d_t_float * create_prewitt_kernel_x(int dimension) {
    array2d_t_float *prewitt_kernel_x = malloc( sizeof *prewitt_kernel_x + sizeof(float[dimension][dimension]) );
    prewitt_kernel_x->height = prewitt_kernel_x->width = dimension;
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            if ( j == ceil( dimension  / 2 - 1 ) ) {
                get_array_float(prewitt_kernel_x)[i][j] = -1.0;
            }
            else if ( j == ceil( ( dimension + 1 ) / 2 ) ) {
                get_array_float(prewitt_kernel_x)[i][j] = 1.0;
            }
            else {
                get_array_float(prewitt_kernel_x)[i][j] = 0;    
            }
        }
    }
    print_float_square_matrix(prewitt_kernel_x, dimension);
    return prewitt_kernel_x;
}


array2d_t_float * create_prewitt_kernel_y(int dimension) {
    array2d_t_float *prewitt_kernel_y = malloc( sizeof *prewitt_kernel_y + sizeof(float[dimension][dimension]) );
    prewitt_kernel_y->height = prewitt_kernel_y->width = dimension;
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            if ( i == ceil( dimension / 2 - 1) ) {
                get_array_float(prewitt_kernel_y)[i][j] = -1.0;
            }
            else if ( i == ceil( ( dimension + 1 ) / 2 ) ) {
                get_array_float(prewitt_kernel_y)[i][j] = 1.0;
            }
            else {
                get_array_float(prewitt_kernel_y)[i][j] = 0;    
            }
        }
    }
    print_float_square_matrix(prewitt_kernel_y, dimension);
    return prewitt_kernel_y;
}



array2d_t_float * call_create_kernel(int type, int dimension) {
    switch (type){
        case 0:
            return create_identity_kernel(dimension);
        case 1:
            return create_average_kernel(dimension);
        case 2:
            return create_prewitt_kernel_x(dimension);
        case 3:
            return create_prewitt_kernel_y(dimension);
        default:
            return create_identity_kernel(dimension);
    }

}


#define kernels
#endif