#include <stdio.h>
#include <omp.h> // Required header 

int main() {
    int array[100];
    #pragma omp parallel for
    for (int i = 0; i < 100; i++) {
        array[i] = i * 2; 
    }
    printf("Array: ");
    for (int i = 0; i < 100; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    return 0;
}