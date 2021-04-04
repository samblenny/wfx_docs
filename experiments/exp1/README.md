# exp1

The objective of this experiment is to include `sl_wfx.h` in a minimal C file
and compile without any errors about missing header files.


## Console Log

```bash
$ ls
main.c  Makefile
$ make
gcc -I ../wfx_fmac_driver main.c
$ ls
a.out  main.c  Makefile
$ ./a.out
$ make clean
rm a.out
$
```
