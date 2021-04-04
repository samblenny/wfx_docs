# exp2

The objective of this experiment is to call a function from sl_wfx.h,
compile main.c, and get a linker error about missing symbols.


## Console Log

```bash
$ ls
Makefile  README.md main.c
$ make
gcc -I ../wfx_fmac_driver main.c
Undefined symbols for architecture x86_64:
  "_sl_wfx_init", referenced from:
      _main in main-e97240.o
ld: symbol(s) not found for architecture x86_64
clang: error: linker command failed with exit code 1 (use -v to see invocation)
make: *** [all] Error 1
$ ls
Makefile  README.md main.c
$
```
