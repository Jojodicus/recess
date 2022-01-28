#define _GNU_SOURCE

// TODO: clean up includes
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>

void *malloc(size_t size) {
    srand(time(0)); // this should not belong here

    // TODO: make chance customizable
    if (!(rand() % 2)) {
        errno = ENOMEM;
        return NULL;
    }

    void *(*orig_malloc)(size_t) = dlsym(RTLD_NEXT, "malloc");
    return orig_malloc(size);
}

// TODO: add lots of other syscalls