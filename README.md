lameduck
========

A brainfuck to x64 compiler written live in an hour during [CPPP](https://cppp.fr).

Requirements:

- [nasm](https://www.nasm.us/) installed and on the path
- [fmt](https://github.com/fmtlib/fmt) 
- A C++ toolchain (you'll likely need to fix up the paths in lameduck.cpp yourself since I hardcoded them for my setup)

## Building

It's a CMake project, so you can build the compiler through your IDE, or on the command line with the usual commands.

## Usage

```cpp
$ build/lameduck #compile hello.bf to ./hello or ./hello.exe
$ ./hello
```
