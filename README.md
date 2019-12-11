# Advent of code 2019

![Advent of Code Logo](https://pbs.twimg.com/media/EFs21m0XYAIjq4T?format=jpg&name=large)

## Goals
The code is mostly written in C++17 following paradigms and style guides from modern C++ and tries to be as little sloppy or "quick 'n dirty" as possible.
My personal goal was to get decent/good performance while keeping the code clean and easy to read. For example, if the problem at hand can be solved with a logarithmic and quadratic solution, I will try to use the logarithmic approach following modern practices and keeping the code clean, to my abilities to do so.

Modern practices include, but are not limited to: (not listed by order of importance)

- using `std::string_view` whenever possible
- never trying to hand-code anything `<algorithm>` already provides
- never using `NULL`
- always preferring references to raw pointers
- using raw pointers only when they don't represent ownership (either unique or shared) of the object pointed to

## Project Structure
The project uses CMake as the build system (you can grab a copy [here](https://cmake.org/download) if you're on Windows or use your package manager of choice if you're on either Linux or MacOS). The build script should be runnable by any CMake version later than 3.0 but has been tested by me only with CMake 3.16.1.
The code should be compilable on any platform with a compiler that supports all C++17 features. I personally have tested it with `gcc 9.2.0` and `clang 9.0.0` but `MSVC` should also have no problems compiling it. (I will run some tests with `MSVC` once the event is over).

The `input` folder contains all the inputs that were assigned to me (which I wanted to keep a copy of, just in case :relieved:).  
The `src` folder contains the actual code. You can compile it with the following comomands, run in the project root:
```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release # Use `Debug` in order to get a debug build
cd build
cmake --build .
```
If everything went nice and smooth, all the built solutions should now live in `<project-root>/build/src` and are named `day_N`, where `N` is the day number of the challenge that solution refers to.
All the solutions are coded such that they take an input file (e.g. those found in the `input` folder) from the command line so that you can easily plug your own input file and get your answer. For example, assuming to be in the build root (e.g.`<project-root>/build`), the following commands will give you the answer for the sixth day's puzzle:
```bash
src/day_6 ../input/day_6.txt # with my input
src/day_6 ~/my-own-input.txt # with your own input
```

## Hacking on the code
The build script generates a `compile_commands.json` file in the build root which you can use with your C++ [language server](https://langserver.org/) of choice to get IDE-like features in your text editor, such as autocompletion, quickly jumping to method/class definitions or even hovering over the `auto` keyword in order to look at the actual type of a variable. Have fun!

## Can I suggest an improvement?
Absolutely! If you spot anything that can be cleaned up or improved, don't hesitate to file an issue or even send a PR. I constantly work on my code style and care about my code being as both performant and clean as possible!
