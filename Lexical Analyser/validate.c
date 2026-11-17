#include "header.h"

Status validate_arguments(int argc, char *argv[], Lexer *lexer)
{
    if (argc < 2 || argc > 3)
    {
        printf(RED "Usage: ./lexer input/library.c [report.txt]\n" RESET);
        return FAILURE;
    }

    size_t length = strlen(argv[1]);

    if (length < 3 || strcmp(argv[1] + length - 2, ".c") != 0)
    {
        printf(RED "Error: Input file must have .c extension.\n" RESET);
        return FAILURE;
    }

    if (strlen(argv[1]) >= sizeof(lexer->input_name))
    {
        printf(RED "Error: Input filename is too long.\n" RESET);
        return FAILURE;
    }

    strcpy(lexer->input_name, argv[1]);

    if (argc == 3)
    {
        if (strlen(argv[2]) >= sizeof(lexer->report_name))
        {
            printf(RED "Error: Report filename is too long.\n" RESET);
            return FAILURE;
        }

        strcpy(lexer->report_name, argv[2]);
    }
    else
    {
        strcpy(lexer->report_name, "Analysis_Report.txt");
    }

    return SUCCESS;
}

Status open_files(Lexer *lexer)
{
    lexer->input = fopen(lexer->input_name, "r");

    if (lexer->input == NULL)
    {
        perror("Unable to open input file");
        return FAILURE;
    }

    lexer->report = fopen(lexer->report_name, "w");

    if (lexer->report == NULL)
    {
        perror("Unable to create report file");
        fclose(lexer->input);
        lexer->input = NULL;
        return FAILURE;
    }

    lexer->line = 1;
    lexer->token_count = 0;
    lexer->error_count = 0;

    return SUCCESS;
}

void close_files(Lexer *lexer)
{
    if (lexer->input != NULL)
    {
        fclose(lexer->input);
        lexer->input = NULL;
    }

    if (lexer->report != NULL)
    {
        fclose(lexer->report);
        lexer->report = NULL;
    }
}