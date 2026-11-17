#include "header.h"

static void display_banner(void)
{
    printf(CYAN BOLD);
    printf("\n");
    printf("============================================================\n");
    printf("                                                            \n");
    printf("               SMART C LEXICAL ANALYZER                    \n");
    printf("                                                            \n");
    printf("============================================================\n");
    printf(RESET);

    printf(WHITE "  A simple and efficient C source code token analyzer\n" RESET);
    printf("  ----------------------------------------------------------\n\n");
}

static void display_summary(Lexer *lexer, Status result)
{
    printf("\n");
    printf(CYAN "============================================================\n");
    printf("                     ANALYSIS SUMMARY                       \n");
    printf("============================================================\n" RESET);

    printf("  Input file       : %s\n", lexer->input_name);
    printf("  Report file      : %s\n", lexer->report_name);

    printf("  Total tokens     : %d\n", lexer->token_count);
    printf("  Total errors     : %d\n", lexer->error_count);

    printf("------------------------------------------------------------\n");

    if (result == SUCCESS)
    {
        printf(GREEN BOLD "  [SUCCESS] Lexical analysis completed successfully!\n" RESET);
    }
    else
    {
        printf(RED BOLD "  [WARNING] Lexical errors were detected.\n" RESET);
    }

    printf(CYAN "============================================================\n\n" RESET);
}

int main(int argc, char *argv[])
{
    Lexer lexer = {0};

    display_banner();

    if (validate_arguments(argc, argv, &lexer) == FAILURE)
    {
        return 1;
    }

    if (open_files(&lexer) == FAILURE)
    {
        return 1;
    }

    printf(YELLOW "  Analyzing source code...\n\n" RESET);

    Status result = start_lexical_analysis(&lexer);

    display_summary(&lexer, result);

    close_files(&lexer);

    return (result == SUCCESS) ? 0 : 1;
}