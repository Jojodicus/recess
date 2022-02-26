#define _GNU_SOURCE

#include "config_utils.h"

// @TODO: clean up includes
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dlfcn.h>

#include "recess.h"

static config_t CFG = {
    .fail_chance = -1,
};

// @TODO XDG compliant
static int _get_config_path(char **path) {
    // get home directory
    char *home = getenv("HOME");
    if (home == NULL) {
        return -1;
    }

    // allocate memory for path
    *path = malloc(strlen(home) + strlen(CONFIG_NAME) + 1);
    if (*path == NULL) {
        return -1;
    }

    // build path
    strncpy(*path, home, strlen(home) + 1);
    strncat(*path, CONFIG_NAME, strlen(CONFIG_NAME) + 1);

    return 0;
}

static void _parse_config() {
    // suppress shims
    g_recess_suppressed = true;

    // get config path
    char *path;
    if (_get_config_path(&path) != 0) {
        fprintf(stderr, "recess: failed to get config path\n");
        CFG.fail_chance = DEFAULT_FAIL_CHANCE;
        g_recess_suppressed = false;
        return;
    }

    // open config file
    FILE *config = fopen(path, "r");
    free(path); // don't need path anymore
    if (config == NULL) {
        perror("recess - failed to open config file");
        CFG.fail_chance = DEFAULT_FAIL_CHANCE;
        g_recess_suppressed = false;
        return;
    }

    // read config file
    char *line = NULL;
    size_t len = 0;
    unsigned int line_num = 0;
    long fail_chance = -1;
    while (getline(&line, &len, config) != -1) {
        line_num++;

        // skip comments
        if (line[0] == '#') {
            continue;
        }

        // parse line @TODO: accept "key = value" as well instead of only "key=value"
        char *key = strtok(line, "=");
        char *value = strtok(NULL, "=");

        // check for valid key
        if (strcmp(key, "FAIL_CHANCE") != 0) {
            continue;
        }

        // check for valid value
        if (value == NULL) {
            fprintf(stderr, "recess - invalid value for FAIL_CHANCE at line %d, searching for valid value\n", line_num);
            continue;
        }

        // parse value
        fail_chance = strtol(value, NULL, 10);

        // check for valid range @TODO: maybe clamp instead of discrading
        if (fail_chance < 0 || fail_chance > 100) {
            fprintf(stderr, "recess - invalid value for FAIL_CHANCE at line %d, searching for valid value\n", line_num);
            continue;
        }

        // set fail chance
        CFG.fail_chance = (int) fail_chance;
        break;
    }

    // check for errors
    if (ferror(config)) {
        perror("recess - error while reading config file");
        CFG.fail_chance = DEFAULT_FAIL_CHANCE;
        g_recess_suppressed = false;
        return;
    }

    // close config file
    fclose(config);

    // free line
    free(line);

    // check if we found a valid value
    if (CFG.fail_chance == -1) {
        fprintf(stderr, "recess - failed to find valid value for FAIL_CHANCE, using default value\n");
        CFG.fail_chance = DEFAULT_FAIL_CHANCE;
    }

    // unsuppress failures
    g_recess_suppressed = false;
}

bool should_fail(){
    // suppressed -> no failures
    if (g_recess_suppressed) {
        return false;
    }

    // initialize rng if necessary
    if (CFG.fail_chance == -1) {
        srand(time(NULL));
        _parse_config();
    }

    // roll
    return rand() % 100 < CFG.fail_chance;
}
