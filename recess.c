#define _GNU_SOURCE

/*
 * @TODO: documentation
 */

// @TODO: clean up includes
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dlfcn.h>

#ifndef FAIL_CHANCE
#define FAIL_CHANCE 50
#endif

// flag if rng has been seeded so we don't seed it more than necessary
static bool _g_rand_init = false;
// flag if failures are currently suppressed, accessible from outside
bool g_recess_suppressed = false;

static bool _should_fail(){
    // suppressed -> no failures
    if (g_recess_suppressed) {
        return false;
    }

    // initialize rng if necessary
    if (!_g_rand_init){
        srand(time(NULL));
        _g_rand_init = true;
    }

    // roll
    return rand() % 100 < FAIL_CHANCE;
}


void *malloc(size_t size) {
    if (_should_fail()) {
        errno = ENOMEM;
        return NULL;
    }

    extern void *__libc_malloc(size_t);
    return __libc_malloc(size);
}

// @TODO: add lots of other syscalls