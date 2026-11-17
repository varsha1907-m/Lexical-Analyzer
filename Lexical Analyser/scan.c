
#include "header.h"

// Read one character
int read_char(Lexer *lexer)
{
    int ch = fgetc(lexer->input);

    if (ch == '\n')
    {
        lexer->line++;
    }

    return ch;
}

// Return one character
void return_char(int ch, Lexer *lexer)
{
    if (ch == EOF)
    {
        return;
    }

    if (ungetc(ch, lexer->input) != EOF && ch == '\n')
    {
        lexer->line--;
    }
}

// Scan comments
static void scan_comment(Lexer *lexer, int line)
{
    char buffer[TOKEN_SIZE] = {0};
    int index = 0;
    int ch;
    int previous = 0;
    int closed = 0;

    ch = read_char(lexer);

    if (ch == '/')
    {
        buffer[index++] = '/';
        buffer[index++] = '/';

        while ((ch = read_char(lexer)) != EOF && ch != '\n')
        {
            if (ch == '\r')
                continue;

            if (index < TOKEN_SIZE - 1)
                buffer[index++] = (char)ch;
        }

        buffer[index] = '\0';

        write_token(lexer, buffer, "SINGLE_LINE_COMMENT", line);
    }
    else if (ch == '*')
    {
        buffer[index++] = '/';
        buffer[index++] = '*';

        while ((ch = read_char(lexer)) != EOF)
        {
            if (ch == '\r')
                continue;

            if (index < TOKEN_SIZE - 1)
                buffer[index++] = (char)ch;

            if (previous == '*' && ch == '/')
            {
                closed = 1;
                break;
            }

            previous = ch;
        }

        buffer[index] = '\0';

        if (closed)
        {
            write_token(lexer, buffer, "MULTI_LINE_COMMENT", line);
        }
        else
        {
            write_token(lexer, buffer, "INVALID_COMMENT", line);
            lexer->error_count++;
        }
    }
    else
    {
        return_char(ch, lexer);
        scan_operator(lexer, '/', line);
    }
}

// Scan preprocessor directives
static void scan_directive(Lexer *lexer, int line)
{
    char buffer[TOKEN_SIZE] = {0};
    int index = 0;
    int ch;

    buffer[index++] = '#';

    while ((ch = read_char(lexer)) != EOF &&
           ch != '\n' && ch != '\r')
    {
        if (index < TOKEN_SIZE - 1)
        {
            buffer[index++] = (char)ch;
        }
    }

    buffer[index] = '\0';

    write_token(lexer, buffer, "PREPROCESSOR_DIRECTIVE", line);
}

// Display table header
static void display_table_header(Lexer *lexer)
{
    printf(CYAN BOLD);
    printf("\n");
    printf("  -----------------------------------------------------------------------------\n");
    printf("  %-6s | %-40s | %-27s\n",
           "LINE", "LEXEME", "TOKEN CATEGORY");
    printf("  -----------------------------------------------------------------------------\n");
    printf(RESET);

    fprintf(lexer->report,
            "SMART C LEXICAL ANALYZER - TOKEN REPORT\n");

    fprintf(lexer->report,
            "================================================================================\n");

    fprintf(lexer->report,
            "%-8s | %-40s | %-28s\n",
            "LINE", "LEXEME", "TOKEN CATEGORY");

    fprintf(lexer->report,
            "--------------------------------------------------------------------------------\n");
}

// Display report footer
static void display_table_footer(Lexer *lexer)
{
    fprintf(lexer->report,
            "\n================================================================================\n");

    fprintf(lexer->report,
            "Total tokens : %d\n", lexer->token_count);

    fprintf(lexer->report,
            "Total errors : %d\n", lexer->error_count);

    fprintf(lexer->report,
            "================================================================================\n");
}

// Start lexical analysis
Status start_lexical_analysis(Lexer *lexer)
{
    int ch;

    display_table_header(lexer);

    while ((ch = read_char(lexer)) != EOF)
    {
        int start_line = lexer->line;

        if (isspace((unsigned char)ch))
        {
            continue;
        }

        if (ch == '#')
        {
            scan_directive(lexer, start_line);
        }
        else if (ch == '/')
        {
            scan_comment(lexer, start_line);
        }
        else if (isalpha((unsigned char)ch) || ch == '_')
        {
            scan_identifier(lexer, ch, start_line);
        }
        else if (isdigit((unsigned char)ch))
        {
            scan_number(lexer, ch, start_line);
        }
        else if (ch == '"' || ch == '\'')
        {
            scan_literal(lexer, ch, start_line);
        }
        else if (strchr("+-*=<>!%&|^~", ch) != NULL)
        {
            scan_operator(lexer, ch, start_line);
        }
        else if (strchr("(){}[];,.:?", ch) != NULL)
        {
            scan_symbol(lexer, ch, start_line);
        }
        else
        {
            char unknown[2] = {(char)ch, '\0'};

            write_token(lexer, unknown, "UNKNOWN", start_line);
            lexer->error_count++;
        }
    }

    display_table_footer(lexer);

    return lexer->error_count == 0 ? SUCCESS : FAILURE;
}
