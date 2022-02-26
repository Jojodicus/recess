#define _GNU_SOURCE

#include "recess.h"

/*
 * @TODO: documentation
 */

// @TODO: clean up includes
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dlfcn.h>

#include "config_utils.h"

// flag if failures are currently suppressed, accessible from outside
bool g_recess_suppressed = false;


void *malloc(size_t size) {
    if (should_fail()) {
        errno = ENOMEM;
        return NULL;
    }

    extern void *__libc_malloc(size_t);
    return __libc_malloc(size);
}

void *calloc(size_t nmemb, size_t size) {
    if (should_fail()) {
        errno = ENOMEM;
        return NULL;
    }

    extern void *__libc_calloc(size_t, size_t);
    return __libc_calloc(nmemb, size);
}

void free(void *ptr) {
    extern void __libc_free(void *);
    __libc_free(ptr);
}

void *realloc(void *ptr, size_t size) {
    if (should_fail()) {
        errno = ENOMEM;
        return NULL;
    }

    extern void *__libc_realloc(void *, size_t);
    return __libc_realloc(ptr, size);
}


// @TODO: add lots of other syscalls
