#include <stdio.h>
#include <string.h>

/* Structure to store book details */
struct Book
{
    int id;
    char name[50];
    float price;
    int quantity;
};

/* Function declarations */
void displayBooks(struct Book books[], int count);
void searchBook(struct Book books[], int count);
float calculateValue(struct Book books[], int count);

int main()
{
    struct Book books[3] =
    {
        {101, "C Programming", 450.50, 10},
        {102, "Embedded Systems", 650.00, 5},
        {103, "Data Structures", 550.75, 8}
    };

    int choice;

    printf("\n===== SMART LIBRARY MANAGEMENT =====\n");

    do
    {
        printf("\n1. Display Books");
        printf("\n2. Search Book");
        printf("\n3. Calculate Library Value");
        printf("\n4. Exit");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                displayBooks(books, 3);
                break;

            case 2:
                searchBook(books, 3);
                break;

            case 3:
                printf("Total value: %.2f\n",
                       calculateValue(books, 3));
                break;

            case 4:
                printf("Thank you!\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while (choice != 4);

    return 0;
}

/* Display all available books */
void displayBooks(struct Book books[], int count)
{
    printf("\n--- BOOK DETAILS ---\n");

    for (int i = 0; i < count; i++)
    {
        printf("ID: %d | Name: %s | Price: %.2f\n",
               books[i].id,
               books[i].name,
               books[i].price);
    }
}

/* Search for a book using its ID */
void searchBook(struct Book books[], int count)
{
    int searchId;
    int found = 0;

    printf("Enter book ID: ");
    scanf("%d", &searchId);

    for (int i = 0; i < count; i++)
    {
        if (books[i].id == searchId)
        {
            printf("Book found: %s\n", books[i].name);
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("Book not found.\n");
    }
}

/* Calculate total value of books */
float calculateValue(struct Book books[], int count)
{
    float total = 0.0;

    for (int i = 0; i < count; i++)
    {
        total += books[i].price * books[i].quantity;
    }

    return total;
}