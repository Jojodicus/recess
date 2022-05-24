# RECESS
(**R**untime **E**rror **C**hecker for **E**xersises in **S**P using **S**hims)

Easily find unhandled errors in your code using shims, no recompiling needed!

This tool was designed for the C exercises in *Systemprogrammierung* (SP) at the *Friedrich-Alexander-University Erlangen*, although it can be used to test pretty much all compiled binaries. It works by intercepting certain system calls and sporadically making them return errors. If any part of the code does not handle these errors correctly, the program will behave unexpectedly or (in the worst case) crash.

## Installation
    $ git clone https://github.com/Jojodicus/recess
    $ cd recess
    $ make install
a relog may be required to make the tool available in your path (if that doesn't work, try [this](#help))

## Usage
To use the tool, simply run `recess`, followed by the name of the binary you want to test. For example:

    $ recess ./my_binary

<!-- @TODO nice list of available shims -->
To check if a certain method call is supported by recess, see if it's contained in [recess.c](src/recess.c).

## Customization
The default fail chance for libcalls is 10%. You can change this by running

    $ recess config [editor]

where editor is the name of your favorite text editor (or leave blank to use one of the default editors).

By changing the fail chance, you can make the tool more or less likely to fail. The value should be between 0 and 100, otherwise the default value of 10 will still be used.

If you really want, you can also edit the config by hand, the path to the config file is `~/.config/recess.cfg`.

### Config format

Recess is using [libconfig](https://hyperrealm.github.io/libconfig/), with every lib-/syscall having its own customizable fail chance in a [libconfig setting](https://hyperrealm.github.io/libconfig/libconfig_manual.html#Settings) format. The name of the setting is the name of the method that should be intercepted and the value is the chance that it will fail. There is also a setting named `default`, which is used as a fallback if the chance was not specified for a particular (supported) method.

#### Example configs

[the default config](recess.cfg)

```
default = 20;
malloc = 5;
```

```
calloc: 25
# this is a comment
default: 100 # always fail
```

---

## Help

### I can't run the tool, it says `command not found`
<!-- @TODO give copy paste for dynamically loading .profile even if .bash_profile is present, also: give better quality help for applying changes -->
Try logging out and in again. If that doesn't work, `~/.local/bin` may be missing in your `$PATH`. Try this:

    $ export PATH=$PATH:$HOME/.local/bin

If it stops working after a relog, add the line above to your `~/.bash_profile` (or if that doesn't exist, the `~/.profile`). Then relog for the changes to take effect.

### `recess config` says `No editor found`
The configurator for the environment variables $EDITOR and $VISUAL. If those aren't set it checks for the editors neovim, vim and nano (in that order). If none of those are found, it will terminate with an error. To fix this, set either of the environment variables, install one of the three editors or run `recess config <editor>` to specify the editor manually.
