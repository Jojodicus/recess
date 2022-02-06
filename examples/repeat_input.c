#include <stdio.h>
#include <stdlib.h>

int main() {
    int *num = malloc(sizeof(int));

    scanf("%d", num);
    printf("number: %d\n", *num);
    
    free(num);
}