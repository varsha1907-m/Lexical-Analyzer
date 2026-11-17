# Lexical Analyzer in C

## Project Description
A Lexical Analyzer is the first phase of a compiler. It reads source code and converts it into a sequence of tokens such as keywords, identifiers, operators, constants, and special symbols.

This project is implemented using C programming language.

## Features
- Identifies keywords and identifiers.
- Recognizes operators and special symbols.
- Detects constants and string literals.
- Performs lexical analysis on C source files.
- Generates token reports.
- Reports lexical errors.

## Technologies Used
- C Programming
- GCC Compiler
- Linux
- Makefile

## Project Structure
```text
Lexical Analyzer/
├── input/
│   ├── error.c
│   ├── library.c
│   ├── literal.c
│   └── test.c
├── main.c
├── scan.c
├── token.c
├── validate.c
├── header.h
├── Analysis_Report.txt
├── error_report.txt
└── test_Report.txt
```

## Compilation and Execution

Compile the project:

```bash
gcc main.c scan.c token.c validate.c -o lexer
```

Run the program:

```bash
./lexer
```

## Sample Input
```c
int main()
{
    int a = 10;
    int b = 20;
    return a + b;
}
```

## Learning Outcomes
- Understanding the phases of a compiler.
- Learning token identification and classification.
- Improving C programming and file-handling skills.
- Understanding lexical errors and validation.

## Author
**Varsha M**

GitHub: [varsha1907-m](https://github.com/varsha1907-m)
