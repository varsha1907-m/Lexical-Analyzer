
#include "header.h"

static const char *keywords[] =
{
    "auto", "break", "case", "char", "const",
    "continue", "default", "do", "double", "else",
    "enum", "extern", "float", "for", "goto",
    "if", "inline", "int", "long", "register",
    "restrict", "return", "short", "signed",
    "sizeof", "static", "struct", "switch",
    "typedef", "union", "unsigned", "void",
    "volatile", "while", "_Bool", "_Complex",
    "_Imaginary", NULL
};

static int is_keyword(const char *word)
{
    for (int i = 0; keywords[i] != NULL; i++)
    {
        if (strcmp(word, keywords[i]) == 0)
        {
            return 1;
        }
    }

    return 0;
}

// Print token in terminal and report file
void write_token(Lexer *lexer, const char *lexeme,
                 const char *category, int line)
{
    printf("  %-6d | %-40s | %-27s\n",
           line, lexeme, category);

    fprintf(lexer->report, "%-8d | %-40s | %-28s\n",
            line, lexeme, category);

    lexer->token_count++;
}

// Scan identifiers and keywords
void scan_identifier(Lexer *lexer, int first, int line)
{
    char buffer[TOKEN_SIZE] = {0};
    int index = 0;
    int ch;

    buffer[index++] = (char)first;

    while ((ch = read_char(lexer)) != EOF)
    {
        if (!isalnum((unsigned char)ch) && ch != '_')
        {
            return_char(ch, lexer);
            break;
        }

        if (index < TOKEN_SIZE - 1)
        {
            buffer[index++] = (char)ch;
        }
    }

    buffer[index] = '\0';

    if (is_keyword(buffer))
    {
        write_token(lexer, buffer, "KEYWORD", line);
    }
    else
    {
        write_token(lexer, buffer, "IDENTIFIER", line);
    }
}

// Scan integer and floating-point constants
void scan_number(Lexer *lexer, int first, int line)
{
    char buffer[TOKEN_SIZE] = {0};
    int index = 0;
    int ch;
    int is_float = 0;

    buffer[index++] = (char)first;

    while ((ch = read_char(lexer)) != EOF)
    {
        if (isdigit((unsigned char)ch))
        {
            if (index < TOKEN_SIZE - 1)
            {
                buffer[index++] = (char)ch;
            }
        }
        else if (ch == '.' && !is_float)
        {
            is_float = 1;

            if (index < TOKEN_SIZE - 1)
            {
                buffer[index++] = '.';
            }
        }
        else
        {
            return_char(ch, lexer);
            break;
        }
    }

    buffer[index] = '\0';

    if (is_float)
    {
        write_token(lexer, buffer, "FLOAT_CONSTANT", line);
    }
    else
    {
        write_token(lexer, buffer, "INTEGER_CONSTANT", line);
    }
}

// Scan string and character literals
void scan_literal(Lexer *lexer, int first, int line)
{
    char buffer[TOKEN_SIZE] = {0};
    int index = 0;
    int ch;
    int closed = 0;
    int escaped = 0;
    int content_count = 0;

    buffer[index++] = (char)first;

    while ((ch = read_char(lexer)) != EOF)
    {
        if (ch == '\n')
        {
            break;
        }

        if (index < TOKEN_SIZE - 1)
        {
            buffer[index++] = (char)ch;
        }

        if (escaped)
        {
            escaped = 0;
            content_count++;
            continue;
        }

        if (ch == '\\')
        {
            escaped = 1;
            continue;
        }

        if (ch == first)
        {
            closed = 1;
            break;
        }

        content_count++;
    }

    buffer[index] = '\0';

    if (first == '"')
    {
        if (closed)
        {
            write_token(lexer, buffer, "STRING_LITERAL", line);
        }
        else
        {
            write_token(lexer, buffer, "INVALID_STRING", line);
            lexer->error_count++;
        }
    }
    else
    {
        if (closed && content_count == 1)
        {
            write_token(lexer, buffer, "CHARACTER_LITERAL", line);
        }
        else
        {
            write_token(lexer, buffer, "INVALID_CHARACTER", line);
            lexer->error_count++;
        }
    }
}

// Scan operators
void scan_operator(Lexer *lexer, int first, int line)
{
    char buffer[4] = {0};
    int ch1;
    int ch2;
    int length = 1;

    buffer[0] = (char)first;

    ch1 = read_char(lexer);

    if (ch1 == EOF)
    {
        write_token(lexer, buffer, "OPERATOR", line);
        return;
    }

    int valid_two = 0;

    switch (first)
    {
        case '+':
            valid_two = (ch1 == '+' || ch1 == '=');
            break;

        case '-':
            valid_two = (ch1 == '-' || ch1 == '=' || ch1 == '>');
            break;

        case '*':
        case '/':
        case '%':
        case '=':
        case '!':
            valid_two = (ch1 == '=');
            break;

        case '<':
        case '>':
            valid_two = (ch1 == '=' || ch1 == first);
            break;

        case '&':
            valid_two = (ch1 == '&' || ch1 == '=');
            break;

        case '|':
            valid_two = (ch1 == '|' || ch1 == '=');
            break;

        case '^':
            valid_two = (ch1 == '=');
            break;
    }

    if (valid_two)
    {
        buffer[1] = (char)ch1;
        length = 2;

        if ((first == '<' || first == '>') && ch1 == first)
        {
            ch2 = read_char(lexer);

            if (ch2 == '=')
            {
                buffer[2] = (char)ch2;
                length = 3;
            }
            else
            {
                return_char(ch2, lexer);
            }
        }
    }
    else
    {
        return_char(ch1, lexer);
    }

    buffer[length] = '\0';

    write_token(lexer, buffer,
                length == 1 ? "OPERATOR" : "COMPOUND_OPERATOR",
                line);
}

// Scan special symbols and delimiters
void scan_symbol(Lexer *lexer, int ch, int line)
{
    char buffer[2] = {(char)ch, '\0'};
    const char *category;

    switch (ch)
    {
        case '(':
        case '{':
        case '[':
            category = "OPEN_DELIMITER";
            break;

        case ')':
        case '}':
        case ']':
            category = "CLOSE_DELIMITER";
            break;

        case ';':
            category = "STATEMENT_TERMINATOR";
            break;

        case ',':
            category = "SEPARATOR";
            break;

        default:
            category = "SPECIAL_SYMBOL";
            break;
    }

    write_token(lexer, buffer, category, line);
}
