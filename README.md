# another-cpp-lox

another-cpp-lox is an experiment and learning exercise to implement an interpreter for the Lox language in C++.

## Credits

This project is based on the Lox interpreter and language specification from Robert Nystrom's book [Crafting Interpreters](https://craftinginterpreters.com).

In the book, Lox is implemented in Java and C; Lox has been implemented in C++ before, but my implementation is independent of other similar projects. To remove ambiguity, I added 'another' to the name.

## Notes

> [!WARNING]  
> This project is in-progress and does not yet fully meet the Lox specification! Use at your own risk.

Because this is being developed during my first read-through of the book, there may be some code quality issues, but I will work to address them.

## Building and usage

Clone this repository and run `make` to produce the `anothercpplox` executable using `g++`. The project's only dependency should be the C++ standard library.

Pass in a file:
```
./anothercpplox myfile.lox
```

Interactive interpreter (one line at a time, and enter an empty line to quit):
```
./anothercpplox
```

## Structure (in progress)

Here's how the interpreter works:
```
Lox source code (contents of a file or lines from the interactive interpreter)
            |
            V
Scanner (converts the code into a sequence of tokens) -> tokens
                                                          |
                                                          V
Parser (uses the tokens to produce an abstract syntax tree) -> AST
                                                                |
                                                                V
Interpreter (interprets the AST)          (in progress)        ...
                                                                |
                                                                V
                                                               ...
```
Other features:
```
AstPrinter (prints a given abstract syntax tree, implementing the visitor interface)
```
For more details, see Nystrom's book linked in credits above).

## Lox Language

For a description of the actual language being implemented, check out Nystrom's [here](https://craftinginterpreters.com/the-lox-language.html).
