# another-cpp-lox

another-cpp-lox is an experiment and learning exercise to implement an interpreter for the Lox language in C++.

## Credits

This project is based on the Lox interpreter and language specification from Robert Nystrom's book [Crafting Interpreters](https://craftinginterpreters.com).

In the book, Lox is implemented in Java and C; Lox has been implemented in C++ before, but my implementation is independent of other similar projects.

## Notes

This project is in-progress and does not yet fully meet the Lox specification! Use at your own risk.

## Building and usage

Clone this repository and run `make` to produce the `cpplox` executable using `g++`. The project's only dependency should be the C++ standard library.

Pass in a file:
```
./anothercpplox myfile.lox
```

Interactive interpreter (one line at a time, and enter an empty line to quit):
```
./anothercpplox
```
