#define _GNU_SOURCE

// TODO: clean up includes
#include <dlfcn.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#ifndef CFG_PATH
#define CFG_PATH "/.config/recess.cfg"
#endif

#define UNINIT 0xFF
#define INITIN 0xFE
#define DEFAULT_CHANCE 100
#define RECESS_INIT if (g_chance == UNINIT) { recess_init(); }

// error chance
static volatile u_int8_t g_chance = UNINIT;

// TODO: remove debug statements
void recess_init(void) {
    // initializing
    g_chance = INITIN;

    // set random seed
    srand(time(NULL));

    fprintf(stderr, "Loading config file...\n");

    // get home environment variable
    char *home = getenv("HOME");
    if (home == NULL) {
        // oh oh
        fprintf(stderr, "Error: HOME environment variable not set. The heck are you doing?\n");
        exit(EXIT_FAILURE);
    }
    fprintf(stderr, "41\n");
    // build path to config file
    size_t home_len = strlen(home);
    fprintf(stderr, "44\n");
    size_t cfg_len = strlen(CFG_PATH);
    fprintf(stderr, "46\n");
    char *cfg_path = malloc(home_len + cfg_len + 1); // segfaults cuz stack overflow ahhh
    if (cfg_path == NULL) {
        // oh oh
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    fprintf(stderr, "53\n");
    strcpy(cfg_path, home);
    strcat(cfg_path, CFG_PATH);
    fprintf(stderr, "56\n");

    // open config file
    FILE *fp = fopen(cfg_path, "r");
    if (fp == NULL) {
        fprintf(stderr, "failed to open config file, using default chance\n");
        g_chance = DEFAULT_CHANCE;
        return;
    }

    // read chance
    char buf[16];
    if (fgets(buf, sizeof(buf), fp) == NULL) {
        fprintf(stderr, "failed to read config file, using default chance\n");
        g_chance = DEFAULT_CHANCE;
        return;
    }

    // parse chance from format chance=%d
    char *p = strchr(buf, '=');
    if (p == NULL) {
        fprintf(stderr, "failed to parse config file, using default chance\n");
        g_chance = DEFAULT_CHANCE;
    }
    p++;
    int chance = atoi(p);

    // check range
    if (chance < 0) {
        fprintf(stderr, "invalid chance %d, clamping to 0\n", chance);
        g_chance = 0;
        return;
    }
    if (chance > 100) {
        fprintf(stderr, "invalid chance %d, clamping to 100\n", chance);
        g_chance = 100;
        return;
    }

    // set chance
    g_chance = chance;
}

// could be changed a macro
static bool should_fail(void) {
    // dont break initialization's syscalls
    if (g_chance == INITIN) {
        return false;
    }

    return (rand() % 100) < g_chance;
}

// example
void *malloc(size_t size) {
    RECESS_INIT

    if (should_fail()) {
        fprintf(stderr, "malloc failed!\n");
        errno = ENOMEM;
        return NULL;
    }

    void *(*orig_malloc)(size_t) = dlsym(RTLD_NEXT, "malloc");
    return orig_malloc(size);
}

// TODO: add lots of other syscalls