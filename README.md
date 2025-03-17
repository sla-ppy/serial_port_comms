## Usage:

```
1. Compile using:
clang src/main.c src/errors.c src/commands.c src/input.c -o main -Wall -Wextra -pedantic -std=gnu99 -gdwarf-4 -O0 -fsanitize=address,undefined

2. Run with one of the valid commands:
.main/ *ASKI:##
.main/ *ASKA:##
.main/ *SETG07:##
```

## Notes for the creator:

```
valgrind and asan are both tools to check for memory leaks, out of bound accessing and overflows
they are also super cool! :D

-gdwarf-4  =>  is a compilation flag we need to run valgrind succesfully after compiling our program
-fsanitize=address,undefined  =>  also known as "asan", which is clang's compilation flag we can use instead of valgrind (prefered method)
```
