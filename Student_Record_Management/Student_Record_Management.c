#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define MAX_HISTORY 50
#define FILENAME_MAX_LEN 50
#define OP_NAME_LEN 20
#define ADMIN_PASSWORD "admin"

// Student data structure
typedef struct
{
    int id;
    char name[50];
    float marks;
} Student;

// History log data structure
typedef struct
{
    char operation[OP_NAME_LEN];
    char details[100];
    char userMode[10];
} HistoryEntry;

// Global variables
Student students[MAX_STUDENTS];
int studentCount = 0;

HistoryEntry history[MAX_HISTORY];
int history_count = 0;

// --- Function Prototypes ---
// Mode-specific menus
void runAdminMode();
void runUserMode();

// Student Record Operations
void addRecord();
void searchRecord();
void updateRecord();
void deleteRecord();
void displayAllRecords();

// File Handling Operations
void saveToFile();
void loadFromFile();
void exportToCSV();

// Statistics and Sorting
void sortRecords();
void calculateStatistics();

// Utility Functions
void displayHistory();
void logOperation(const char *op, const char *details, const char *mode);
void clearInputBuffer();

// --- Main Function ---
int main()
{
    int choice;
    char modeChoice[10];

    printf("Welcome to the Student Record Management System\n");

    do
    {
        printf("\nSelect Mode:\n");
        printf("1. Admin Mode\n");
        printf("2. User Mode\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice)
        {
        case 1:
            runAdminMode();
            break;
        case 2:
            runUserMode();
            break;
        case 3:
            printf("Exiting program. Goodbye!\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);

    return 0;
}

// --- Mode-Specific Menu Functions ---

void runAdminMode()
{
    loadFromFile();
    char password[20];
    printf("Enter Admin Password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;

    if (strcmp(password, ADMIN_PASSWORD) != 0)
    {
        printf("Incorrect password. Access denied.\n");
        logOperation("Admin Login", "Failed", "Admin");
        return;
    }

    int choice;
    do
    {
        printf("\n--- Admin Mode Menu ---\n");
        printf("1. Add Student Record\n");
        printf("2. Search for a Student\n");
        printf("3. Update a Student Record\n");
        printf("4. Delete a Student Record\n");
        printf("5. Display All Records\n");
        printf("6. Save Records to File \n");
        printf("7. Load Records from File \n");
        printf("8. Export Records to CSV\n");
        printf("9. Sort Records\n");
        printf("10. Calculate Statistics\n");
        printf("11. View History\n");
        printf("12. Logout\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1)
        {
            clearInputBuffer();
            printf("Invalid input!\n");
            continue;
        }
        clearInputBuffer();

        switch (choice)
        {
        case 1:
            addRecord();
            break;
        case 2:
            searchRecord();
            break;
        case 3:
            updateRecord();
            break;
        case 4:
            deleteRecord();
            break;
        case 5:
            displayAllRecords();
            break;
        case 6:
            saveToFile();
            break;
        case 7:
            loadFromFile();
            break;
        case 8:
            exportToCSV();
            break;
        case 9:
            sortRecords();
            break;
        case 10:
            calculateStatistics();
            break;
        case 11:
            displayHistory();
            break;
        case 12:
            printf("Logging out of Admin Mode.\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 12);
}

void runUserMode()
{
    int choice;
    printf("\n--- User Mode Menu ---\n");
    do
    {
        printf("1. Search for a Student\n");
        printf("2. Display All Records\n");
        printf("3. Calculate Statistics\n");
        printf("4. View History\n");
        printf("5. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice)
        {
        case 1:
            searchRecord();
            break;
        case 2:
            displayAllRecords();
            break;
        case 3:
            calculateStatistics();
            break;
        case 4:
            displayHistory();
            break;
        case 5:
            printf("Logging out of User Mode.\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);
}

// --- Student Record Operations ---

void addRecord()
{
    if (studentCount >= MAX_STUDENTS)
    {
        printf("Error: Maximum student capacity reached.\n");
        logOperation("Add", "Failed (Capacity full)", "Admin");
        return;
    }

    for (int i = 0; i < studentCount; i++)
    {
        if (students[i].id == students[studentCount].id)
        {
            printf("Student ID already exists!\n");
            return;
        }
    }

    printf("\n--- Add New Student Record ---\n");
    printf("Enter Student ID: ");
    scanf("%d", &students[studentCount].id);
    clearInputBuffer();

    printf("Enter Student Name: ");
    fgets(students[studentCount].name, sizeof(students[studentCount].name), stdin);
    students[studentCount].name[strcspn(students[studentCount].name, "\n")] = 0; // Remove newline

    printf("Enter Student Marks: ");
    scanf("%f", &students[studentCount].marks);

    char details[100];
    sprintf(details, "ID: %d, Name: %s", students[studentCount].id, students[studentCount].name);
    logOperation("Add", details, "Admin");

    studentCount++;
    saveToFile();
    printf("Student record added successfully.\n");
}

void searchRecord()
{
    if (studentCount == 0)
    {
        printf("No records to search.\n");
        logOperation("Search", "Failed (No records)", "Both");
        return;
    }
    int searchChoice, searchID;
    char searchName[50];
    int found = 0;

    printf("\n--- Search Student Record ---\n");
    printf("1. Search by ID\n");
    printf("2. Search by Name\n");
    printf("Enter your choice: ");
    scanf("%d", &searchChoice);
    clearInputBuffer();

    printf("\n+------------------------------------------------+\n");
    printf("|                    Search Results              |\n");
    printf("+------------+----------------------+-------------+\n");
    printf("| ID         | Name                 | Marks       |\n");
    printf("+------------+----------------------+-------------+\n");

    if (searchChoice == 1)
    {
        printf("Enter Student ID to search: ");
        scanf("%d", &searchID);
        for (int i = 0; i < studentCount; i++)
        {
            if (students[i].id == searchID)
            {
                printf("| %-10d | %-20s | %-11.2f |\n", students[i].id, students[i].name, students[i].marks);
                found = 1;
            }
        }
        char details[50];
        sprintf(details, "ID: %d", searchID);
        logOperation("Search", found ? details : "Failed (Not found)", "Both");
    }
    else if (searchChoice == 2)
    {
        printf("Enter Student Name to search: ");
        fgets(searchName, sizeof(searchName), stdin);
        searchName[strcspn(searchName, "\n")] = 0;
        for (int i = 0; i < studentCount; i++)
        {
            if (strcmp(students[i].name, searchName) == 0)
            {
                printf("| %-10d | %-20s | %-11.2f |\n", students[i].id, students[i].name, students[i].marks);
                found = 1;
            }
        }
        char details[50];
        sprintf(details, "Name: %s", searchName);
        logOperation("Search", found ? details : "Failed (Not found)", "Both");
    }
    else
    {
        printf("Invalid search choice.\n");
        logOperation("Search", "Failed (Invalid choice)", "Both");
    }
    printf("+------------+----------------------+-------------+\n");
    if (!found)
    {
        printf("Student not found.\n");
    }
}

void updateRecord()
{
    if (studentCount == 0)
    {
        printf("No records to update.\n");
        logOperation("Update", "Failed (No records)", "Admin");
        return;
    }
    int idToUpdate;
    printf("Enter the ID of the student to update: ");
    scanf("%d", &idToUpdate);
    clearInputBuffer();

    int foundIndex = -1;
    for (int i = 0; i < studentCount; i++)
    {
        if (students[i].id == idToUpdate)
        {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex != -1)
    {
        printf("Record found. Enter new details:\n");
        printf("Enter New Name: ");
        fgets(students[foundIndex].name, sizeof(students[foundIndex].name), stdin);
        students[foundIndex].name[strcspn(students[foundIndex].name, "\n")] = 0;

        printf("Enter New Marks: ");
        scanf("%f", &students[foundIndex].marks);
        clearInputBuffer();
        saveToFile();
        printf("Record updated successfully.\n");
        char details[100];
        sprintf(details, "Updated ID: %d, Name: %s", students[foundIndex].id, students[foundIndex].name);
        logOperation("Update", details, "Admin");
    }
    else
    {
        printf("Error: Student with ID %d not found.\n", idToUpdate);
        char details[50];
        sprintf(details, "Failed (ID: %d not found)", idToUpdate);
        logOperation("Update", details, "Admin");
    }
}

void deleteRecord()
{
    if (studentCount == 0)
    {
        printf("No records to delete.\n");
        logOperation("Delete", "Failed (No records)", "Admin");
        return;
    }
    int idToDelete;
    printf("Enter the ID of the student to delete: ");
    scanf("%d", &idToDelete);
    clearInputBuffer();
    int foundIndex = -1;
    for (int i = 0; i < studentCount; i++)
    {
        if (students[i].id == idToDelete)
        {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex != -1)
    {
        char details[100];
        sprintf(details, "Deleted ID: %d, Name: %s", students[foundIndex].id, students[foundIndex].name);
        logOperation("Delete", details, "Admin");

        for (int i = foundIndex; i < studentCount - 1; i++)
        {
            students[i] = students[i + 1];
        }
        studentCount--;
        saveToFile();
        printf("Record deleted successfully.\n");
    }
    else
    {
        printf("Error: Student with ID %d not found.\n", idToDelete);
        char details[50];
        sprintf(details, "Failed (ID: %d not found)", idToDelete);
        logOperation("Delete", details, "Admin");
    }
}

void displayAllRecords()
{
    if (studentCount == 0)
    {
        printf("No records to display.\n");
        logOperation("Display", "Failed (No records)", "Both");
        return;
    }

    printf("\n+------------------------------------------------+\n");
    printf("|                Student Records                 |\n");
    printf("+------------+----------------------+-------------+\n");
    printf("| ID         | Name                 | Marks       |\n");
    printf("+------------+----------------------+-------------+\n");
    for (int i = 0; i < studentCount; i++)
    {
        printf("| %-10d | %-20s | %-11.2f |\n", students[i].id, students[i].name, students[i].marks);
    }
    printf("+------------+----------------------+-------------+\n");
    logOperation("Display", "All records shown", "Both");
}

// --- File Handling Operations ---

void saveToFile()
{
    FILE *filePointer = fopen("students.txt", "w");

    if (filePointer == NULL)
    {
        printf("Error: Could not save file.\n");
        logOperation("Save", "Failed (File error)", "Admin");
        return;
    }

    // Save number of students
    fprintf(filePointer, "%d\n", studentCount);

    // Save student records
    for (int i = 0; i < studentCount; i++)
    {
        fprintf(filePointer, "%d %s %.2f\n",
                students[i].id,
                students[i].name,
                students[i].marks);
    }

    fclose(filePointer);

    printf("Records saved to students.txt successfully.\n");

    char details[50];
    sprintf(details, "%d records saved", studentCount);
    logOperation("Save", details, "Admin");
}

void loadFromFile()
{
    FILE *filePointer = fopen("students.txt", "r");

    if (filePointer == NULL)
    {
        printf("Error: Could not load file.\n");
        logOperation("Load", "Failed (File not found)", "Admin");
        return;
    }

    fscanf(filePointer, "%d", &studentCount);

    for (int i = 0; i < studentCount; i++)
    {
        fscanf(filePointer, "%d %49s %f",
               &students[i].id,
               students[i].name,
               &students[i].marks);
    }

    fclose(filePointer);

    printf("Records loaded from students.txt successfully.\n");

    char details[50];
    sprintf(details, "%d records loaded", studentCount);
    logOperation("Load", details, "Admin");
}

void exportToCSV()
{
    if (studentCount == 0)
    {
        printf("No records to export.\n");
        logOperation("Export", "Failed (No records)", "Admin");
        return;
    }
    FILE *filePointer;
    filePointer = fopen("students.csv", "w");
    if (filePointer == NULL)
    {
        printf("Error: Could not export to CSV file.\n");
        logOperation("Export", "Failed (File error)", "Admin");
        return;
    }

    fprintf(filePointer, "ID,Name,Marks\n");
    for (int i = 0; i < studentCount; i++)
    {
        fprintf(filePointer, "%d,%s,%.2f\n", students[i].id, students[i].name, students[i].marks);
    }

    fclose(filePointer);
    printf("Records exported to students.csv successfully.\n");
    char details[50];
    sprintf(details, "%d records exported", studentCount);
    logOperation("Export", details, "Admin");
}

// --- Statistics and Sorting ---

void sortRecords()
{
    if (studentCount <= 1)
    {
        printf("Not enough records to sort.\n");
        logOperation("Sort", "Failed (Not enough records)", "Admin");
        return;
    }

    int sortChoice;
    printf("\n--- Sort Records ---\n");
    printf("1. Sort by ID (Ascending)\n");
    printf("2. Sort by Name (Alphabetical)\n");
    printf("3. Sort by Marks (Descending)\n");
    printf("Enter your choice: ");
    scanf("%d", &sortChoice);

    if (sortChoice < 1 || sortChoice > 3)
    {
        printf("Invalid sort choice.\n");
        logOperation("Sort", "Failed (Invalid choice)", "Admin");
        return;
    }

    // Bubble Sort
    for (int i = 0; i < studentCount - 1; i++)
    {
        for (int j = 0; j < studentCount - i - 1; j++)
        {

            int swapNeeded = 0;

            switch (sortChoice)
            {
            case 1:
                if (students[j].id > students[j + 1].id)
                    swapNeeded = 1;
                break;

            case 2:
                if (strcmp(students[j].name, students[j + 1].name) > 0)
                    swapNeeded = 1;
                break;

            case 3:
                if (students[j].marks < students[j + 1].marks)
                    swapNeeded = 1;
                break;
            }

            if (swapNeeded)
            {
                Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }

    printf("Records sorted successfully.\n");

    // Save sorted records to students.txt
    saveToFile();

    logOperation("Sort", "Records sorted", "Admin");
}

void calculateStatistics()
{
    if (studentCount == 0)
    {
        printf("No records to calculate statistics.\n");
        logOperation("Calculate", "Failed (No records)", "Both");
        return;
    }

    float totalMarks = 0;
    float highestMarks = students[0].marks;
    float lowestMarks = students[0].marks;

    for (int i = 0; i < studentCount; i++)
    {
        totalMarks += students[i].marks;
        if (students[i].marks > highestMarks)
            highestMarks = students[i].marks;
        if (students[i].marks < lowestMarks)
            lowestMarks = students[i].marks;
    }

    printf("\n--- Student Statistics ---\n");
    printf("Total Records: %d\n", studentCount);
    printf("Average Marks: %.2f\n", totalMarks / studentCount);
    printf("Highest Marks: %.2f\n", highestMarks);
    printf("Lowest Marks: %.2f\n", lowestMarks);

    logOperation("Calculate", "Statistics computed", "Both");
}

// --- Utility Functions ---

void displayHistory()
{
    printf("\n\n+-------------------------------------------------------------------+\n");
    printf("|                         History of Operations                       |\n");
    printf("+----------------------+---------------------------------+-----------+\n");
    printf("| Operation            | Details                         | Mode      |\n");
    printf("+----------------------+---------------------------------+-----------+\n");
    if (history_count == 0)
    {
        printf("| %-67s |\n", "No operations recorded yet.");
    }
    else
    {
        for (int i = 0; i < history_count; i++)
        {
            printf("| %-20s | %-31s | %-9s |\n",
                   history[i].operation, history[i].details, history[i].userMode);
        }
    }
    printf("+----------------------+---------------------------------+-----------+\n\n");
}

void logOperation(const char *op, const char *details, const char *mode)
{
    if (history_count < MAX_HISTORY)
    {
        strcpy(history[history_count].operation, op);
        strcpy(history[history_count].details, details);
        strcpy(history[history_count].userMode, mode);
        history_count++;
    }
    else
    {
        printf("History log is full.\n");
    }
}

void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}
