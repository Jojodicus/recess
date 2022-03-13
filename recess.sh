#!/bin/sh

# @TODO: XDG path
LIB_PATH="$HOME/.local/bin/librecess.so"
shim() {
    if test -f $LIB_PATH; then
        # execute
        LD_PRELOAD=$LIB_PATH $@
    else
        echo "recess library not found"
        exit 1
    fi
}

# @TODO: XDG path
CFG_PATH="$HOME/.config/recess.cfg"
config() {
    # check if editor is specified
    if test -n "$1"; then
        $1 $CFG_PATH
    elif test -n "$EDITOR" then # $EDITOR
        $EDITOR $CFG_PATH
    elif test -n "$VISUAL" then # $VISUAL
        $VISUAL $CFG_PATH
    elif command -v nvim >/dev/null 2>&1; then # neovim
        nvim $CFG_PATH
    elif command -v vim >/dev/null 2>&1; then # vim
        vim $CFG_PATH
    elif command -v nano >/dev/null 2>&1; then # nano
        nano $CFG_PATH
    else
        echo "No editor found" # ???
        exit 1
    fi
}

# main
if [ $# -eq 0 ]; then
    echo "Usage: recess <cmd> | config [editor]"
    exit 1
elif [ $1 = "config" ]; then
    config $2
else
    shim $@
fi
