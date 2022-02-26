#ifndef CONFIG_UTILS_H
#define CONFIG_UTILS_H

#include <stdbool.h>

// @TODO: XDG compliant
// config will be in $HOME/CONFIG_NAME
#define CONFIG_NAME "/.config/recess.cfg"

#ifndef DEFAULT_FAIL_CHANCE
#define DEFAULT_FAIL_CHANCE 10
#endif

typedef struct {
    int fail_chance;
} config_t;

bool should_fail();

#endif // CONFIG_UTILS_H
