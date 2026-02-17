#include <stdio.h>
#include <stdlib.h>

long long vector_cycle(const int* x, const int* y, int length) {
    long long s;
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            s += x[i] * y[j];       
        }
    }
    return s;
}

int* create_vector(int length) {
    int* vector = malloc(sizeof(int) * length);
    if (!vector) return NULL;
    for (int i = 0; i < length; i++) { vector[i] = rand(); }
    return vector;
}

void delete_vector(int** v) {
    if (*v) free(*v);
    *v = NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        puts("Specify the vector length");
        puts("Usage:");
        printf("    %s [vector length]\n", argv[0]);
        return 0;
    }

    const int length = atoi(argv[1]);
    int* x =  create_vector(length);
    int* y = create_vector(length);

    if (!x || !y) {
        delete_vector(&x);
        delete_vector(&y);
        puts("Bad malloc. May be not enough memory?");
        return 1;
    }

    long long s = vector_cycle(x, y, length);
    printf("%lld\n", s);
    
    return 0;
}
