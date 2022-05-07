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

static config_t *parsed_config = NULL;

static void make_default_config()
{
    config_destroy(parsed_config);
    config_init(parsed_config);
    config_setting_t *root = config_root_setting(parsed_config);
    config_setting_t *default_int = config_setting_add(root, "default", CONFIG_TYPE_INT);
    config_setting_set_int(default_int, DEFAULT_FAIL_CHANCE);
}

static void destroy_config()
{
    recess_suppressed = true;
    config_destroy(parsed_config);
    free(parsed_config);
    recess_suppressed = false;
}

// @TODO XDG compliant
static int get_config_path(char **path)
{
    // get home directory
    char *home = getenv("HOME");
    if (home == NULL)
    {
        return -1;
    }

    // allocate memory for path
    *path = malloc(strlen(home) + strlen(CONFIG_NAME) + 1);
    if (*path == NULL)
    {
        return -1;
    }

    // build path
    if (sprintf(*path, "%s/%s", home, CONFIG_NAME) < 0)
    {
        return -1;
    }

    return 0;
}

static void parse_config()
{
    // suppress shims
    recess_suppressed = true;

    parsed_config = malloc(sizeof(config_t));
    config_init(parsed_config);

    // get config path
    char *path;
    if (get_config_path(&path) != 0)
    {
        fprintf(stderr, "recess: failed to get config path\n");
        make_default_config();
        recess_suppressed = false;
        return;
    }

    // open config file
    config_init(parsed_config);
    FILE *file = fopen(path, "r");
    if (file != NULL)
    {
        fclose(file);
        if (config_read_file(parsed_config, path) != CONFIG_FALSE)
        {
            free(path);

            // cleanup
            atexit(destroy_config);

            // unsuppress failures
            recess_suppressed = false;
            return;
        }
    }
    // Something went wrong. We hate that.
    fprintf(stderr, "%s:%d - %s\n", config_error_file(parsed_config),
            config_error_line(parsed_config), config_error_text(parsed_config));
    make_default_config();
    config_destroy(parsed_config);
    free(path);
    recess_suppressed = false;
    return;
}

bool should_fail(const char *method)
{
    // suppressed -> no failures
    if (recess_suppressed)
    {
        return false;
    }

    // initialize rng if necessary
    if (parsed_config == NULL)
    {
        srand(time(NULL));
        parse_config();
    }

    // lookup
    int chance;
    if (config_lookup_int(parsed_config, method, &chance) == CONFIG_FALSE)
    {
        if (config_lookup_int(parsed_config, "default", &chance) == CONFIG_FALSE)
        {
            chance = DEFAULT_FAIL_CHANCE;
        }
    }

    // roll
    // clamp not neccessary as formula works regardless
    return rand() % 100 < chance;
}
