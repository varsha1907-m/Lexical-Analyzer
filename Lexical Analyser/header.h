#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Maximum token length */
#define TOKEN_SIZE 256

/* Terminal colors */
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define CYAN    "\033[1;36m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define RED     "\033[1;31m"
#define WHITE   "\033[1;37m"
#define BLUE    "\033[1;34m"

/* Function status */
typedef enum
{
    SUCCESS,
    FAILURE
} Status;

/* Lexer information */
typedef struct
{
    FILE *input;
    FILE *report;

    char input_name[256];
    char report_name[256];

    int line;
    int token_count;
    int error_count;

} Lexer;

/* File handling */
Status validate_arguments(int argc, char *argv[], Lexer *lexer);
Status open_files(Lexer *lexer);
void close_files(Lexer *lexer);

/* Character handling */
int read_char(Lexer *lexer);
void return_char(int ch, Lexer *lexer);

/* Token handling */
void write_token(Lexer *lexer, const char *lexeme,
                 const char *category, int line);

/* Token scanning */
void scan_identifier(Lexer *lexer, int first, int line);
void scan_number(Lexer *lexer, int first, int line);
void scan_literal(Lexer *lexer, int first, int line);
void scan_operator(Lexer *lexer, int first, int line);
void scan_symbol(Lexer *lexer, int ch, int line);

/* Main lexical analyzer */
Status start_lexical_analysis(Lexer *lexer);

#endif