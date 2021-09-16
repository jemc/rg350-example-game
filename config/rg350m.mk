# Replace this with the path to the compiler in your prepared buildroot.
CC = /home/jemc/Documents/code/RG350_buildroot/output/host/usr/bin/mipsel-linux-gcc
SYSROOT = $(shell $(CC) --print-sysroot)
