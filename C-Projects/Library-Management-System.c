#include <stdio.h>      // For standard input/output functions
#include <stdlib.h>     // For malloc and free functions
#include <string.h>     // For string handling functions like strcpy, strcmp
#include <ctype.h>      // For character handling like tolower(), isdigit()

#define MAX_BOOKS 100   // Maximum number of books that can be stored
#define MAX_TITLE 100   // Max length for a book title
#define MAX_AUTHOR 100  // Max length for an author's name
#define MAX_ISBN 14     // 13 digits + null terminator for ISBN

// Arrays of pointers to dynamically store each book's details
char* titles[MAX_BOOKS];
char* authors[MAX_BOOKS];
char* isbns[MAX_BOOKS];
int bookCount = 0;  // Keeps track of how many books are currently stored

// ISBN VALIDATION & DUPLICATE CHECKS //

// Function to validate ISBN-13 format and checksum
int isValidISBN13(char isbn[]) {
    if (strlen(isbn) != 13)
        return 0;
  
    for (int i = 0; i < 13; i++) {
        if (!isdigit(isbn[i]))
            return 0;
    }

    int sum = 0;
    for (int i = 0; i < 12; i++) {
        int digit = isbn[i] - '0';
        sum += (i % 2 == 0) ? digit : digit * 3;
    }

    int checksum = (10 - (sum % 10)) % 10;
    return (isbn[12] - '0') == checksum;
}

// Function to check for duplicate ISBN
int isDuplicateISBN(char isbn[]) {
    for (int i = 0; i < bookCount; i++) {
        if (strcmp(isbns[i], isbn) == 0)
            return 1;
    }
    return 0;
}

// BOOK ADDITION FUNCTION //

void addBook() {
    if (bookCount >= MAX_BOOKS) {
        printf("Library is full!\n");
        return;
    }

    // Temporary input buffers
    char tempTitle[MAX_TITLE];
    char tempAuthor[MAX_AUTHOR];
    char tempISBN[MAX_ISBN];

    // Get user input
    printf("Enter Book Title: ");
    scanf(" %[^\n]", tempTitle);

    printf("Enter Author Name: ");
    scanf(" %[^\n]", tempAuthor);

    printf("Enter ISBN (13 digits): ");
    scanf(" %[^\n]", tempISBN);

    // Validate input
    if (!isValidISBN13(tempISBN)) {
        printf("Invalid ISBN! Must be 13 digits with correct checksum.\n");
        return;
    }
    if (isDuplicateISBN(tempISBN)) {
        printf("A book with this ISBN already exists!\n");
        return;
    }

    // Allocate memory and copy data
    titles[bookCount] = (char*) malloc(strlen(tempTitle) + 1);
    authors[bookCount] = (char*) malloc(strlen(tempAuthor) + 1);
    isbns[bookCount] = (char*) malloc(strlen(tempISBN) + 1);

    strcpy(titles[bookCount], tempTitle);
    strcpy(authors[bookCount], tempAuthor);
    strcpy(isbns[bookCount], tempISBN);

    bookCount++;
    printf("Book added successfully!\n");
}

// BOOK DELETION FUNCTION  //

void deleteBookByISBN() {
    char isbnToDelete[MAX_ISBN];
    printf("Enter ISBN to delete: ");
    scanf(" %[^\n]", isbnToDelete);

    int found = 0;

    for (int i = 0; i < bookCount; i++) {
        if (strcmp(isbns[i], isbnToDelete) == 0) {
            found = 1;

            // Free memory of the book to delete
            free(titles[i]);
            free(authors[i]);
            free(isbns[i]);

            // Shift remaining books leftward
            for (int j = i; j < bookCount - 1; j++) {
                titles[j] = titles[j + 1];
                authors[j] = authors[j + 1];
                isbns[j] = isbns[j + 1];
            }

            bookCount--;
            printf("Book deleted successfully!\n");
            break;
        }
    }
    if (!found) {
        printf("Book with ISBN %s not found.\n", isbnToDelete);
    }
}

// CASE-INSENSITIVE TITLE COMPARISON  //

// Helper function to compare two strings case-insensitively
int caseInsensitiveCompare(const char* a, const char* b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
            return 0;
        a++;
        b++;
    }
    return *a == *b;
}

// Search function with case-insensitive title comparison
void searchByTitle() {
    char titleToSearch[MAX_TITLE];
    printf("Enter title to search: ");
    scanf(" %[^\n]", titleToSearch);

    int found = 0;

    for (int i = 0; i < bookCount; i++) {
        if (caseInsensitiveCompare(titles[i], titleToSearch)) {
            printf("\nBook Found:\n");
            printf("Title: %s\nAuthor: %s\nISBN: %s\n", titles[i], authors[i], isbns[i]);
            found = 1;
        }
    }
    if (!found) {
        printf("No book found with the title \"%s\".\n", titleToSearch);
    }
}

// DISPLAY ALL BOOKS FUNCTION //

void displayBooks() {
    if (bookCount == 0) {
        printf("No books in the library.\n");
        return;
    }
    printf("\n--- Book List ---\n");
    for (int i = 0; i < bookCount; i++) {
        printf("Title: %s | Author: %s | ISBN: %s\n", titles[i], authors[i], isbns[i]);
    }
}

// MEMORY CLEAN-UP BLOCK //

void freeMemory() {
    for (int i = 0; i < bookCount; i++) {
        free(titles[i]);
        free(authors[i]);
        free(isbns[i]);
    }
}

// MAIN FUNCTION  //

int main() {
    int choice;
    do {
        // Display the menu
        printf("\n--- Library Management System ---\n");
        printf("1. Add Book\n");
        printf("2. Delete Book by ISBN\n");
        printf("3. Search Book by Title\n");
        printf("4. Display All Books\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Clears leftover newline

        switch (choice) {
            case 1: addBook(); break;
            case 2: deleteBookByISBN(); break;
            case 3: searchByTitle(); break;
            case 4: displayBooks(); break;
            case 5:
                printf("Exiting program. Freeing memory...\n");
                freeMemory();
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 5);
    return 0;
}
