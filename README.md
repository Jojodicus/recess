# RECESS
(**R**untime **E**rror **C**hecker for **E**xersises in **S**P using **S**hims)

</p>

Easily find unhandled errors in your code using shims, no recompiling needed!

</p>

This tool was designed for the C exercises in *Systemprogrammierung* (SP) at the *Friedrich-Alexander-University Erlangen*, although it can be used to test pretty much all compiled binaries. It works by intercepting certain system calls and sporadically making them return errors. If any part of the code does not handle these errors correctly, the program will behave unexpectedly or (in the worst case) crash.

## Installation
    $ git clone https://github.com/Jojodicus/recess
    $ cd recess
    $ make install
a relog may be required to make the tool available in your path

## Usage
<!-- TODO -->
## Customization
<!-- TODO -->