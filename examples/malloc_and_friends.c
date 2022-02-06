#include <stdlib.h>
#include <stdio.h>

int main() {
    int *mal = (int *) malloc(sizeof(int));
    *mal = 1;
    printf("malloc: %d\n", *mal);

    int *call = (int *) calloc(2, sizeof(int));
    call[0] = 1;
    call[1] = 2;
    printf("calloc: %d %d\n", call[0], call[1]);

    int *re = realloc(mal, 2 * sizeof(int));
    re[1] = 2;
    printf("realloc: %d %d\n", re[0], re[1]);

    free(call);
    free(re);
}