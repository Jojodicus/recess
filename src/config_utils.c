#define _GNU_SOURCE

#include "config_utils.h"

// @TODO: clean up includes
#include <errno.h>
#include <libconfig.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dlfcn.h>

#include "recess.h"

static config_t *_g_cfg = NULL;

static void _make_default_config() {
    config_clear(_g_cfg);
    config_setting_t *root = config_root_setting(_g_cfg);
    config_setting_add(root, "default", CONFIG_TYPE_INT);
}

static void _destroy_config() {
    g_recess_suppressed = true;
    config_destroy(_g_cfg);
    free(_g_cfg);
    g_recess_suppressed = false;
}

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
    if (sprintf(*path, "%s/%s", home, CONFIG_NAME) < 0) {
        return -1;
    }

    return 0;
}

static void _parse_config() {
    // suppress shims
    g_recess_suppressed = true;

    _g_cfg = malloc(sizeof(config_t));
    config_init(_g_cfg);

    // get config path
    char *path;
    if (_get_config_path(&path) != 0) {
        fprintf(stderr, "recess: failed to get config path\n");
        _make_default_config();
        g_recess_suppressed = false;
        return;
    }

    // open config file
    config_init(_g_cfg);
    if (config_read_file(_g_cfg, path) == CONFIG_FALSE) {
        fprintf(stderr, "%s:%d - %s\n", config_error_file(_g_cfg),
            config_error_line(_g_cfg), config_error_text(_g_cfg));
        _make_default_config();
        config_destroy(_g_cfg);
        free(path);
        g_recess_suppressed = false;
        return;
    }
    free(path);

    // cleanup
    atexit(&_destroy_config);

    // unsuppress failures
    g_recess_suppressed = false;
}

bool should_fail(const char *method){
    // suppressed -> no failures
    if (g_recess_suppressed) {
        return false;
    }

    // initialize rng if necessary
    if (_g_cfg == NULL) {
        srand(time(NULL));
        _parse_config();
    }

    // lookup
    int chance;
    if (config_lookup_int(_g_cfg, method, &chance) == CONFIG_FALSE) {
        if (config_lookup_int(_g_cfg, "default", &chance) == CONFIG_FALSE) {
            chance = DEFAULT_FAIL_CHANCE;
        }
    }

    // roll
    // clamp not neccessary as formula works regardless
    return rand() % 100 < chance;
}
