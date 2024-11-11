# Fractal Generator
Colouring book is a fractal generator that takes an NFSF file as input and generates graphics in an SVG file.

Used C17
File encoding with UTF-8
CMake version 3.26

Files are main.c and main.h

Libraries include: <stdio.h>, <string.h>, <stdlib.h>, <math.h>

# Usage

Should run this code in CLion IDE

Must provide an NFSF file stored in cmake-build-debug called "NFSF.txt" that contains NFSF instructions

Must provide an SVG file stored in cmake-build-debug called "output.svg" that will store the graphical output from the program

If a 'file format error' is printed, the NFSF file is not formatted according to NFSF specifications
