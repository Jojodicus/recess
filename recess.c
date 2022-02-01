#define _GNU_SOURCE

// TODO: clean up includes
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dlfcn.h>

#ifndef FAIL_CHANCE
#define FAIL_CHANCE 0
#endif

static bool rand_init = false;

static bool should_fail(){
    if (!rand_init){
        srand(time(NULL));
        rand_init = true;
    }

    return rand() % 100 < FAIL_CHANCE;
}

static void *(*real_malloc)(size_t) = NULL;

static void malloc_init(void) {
    real_malloc = dlsym(RTLD_NEXT, "malloc");
    if (real_malloc == NULL) {
        fprintf(stderr, "dlsym failed: %s\n", dlerror());
    }
}

void *malloc(size_t size) { // still segfaults
    if (real_malloc == NULL) {
        malloc_init();
    }

    if (should_fail()) {
        errno = ENOMEM;
        return NULL;
    }

    return real_malloc(size);
}

// TODO: add lots of other syscalls