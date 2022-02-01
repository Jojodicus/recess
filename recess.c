#define _GNU_SOURCE

// TODO: clean up includes
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <dlfcn.h>

#ifndef FAIL_CHANCE
#define FAIL_CHANCE 100
#endif

static volatile bool lookup = false;

// could be changed a macro
static bool should_fail(void) {
    if (lookup) {
        return false;
    }
    return (rand() % 100) < FAIL_CHANCE;
}

void *malloc(size_t size) { // segfaults cuz dlsym uses our malloc ahhhhhh
    if (should_fail()) {
        errno = ENOMEM;
        return NULL;
    }

    void *(*orig_malloc)(size_t) = dlsym(RTLD_NEXT, "malloc");
    return orig_malloc(size);
}

// TODO: add lots of other syscalls