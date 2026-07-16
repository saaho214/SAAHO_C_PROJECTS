#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// --- Data Structure Definitions ---
#define MAX_BOOKS 100
#define MAX_MEMBERS 50

// Structure to hold book information
typedef struct {
    int id;
    char title[100];
    char author[100];
    int available; // 1 for available, 0 for borrowed
    int borrowerId; // ID of the member who borrowed it
    int borrowCount; // New field to track total borrows
} Book;

// Structure to hold member information
typedef struct {
    int id;
    char name[100];
    int borrowedCount;
    int borrowedBookIds[10]; // Stores IDs of borrowed books
} Member;

// --- Global Arrays and Counters ---
Book books[MAX_BOOKS];
Member members[MAX_MEMBERS];
int bookCount = 0;
int memberCount = 0;

// --- Function Prototypes ---
void loadData();
void saveData();
void login();
void adminMenu();
void memberMenu(int memberId);

// Admin functions
void addBook();
void addMember();
void displayAllBooks();
void displayAllMembers();
void deleteMember(); // New function to delete a member
void borrowBookAdmin();
void returnBookAdmin();
void displayAnalytics();

// Member functions
void searchBook();
void displayBorrowedBooks(int memberId);
void borrowBookMember(int memberId);
void returnBookMember(int memberId);

// Utility functions
int findBookById(int bookId);
int findMemberById(int memberId);
void clearInputBuffer();

// --- Main Function ---
void createInitialData();

int main() {
    int choice;
    // Load existing data from files at program start
    loadData();

    // If no data is loaded (first run), create initial dummy data
    if (bookCount == 0 && memberCount == 0) {
        createInitialData();
        saveData(); // Save the initial data immediately
    }

    do {
        printf("\n--- Library Management System ---\n");
        printf("1. Login\n");
        printf("2. Register as a new member\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1:
                login();
                break;
            case 2:
                addMember();
                break;
            case 3:
                // Save data before exiting
                saveData();
                printf("Exiting program. Goodbye!\n");
                return 0; // Exit the program
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);

    return 0;
}

// --- Data Persistence Functions (File I/O) ---
// Loads book and member data from binary files
void loadData() {
    FILE *bookFile = fopen("books.dat", "rb");
    if (bookFile != NULL) {
        fread(&bookCount, sizeof(int), 1, bookFile);
        fread(books, sizeof(Book), bookCount, bookFile);
        fclose(bookFile);
    }

    FILE *memberFile = fopen("members.dat", "rb");
    if (memberFile != NULL) {
        fread(&memberCount, sizeof(int), 1, memberFile);
        fread(members, sizeof(Member), memberCount, memberFile);
        fclose(memberFile);
    }
}

// Saves book and member data to binary files
void saveData() {
    FILE *bookFile = fopen("books.dat", "wb");
    if (bookFile != NULL) {
        fwrite(&bookCount, sizeof(int), 1, bookFile);
        fwrite(books, sizeof(Book), bookCount, bookFile);
        fclose(bookFile);
    }

    FILE *memberFile = fopen("members.dat", "wb");
    if (memberFile != NULL) {
        fwrite(&memberCount, sizeof(int), 1, memberFile);
        fwrite(members, sizeof(Member), memberCount, memberFile);
        fclose(memberFile);
    }
}

// --- Authentication Functions ---
void login() {
    int userType;
    printf("\n--- Login ---\n");
    printf("1. Admin\n");
    printf("2. Member\n");
    printf("Enter your choice: ");
    scanf("%d", &userType);
    clearInputBuffer();

    if (userType == 1) {
        char password[50];
        printf("Enter Admin Password: ");
        scanf("%s", password);
        clearInputBuffer();
        if (strcmp(password, "admin123") == 0) {
            printf("\nAdmin login successful!\n");
            adminMenu();
        } else {
            printf("\nInvalid password. Returning to main menu.\n");
        }
    } else if (userType == 2) {
        int memberId;
        printf("Enter Member ID: ");
        scanf("%d", &memberId);
        clearInputBuffer();
        if (findMemberById(memberId) != -1) {
            printf("\nMember login successful!\n");
            memberMenu(memberId);
        } else {
            printf("\nInvalid Member ID. Returning to main menu.\n");
        }
    } else {
        printf("\nInvalid choice. Returning to main menu.\n");
    }
}

// --- Admin Menu and Functions ---
void adminMenu() {
    int choice;
    do {
        printf("\n--- Admin Menu ---\n");
        printf("Total Members: %d\n", memberCount); // Display total members
        printf("1. Add a new book\n");
        printf("2. Display all books\n");
        printf("3. Display all members\n");
        printf("4. Delete a member\n"); // New menu option
        printf("5. Borrow a book\n");
        printf("6. Return a book\n");
        printf("7. Display analytics\n");
        printf("8. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1:
                addBook();
                break;
            case 2:
                displayAllBooks();
                break;
            case 3:
                displayAllMembers();
                break;
            case 4:
                deleteMember();
                break;
            case 5:
                borrowBookAdmin();
                break;
            case 6:
                returnBookAdmin();
                break;
            case 7:
                displayAnalytics();
                break;
            case 8:
                printf("Logging out...\n");
                return; // Go back to the main menu
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 8);
}

// Adds a new book to the library
void addBook() {
    if (bookCount >= MAX_BOOKS) {
        printf("Error: Library is full. Cannot add more books.\n");
        return;
    }
    printf("\n--- Add New Book ---\n");
    books[bookCount].id = bookCount + 1;
    printf("Enter title: ");
    scanf(" %[^\n]s", books[bookCount].title);
    clearInputBuffer();
    printf("Enter author: ");
    scanf(" %[^\n]s", books[bookCount].author);
    clearInputBuffer();
    books[bookCount].available = 1;
    books[bookCount].borrowerId = -1; // -1 indicates no borrower
    books[bookCount].borrowCount = 0; // Initialize new field
    bookCount++;
    printf("Book added successfully!\n");
}

// Adds a new member to the library
void addMember() {
    if (memberCount >= MAX_MEMBERS) {
        printf("Error: Max members reached. Cannot add more.\n");
        return;
    }
    printf("\n--- Add New Member ---\n");
    members[memberCount].id = memberCount + 1;
    printf("Enter member's name: ");
    scanf(" %[^\n]s", members[memberCount].name);
    clearInputBuffer();
    members[memberCount].borrowedCount = 0;
    memberCount++;
    printf("Member '%s' added successfully with ID %d.\n", members[memberCount - 1].name, members[memberCount - 1].id);
}

// Displays a list of all books in the library
void displayAllBooks() {
    printf("\n--- All Books ---\n");
    if (bookCount == 0) {
        printf("No books in the library.\n");
        return;
    }
    printf("%-5s %-30s %-25s %-10s %-15s\n", "ID", "Title", "Author", "Status", "Times Borrowed");
    printf("--------------------------------------------------------------------------------\n");
    for (int i = 0; i < bookCount; i++) {
        printf("%-5d %-30s %-25s %-10s %-15d\n",
               books[i].id,
               books[i].title,
               books[i].author,
               books[i].available ? "Available" : "Borrowed",
               books[i].borrowCount);
    }
}

// Displays a list of all registered members
void displayAllMembers() {
    printf("\n--- All Members ---\n");
    printf("Total Members: %d\n", memberCount);
    if (memberCount == 0) {
        printf("No members registered.\n");
        return;
    }
    printf("%-5s %-25s %-15s\n", "ID", "Name", "Borrowed Count");
    printf("---------------------------------------------\n");
    for (int i = 0; i < memberCount; i++) {
        printf("%-5d %-25s %-15d\n",
               members[i].id,
               members[i].name,
               members[i].borrowedCount);
    }
}

// Deletes a member by ID
void deleteMember() {
    int memberId;
    printf("\n--- Delete a Member ---\n");
    printf("Enter Member ID to delete: ");
    scanf("%d", &memberId);
    clearInputBuffer();

    int memberIndex = findMemberById(memberId);

    if (memberIndex == -1) {
        printf("Error: Member not found.\n");
        return;
    }

    // Check if the member has any books borrowed
    if (members[memberIndex].borrowedCount > 0) {
        printf("Error: Cannot delete member '%s'. Please ensure they return all borrowed books first.\n", members[memberIndex].name);
        return;
    }

    // Shift elements to fill the gap left by the deleted member
    for (int i = memberIndex; i < memberCount - 1; i++) {
        members[i] = members[i + 1];
    }
    memberCount--;
    printf("Member with ID %d successfully deleted.\n", memberId);
}


// Handles borrowing a book from the admin side
void borrowBookAdmin() {
    int bookId, memberId;
    printf("\n--- Borrow a Book ---\n");
    printf("Enter Book ID: ");
    scanf("%d", &bookId);
    clearInputBuffer();
    printf("Enter Member ID: ");
    scanf("%d", &memberId);
    clearInputBuffer();

    int bookIndex = findBookById(bookId);
    int memberIndex = findMemberById(memberId);

    if (bookIndex == -1) {
        printf("Error: Book not found.\n");
    } else if (memberIndex == -1) {
        printf("Error: Member not found.\n");
    } else if (!books[bookIndex].available) {
        printf("Error: Book is already borrowed.\n");
    } else {
        // Update book and member records
        books[bookIndex].available = 0;
        books[bookIndex].borrowerId = memberId;
        books[bookIndex].borrowCount++; // Increment borrow count
        members[memberIndex].borrowedBookIds[members[memberIndex].borrowedCount] = bookId;
        members[memberIndex].borrowedCount++;
        printf("Book '%s' successfully borrowed by '%s'.\n",
               books[bookIndex].title, members[memberIndex].name);
    }
}

// Handles returning a book from the admin side
void returnBookAdmin() {
    int bookId;
    printf("\n--- Return a Book ---\n");
    printf("Enter Book ID: ");
    scanf("%d", &bookId);
    clearInputBuffer();

    int bookIndex = findBookById(bookId);

    if (bookIndex == -1) {
        printf("Error: Book not found.\n");
    } else if (books[bookIndex].available) {
        printf("Error: Book is already available.\n");
    } else {
        int memberIndex = findMemberById(books[bookIndex].borrowerId);
        // Reset book status
        books[bookIndex].available = 1;
        books[bookIndex].borrowerId = -1;
        // Remove book from member's record
        if (memberIndex != -1) {
            for (int i = 0; i < members[memberIndex].borrowedCount; i++) {
                if (members[memberIndex].borrowedBookIds[i] == bookId) {
                    // Shift elements to fill the gap
                    for (int j = i; j < members[memberIndex].borrowedCount - 1; j++) {
                        members[memberIndex].borrowedBookIds[j] = members[memberIndex].borrowedBookIds[j + 1];
                    }
                    members[memberIndex].borrowedCount--;
                    break;
                }
            }
        }
        printf("Book '%s' successfully returned.\n", books[bookIndex].title);
    }
}

// Displays key library statistics
void displayAnalytics() {
    int borrowedCount = 0;
    int mostBorrowedBookIndex = -1;
    int maxBorrows = -1;
    int mostActiveMemberIndex = -1;
    int maxBorrowsMember = -1;

    // Calculate total borrowed books and find most borrowed book
    for (int i = 0; i < bookCount; i++) {
        if (!books[i].available) {
            borrowedCount++;
        }
        if (books[i].borrowCount > maxBorrows) {
            maxBorrows = books[i].borrowCount;
            mostBorrowedBookIndex = i;
        }
    }

    // Find the most active member
    for (int i = 0; i < memberCount; i++) {
        if (members[i].borrowedCount > maxBorrowsMember) {
            maxBorrowsMember = members[i].borrowedCount;
            mostActiveMemberIndex = i;
        }
    }

    printf("\n--- Library Analytics ---\n");
    printf("Total Books: %d\n", bookCount);
    printf("Books Available: %d\n", bookCount - borrowedCount);
    printf("Books Borrowed: %d\n", borrowedCount);
    printf("Total Members: %d\n", memberCount);
    
    // Display most borrowed book
    if (mostBorrowedBookIndex != -1) {
        printf("Most Borrowed Book: '%s' by %s (Borrowed %d times)\n",
               books[mostBorrowedBookIndex].title,
               books[mostBorrowedBookIndex].author,
               books[mostBorrowedBookIndex].borrowCount);
    } else {
        printf("Most Borrowed Book: No books have been borrowed yet.\n");
    }

    // Display most active member
    if (mostActiveMemberIndex != -1) {
        printf("Most Active Member: '%s' (Borrowed %d books)\n",
               members[mostActiveMemberIndex].name,
               members[mostActiveMemberIndex].borrowedCount);
    } else {
        printf("Most Active Member: No members have borrowed books yet.\n");
    }
}

// --- Member Menu and Functions ---
void memberMenu(int memberId) {
    int choice;
    do {
        printf("\n--- Member Menu (ID: %d) ---\n", memberId);
        printf("1. Search for a book\n");
        printf("2. Display my borrowed books\n");
        printf("3. Borrow a book\n");
        printf("4. Return a book\n");
        printf("5. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1:
                searchBook();
                break;
            case 2:
                displayBorrowedBooks(memberId);
                break;
            case 3:
                borrowBookMember(memberId);
                break;
            case 4:
                returnBookMember(memberId);
                break;
            case 5:
                printf("Logging out...\n");
                return; // Go back to main menu
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);
}

// Searches for a book by title
void searchBook() {
    char searchTerm[100];
    printf("\n--- Search Book ---\n");
    printf("Enter book title or author to search: ");
    scanf(" %[^\n]s", searchTerm);
    clearInputBuffer();

    int found = 0;
    printf("\nSearch Results:\n");
    printf("%-5s %-30s %-25s %-10s\n", "ID", "Title", "Author", "Status");
    printf("------------------------------------------------------------------\n");
    for (int i = 0; i < bookCount; i++) {
        char lowerTitle[100], lowerAuthor[100], lowerSearchTerm[100];
        strcpy(lowerTitle, books[i].title);
        strcpy(lowerAuthor, books[i].author);
        strcpy(lowerSearchTerm, searchTerm);
        for(int k=0; lowerTitle[k]; k++){ lowerTitle[k]=tolower(lowerTitle[k]); }
        for(int k=0; lowerAuthor[k]; k++){ lowerAuthor[k]=tolower(lowerAuthor[k]); }
        for(int k=0; lowerSearchTerm[k]; k++){ lowerSearchTerm[k]=tolower(lowerSearchTerm[k]); }

        if (strstr(lowerTitle, lowerSearchTerm) || strstr(lowerAuthor, lowerSearchTerm)) {
            printf("%-5d %-30s %-25s %-10s\n",
                   books[i].id,
                   books[i].title,
                   books[i].author,
                   books[i].available ? "Available" : "Borrowed");
            found = 1;
        }
    }
    if (!found) {
        printf("No matching books found.\n");
    }
}

// Displays all books currently borrowed by a member
void displayBorrowedBooks(int memberId) {
    int memberIndex = findMemberById(memberId);
    if (memberIndex == -1 || members[memberIndex].borrowedCount == 0) {
        printf("\nYou have not borrowed any books.\n");
        return;
    }

    printf("\n--- Your Borrowed Books ---\n");
    printf("%-5s %-30s %-25s\n", "ID", "Title", "Author");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < members[memberIndex].borrowedCount; i++) {
        int bookId = members[memberIndex].borrowedBookIds[i];
        int bookIndex = findBookById(bookId);
        if (bookIndex != -1) {
            printf("%-5d %-30s %-25s\n",
                   books[bookIndex].id,
                   books[bookIndex].title,
                   books[bookIndex].author);
        }
    }
}

// Allows a member to borrow a book
void borrowBookMember(int memberId) {
    int bookId;
    printf("\n--- Borrow a Book ---\n");
    printf("Enter Book ID to borrow: ");
    scanf("%d", &bookId);
    clearInputBuffer();

    int bookIndex = findBookById(bookId);

    if (bookIndex == -1) {
        printf("Error: Book not found.\n");
    } else if (!books[bookIndex].available) {
        printf("Error: Book is already borrowed.\n");
    } else {
        int memberIndex = findMemberById(memberId);
        if (memberIndex != -1 && members[memberIndex].borrowedCount < 10) {
            // Update book and member records
            books[bookIndex].available = 0;
            books[bookIndex].borrowerId = memberId;
            books[bookIndex].borrowCount++; // Increment borrow count
            members[memberIndex].borrowedBookIds[members[memberIndex].borrowedCount] = bookId;
            members[memberIndex].borrowedCount++;
            printf("Book '%s' successfully borrowed.\n", books[bookIndex].title);
        } else {
            printf("Error: You have reached the maximum number of books you can borrow.\n");
        }
    }
}

// Allows a member to return a book
void returnBookMember(int memberId) {
    int bookId;
    printf("\n--- Return a Book ---\n");
    printf("Enter Book ID to return: ");
    scanf("%d", &bookId);
    clearInputBuffer();

    int bookIndex = findBookById(bookId);

    if (bookIndex == -1) {
        printf("Error: Book not found.\n");
    } else if (books[bookIndex].borrowerId != memberId) {
        printf("Error: You did not borrow this book.\n");
    } else {
        int memberIndex = findMemberById(memberId);
        // Reset book status
        books[bookIndex].available = 1;
        books[bookIndex].borrowerId = -1;
        // Remove book from member's record
        if (memberIndex != -1) {
            for (int i = 0; i < members[memberIndex].borrowedCount; i++) {
                if (members[memberIndex].borrowedBookIds[i] == bookId) {
                    for (int j = i; j < members[memberIndex].borrowedCount - 1; j++) {
                        members[memberIndex].borrowedBookIds[j] = members[memberIndex].borrowedBookIds[j + 1];
                    }
                    members[memberIndex].borrowedCount--;
                    break;
                }
            }
        }
        printf("Book '%s' successfully returned.\n", books[bookIndex].title);
    }
}

// --- Utility Functions ---
// Finds a book by its ID and returns its index, or -1 if not found
int findBookById(int bookId) {
    for (int i = 0; i < bookCount; i++) {
        if (books[i].id == bookId) {
            return i;
        }
    }
    return -1;
}

// Finds a member by their ID and returns their index, or -1 if not found
int findMemberById(int memberId) {
    for (int i = 0; i < memberCount; i++) {
        if (members[i].id == memberId) {
            return i;
        }
    }
    return -1;
}

// Clears the input buffer to prevent issues with scanf
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Initial dummy data for demonstration (only runs on first launch)
void createInitialData() {
    // Add dummy books
    books[bookCount++] = (Book){1, "The Hitchhiker's Guide to the Galaxy", "Douglas Adams", 1, -1, 0};
    books[bookCount++] = (Book){2, "Dune", "Frank Herbert", 0, 1, 1};
    books[bookCount++] = (Book){3, "1984", "George Orwell", 1, -1, 0};
    
    // Add dummy members
    members[memberCount++] = (Member){1, "Jane Doe", 1, {2}};
    members[memberCount++] = (Member){2, "John Smith", 0, {0}};
}
