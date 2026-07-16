#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// --- Data Structure Definitions ---
#define MAX_RECORDS 100

// Main user structure for authentication and roles
typedef struct
{
    int id;
    char username[50];
    char password[50];
    char role[20]; // "student" or "teacher" or "admin"
} User;

// Structure for a student
typedef struct
{
    int id;
    char name[100];
    int class;
    float feesPaid;
    float totalFees;
    int attendanceCount; // Total present days
} Student;

// Structure for a teacher
typedef struct
{
    int id;
    char name[100];
    char subject[50];
} Teacher;

// Structure for a class
typedef struct
{
    int id;
    char name[50];
    int studentCount;
} Class;

// Structure for a grade/result
typedef struct
{
    int studentId;
    char subject[50];
    float score;
} Grade;

// Structure for an announcement
typedef struct
{
    int id;
    char message[256];
} Announcement;

// Structure for a fee payment record
typedef struct
{
    int studentId;
    float amount;
} FeePayment;

// Structure for an expense record
typedef struct
{
    char description[100];
    float amount;
} Expense;

// --- Global Arrays and Counters ---
User users[MAX_RECORDS];
Student students[MAX_RECORDS];
Teacher teachers[MAX_RECORDS];
Class classes[10]; // Assuming up to 10 classes
Grade grades[MAX_RECORDS];
Announcement announcements[MAX_RECORDS];
Expense expenses[MAX_RECORDS];

int userCount = 0;
int studentCount = 0;
int teacherCount = 0;
int classCount = 0;
int gradeCount = 0;
int announcementCount = 0;
int expenseCount = 0;

// --- Function Prototypes ---
void loadData();
void saveData();
void clearInputBuffer();

// Main menu and login
void mainMenu();
void login();
void registration();
void adminMenu();
void studentMenu(int userId);
void teacherMenu(int userId);

// Admin functions
void admissions();
void displayAllStudents();
void displayStudentInfo(int id);
void displayAllStaff();
void displayStaffInfo(int id);
void displayClassDistribution();
void manageExamsAndResults();
void manageFeesAndPayments();
void manageExpenses();
void manageAnnouncements();
void manageAttendance();
void viewAutomatedReports();
void exportDataToFile();

// Utility functions
int findUser(char *username, char *password, char *role);
int findStudent(int studentId);
int findTeacher(int teacherId);
int findClass(int classId);

// --- Main Function ---
void createInitialData();

int main(){
    loadData();

    if (userCount == 0)
    {
        createInitialData();
        saveData();
    }

    mainMenu();
    saveData();
    return 0;
}

// --- Data Persistence Functions (File I/O) ---
void loadData(){
    FILE *file;
    int i;

    // Users
    file = fopen("users.txt", "r");
    if (file)
    {
        fscanf(file, "%d\n", &userCount);
        for (i = 0; i < userCount; i++)
        {
            fscanf(file, "%d,%49[^,],%49[^,],%19[^\n]\n",
                   &users[i].id,
                   users[i].username,
                   users[i].password,
                   users[i].role);
        }
        fclose(file);
    }

    // Students
    file = fopen("students.txt", "r");
    if (file)
    {
        fscanf(file, "%d\n", &studentCount);
        for (i = 0; i < studentCount; i++)
        {
            fscanf(file, "%d,%99[^,],%d,%f,%f,%d\n",
                   &students[i].id,
                   students[i].name,
                   &students[i].class,
                   &students[i].feesPaid,
                   &students[i].totalFees,
                   &students[i].attendanceCount);
        }
        fclose(file);
    }

    // Teachers
    file = fopen("teachers.txt", "r");
    if (file)
    {
        fscanf(file, "%d\n", &teacherCount);
        for (i = 0; i < teacherCount; i++)
        {
            fscanf(file, "%d,%99[^,],%49[^\n]\n",
                   &teachers[i].id,
                   teachers[i].name,
                   teachers[i].subject);
        }
        fclose(file);
    }

    // Classes
    file = fopen("classes.txt", "r");
    if (file)
    {
        fscanf(file, "%d\n", &classCount);
        for (i = 0; i < classCount; i++)
        {
            fscanf(file, "%d,%49[^,],%d\n",
                   &classes[i].id,
                   classes[i].name,
                   &classes[i].studentCount);
        }
        fclose(file);
    }

    // Grades
    file = fopen("grades.txt", "r");
    if (file)
    {
        fscanf(file, "%d\n", &gradeCount);
        for (i = 0; i < gradeCount; i++)
        {
            fscanf(file, "%d,%49[^,],%f\n",
                   &grades[i].studentId,
                   grades[i].subject,
                   &grades[i].score);
        }
        fclose(file);
    }

    // Announcements
    file = fopen("announcements.txt", "r");
    if (file)
    {
        fscanf(file, "%d\n", &announcementCount);
        for (i = 0; i < announcementCount; i++)
        {
            fscanf(file, "%d,%255[^\n]\n",
                   &announcements[i].id,
                   announcements[i].message);
        }
        fclose(file);
    }

    // Expenses
    file = fopen("expenses.txt", "r");
    if (file)
    {
        fscanf(file, "%d\n", &expenseCount);
        for (i = 0; i < expenseCount; i++)
        {
            fscanf(file, "%99[^,],%f\n",
                   expenses[i].description,
                   &expenses[i].amount);
        }
        fclose(file);
    }
}

void saveData(){
    FILE *file;
    int i;

    // Users
    file = fopen("users.txt", "w");
    if (file)
    {
        fprintf(file, "%d\n", userCount);
        for (i = 0; i < userCount; i++)
        {
            fprintf(file, "%d,%s,%s,%s\n",
                    users[i].id,
                    users[i].username,
                    users[i].password,
                    users[i].role);
        }
        fclose(file);
    }

    // Students
    file = fopen("students.txt", "w");
    if (file)
    {
        fprintf(file, "%d\n", studentCount);
        for (i = 0; i < studentCount; i++)
        {
            fprintf(file, "%d,%s,%d,%.2f,%.2f,%d\n",
                    students[i].id,
                    students[i].name,
                    students[i].class,
                    students[i].feesPaid,
                    students[i].totalFees,
                    students[i].attendanceCount);
        }
        fclose(file);
    }

    // Teachers
    file = fopen("teachers.txt", "w");
    if (file)
    {
        fprintf(file, "%d\n", teacherCount);
        for (i = 0; i < teacherCount; i++)
        {
            fprintf(file, "%d,%s,%s\n",
                    teachers[i].id,
                    teachers[i].name,
                    teachers[i].subject);
        }
        fclose(file);
    }

    // Classes
    file = fopen("classes.txt", "w");
    if (file)
    {
        fprintf(file, "%d\n", classCount);
        for (i = 0; i < classCount; i++)
        {
            fprintf(file, "%d,%s,%d\n",
                    classes[i].id,
                    classes[i].name,
                    classes[i].studentCount);
        }
        fclose(file);
    }

    // Grades
    file = fopen("grades.txt", "w");
    if (file)
    {
        fprintf(file, "%d\n", gradeCount);
        for (i = 0; i < gradeCount; i++)
        {
            fprintf(file, "%d,%s,%.2f\n",
                    grades[i].studentId,
                    grades[i].subject,
                    grades[i].score);
        }
        fclose(file);
    }

    // Announcements
    file = fopen("announcements.txt", "w");
    if (file)
    {
        fprintf(file, "%d\n", announcementCount);
        for (i = 0; i < announcementCount; i++)
        {
            fprintf(file, "%d,%s\n",
                    announcements[i].id,
                    announcements[i].message);
        }
        fclose(file);
    }

    // Expenses
    file = fopen("expenses.txt", "w");
    if (file)
    {
        fprintf(file, "%d\n", expenseCount);
        for (i = 0; i < expenseCount; i++)
        {
            fprintf(file, "%s,%.2f\n",
                    expenses[i].description,
                    expenses[i].amount);
        }
        fclose(file);
    }
}

// --- Main Menu and Authentication ---
void mainMenu(){
    int choice;
    do
    {
        printf("\n--- School Management System ---\n");
        printf("1. Login\n");
        printf("2. Register New User\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice)
        {
        case 1:
            login();
            break;
        case 2:
            registration();
            break;
        case 3:
            printf("Exiting program. Goodbye!\n");
            return;
        default:
            printf("Invalid choice.\n");
        }
    } while (1);
}

void login(){
    char username[50], password[50], role[20];
    int userId;
    printf("\n--- Login ---\n");
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);
    printf("Enter role (admin/teacher/student): ");
    scanf("%s", role);

    userId = findUser(username, password, role);
    if (userId != -1)
    {
        printf("\nLogin successful! Welcome, %s.\n", username);
        if (strcmp(role, "admin") == 0)
            adminMenu();
        else if (strcmp(role, "student") == 0)
            studentMenu(userId);
        else if (strcmp(role, "teacher") == 0)
            teacherMenu(userId);
    }
    else
    {
        printf("\nInvalid credentials or role.\n");
    }
}

void registration(){
    char username[50], password[50], role[20];
    if (userCount >= MAX_RECORDS)
    {
        printf("Error: Maximum user limit reached.\n");
        return;
    }

    printf("\n--- New User Registration ---\n");
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);
    printf("Enter role (teacher/student): ");
    scanf("%s", role);

    if (strcmp(role, "admin") == 0)
    {
        printf("Admin registration is not allowed here.\n");
        return;
    }

    if (strcmp(role, "student") == 0)
    {
        if (studentCount >= MAX_RECORDS)
        {
            printf("Error: Maximum student limit reached.\n");
            return;
        }
        students[studentCount].id = studentCount + 1;
        strcpy(students[studentCount].name, username);
        students[studentCount].feesPaid = 0.0;
        printf("Enter class (1-10): ");
        scanf("%d", &students[studentCount].class);
        printf("Enter total fees: ");
        scanf("%f", &students[studentCount].totalFees);
        students[studentCount].attendanceCount = 0;
        studentCount++;
    }
    else if (strcmp(role, "teacher") == 0)
    {
        if (teacherCount >= MAX_RECORDS)
        {
            printf("Error: Maximum teacher limit reached.\n");
            return;
        }
        teachers[teacherCount].id = teacherCount + 1;
        strcpy(teachers[teacherCount].name, username);
        printf("Enter subject: ");
        scanf("%s", teachers[teacherCount].subject);
        teacherCount++;
    }
    else
    {
        printf("Invalid role. Please choose 'student' or 'teacher'.\n");
        return;
    }

    users[userCount].id = (strcmp(role, "student") == 0) ? students[studentCount - 1].id : teachers[teacherCount - 1].id;
    strcpy(users[userCount].username, username);
    strcpy(users[userCount].password, password);
    strcpy(users[userCount].role, role);
    userCount++;
    printf("Registration successful! Your ID is: %d\n", users[userCount - 1].id);
}

// --- Admin Functions ---
void adminMenu(){
    int choice;
    do
    {
        printf("\n--- Admin Menu ---\n");
        printf("1. Manage Admissions & Students\n");
        printf("2. Manage Staff Profiles\n");
        printf("3. Manage Timetable & Class Distribution\n");
        printf("4. Manage Exams & Results\n");
        printf("5. Manage Fees & Payments\n");
        printf("6. Manage Expenses\n");
        printf("7. Manage Announcements\n");
        printf("8. View Automated Reports\n");
        printf("9. Export Data to File\n");
        printf("10. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice)
        {
        case 1:
            admissions();
            break;
        case 2:
            displayAllStaff();
            break;
        case 3:
            displayClassDistribution();
            break;
        case 4:
            manageExamsAndResults();
            break;
        case 5:
            manageFeesAndPayments();
            break;
        case 6:
            manageExpenses();
            break;
        case 7:
            manageAnnouncements();
            break;
        case 8:
            viewAutomatedReports();
            break;
        case 9:
            exportDataToFile();
            break;
        case 10:
            printf("Logging out...\n");
            return;
        default:
            printf("Invalid choice.\n");
        }
    } while (1);
}

void admissions(){
    int choice;
    do
    {
        printf("\n--- Admissions & Student Management ---\n");
        printf("1. Add a new student\n");
        printf("2. View all students\n");
        printf("3. View student information by ID\n");
        printf("4. Go back\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();
        switch (choice)
        {
        case 1:
            registration();
            break;
        case 2:
            displayAllStudents();
            break;
        case 3:
        {
            int id;
            printf("Enter student ID: ");
            scanf("%d", &id);
            displayStudentInfo(id);
            break;
        }
        case 4:
            return;
        default:
            printf("Invalid choice.\n");
        }
    } while (1);
}

void displayAllStudents(){
    printf("\n--- All Students ---\n");
    if (studentCount == 0)
    {
        printf("No students registered.\n");
        return;
    }
    printf("%-5s %-20s %-10s\n", "ID", "Name", "Class");
    printf("----------------------------------\n");
    for (int i = 0; i < studentCount; i++)
    {
        printf("%-5d %-20s %-10d\n", students[i].id, students[i].name, students[i].class);
    }
}

void displayStudentInfo(int id){
    int studentIndex = findStudent(id);
    if (studentIndex == -1)
    {
        printf("Student not found.\n");
        return;
    }
    Student s = students[studentIndex];
    printf("\n--- Student Information ---\n");
    printf("ID: %d\n", s.id);
    printf("Name: %s\n", s.name);
    printf("Class: %d\n", s.class);
    printf("Attendance: %d days\n", s.attendanceCount);
    printf("Fees Paid: $%.2f / $%.2f\n", s.feesPaid, s.totalFees);
}

void displayAllStaff(){
    printf("\n--- All Staff ---\n");
    if (teacherCount == 0)
    {
        printf("No teachers registered.\n");
        return;
    }
    printf("%-5s %-20s %-15s\n", "ID", "Name", "Subject");
    printf("--------------------------------------\n");
    for (int i = 0; i < teacherCount; i++)
    {
        printf("%-5d %-20s %-15s\n", teachers[i].id, teachers[i].name, teachers[i].subject);
    }
}

void displayClassDistribution(){
    int classCounts[11] = {0}; 
    for (int i = 0; i < studentCount; i++)
    {
        if (students[i].class >= 1 && students[i].class <= 10)
        {
            classCounts[students[i].class]++;
        }
    }
    printf("\n--- Class Distribution ---\n");
    for (int i = 1; i <= 10; i++)
    {
        printf("Class %d: %d students\n", i, classCounts[i]);
    }
}

void manageExamsAndResults(){
    int choice;
    printf("\n--- Exams & Results ---\n");
    printf("1. Add a student's grade\n");
    printf("2. View all grades\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    if (choice == 1)
    {
        if (gradeCount >= MAX_RECORDS)
        {
            printf("Error: Max grade limit reached.\n");
            return;
        }
        printf("Enter student ID: ");
        scanf("%d", &grades[gradeCount].studentId);
        printf("Enter subject: ");
        scanf("%s", grades[gradeCount].subject);
        printf("Enter score: ");
        scanf("%f", &grades[gradeCount].score);
        gradeCount++;
        printf("Grade added.\n");
    }
    else if (choice == 2)
    {
        printf("\n--- All Grades ---\n");
        printf("%-15s %-15s %-10s\n", "Student ID", "Subject", "Score");
        printf("-----------------------------------------\n");
        for (int i = 0; i < gradeCount; i++)
        {
            printf("%-15d %-15s %-10.2f\n", grades[i].studentId, grades[i].subject, grades[i].score);
        }
    }
}

void manageFeesAndPayments(){
    int choice, id;
    float amount;
    int studentIndex;
    printf("\n--- Fees & Payments ---\n");
    printf("1. Record a payment\n");
    printf("2. View fee status\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        printf("Enter student ID: ");
        scanf("%d", &id);
        studentIndex = findStudent(id);
        if (studentIndex != -1)
        {
            printf("Enter amount to pay: ");
            scanf("%f", &amount);
            students[studentIndex].feesPaid += amount;
            printf("Payment recorded. New balance for %s: $%.2f\n", students[studentIndex].name, students[studentIndex].feesPaid);
        }
        else
        {
            printf("Student not found.\n");
        }
    }
    else if (choice == 2)
    {
        printf("\n--- Fee Status Report ---\n");
        printf("%-5s %-20s %-15s %-15s\n", "ID", "Name", "Fees Paid", "Fees Due");
        printf("----------------------------------------------------------\n");
        for (int i = 0; i < studentCount; i++)
        {
            printf("%-5d %-20s $%-14.2f $%-14.2f\n", students[i].id, students[i].name, students[i].feesPaid, students[i].totalFees - students[i].feesPaid);
        }
    }
}

void manageExpenses(){
    int choice;
    printf("\n--- Manage Expenses ---\n");
    printf("1. Add a new expense\n");
    printf("2. View all expenses\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    if (choice == 1)
    {
        if (expenseCount >= MAX_RECORDS)
        {
            printf("Error: Max expense limit reached.\n");
            return;
        }
        printf("Enter description: ");
        scanf(" %[^\n]", expenses[expenseCount].description);
        printf("Enter amount: ");
        scanf("%f", &expenses[expenseCount].amount);
        expenseCount++;
        printf("Expense added.\n");
    }
    else if (choice == 2)
    {
        printf("\n--- All Expenses ---\n");
        printf("%-30s %-10s\n", "Description", "Amount");
        printf("----------------------------------------\n");
        float total = 0.0;
        for (int i = 0; i < expenseCount; i++)
        {
            printf("%-30s $%-10.2f\n", expenses[i].description, expenses[i].amount);
            total += expenses[i].amount;
        }
        printf("----------------------------------------\n");
        printf("Total Expenses: $%.2f\n", total);
    }
}

void manageAnnouncements()
{
    int choice;
    printf("\n--- Manage Announcements ---\n");
    printf("1. Post a new announcement\n");
    printf("2. View all announcements\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    if (choice == 1)
    {
        if (announcementCount >= MAX_RECORDS)
        {
            printf("Error: Max announcement limit reached.\n");
            return;
        }
        announcements[announcementCount].id = announcementCount + 1;
        printf("Enter announcement message: ");
        scanf(" %[^\n]", announcements[announcementCount].message);
        announcementCount++;
        printf("Announcement posted.\n");
    }
    else if (choice == 2)
    {
        printf("\n--- All Announcements ---\n");
        if (announcementCount == 0)
        {
            printf("No announcements yet.\n");
            return;
        }
        for (int i = 0; i < announcementCount; i++)
        {
            printf("ID %d: %s\n", announcements[i].id, announcements[i].message);
        }
    }
}

void viewAutomatedReports()
{
    float totalFeesPaid = 0;
    float totalFeesDue = 0;
    float totalExpenses = 0;

    for (int i = 0; i < studentCount; i++)
    {
        totalFeesPaid += students[i].feesPaid;
        totalFeesDue += students[i].totalFees - students[i].feesPaid;
    }

    for (int i = 0; i < expenseCount; i++)
    {
        totalExpenses += expenses[i].amount;
    }

    printf("\n--- Automated Financial Report ---\n");
    printf("Total Students: %d\n", studentCount);
    printf("Total Teachers: %d\n", teacherCount);
    printf("Total Fees Collected: $%.2f\n", totalFeesPaid);
    printf("Total Fees Due: $%.2f\n", totalFeesDue);
    printf("Total Expenses: $%.2f\n", totalExpenses);
    printf("Net Profit/Loss: $%.2f\n", totalFeesPaid - totalExpenses);
}

void exportDataToFile()
{
    FILE *file = fopen("school_report.txt", "w");
    if (file == NULL)
    {
        printf("Error creating file.\n");
        return;
    }

    fprintf(file, "--- School Management Report ---\n\n");

    fprintf(file, "STUDENT INFORMATION\n");
    fprintf(file, "%-5s %-20s %-10s\n", "ID", "Name", "Class");
    fprintf(file, "----------------------------------\n");
    for (int i = 0; i < studentCount; i++)
    {
        fprintf(file, "%-5d %-20s %-10d\n", students[i].id, students[i].name, students[i].class);
    }
    fprintf(file, "\n");

    fprintf(file, "FINANCIAL REPORT\n");
    float totalFeesPaid = 0;
    float totalFeesDue = 0;
    float totalExpenses = 0;
    for (int i = 0; i < studentCount; i++)
    {
        totalFeesPaid += students[i].feesPaid;
        totalFeesDue += students[i].totalFees - students[i].feesPaid;
    }
    for (int i = 0; i < expenseCount; i++)
    {
        totalExpenses += expenses[i].amount;
    }
    fprintf(file, "Total Fees Collected: $%.2f\n", totalFeesPaid);
    fprintf(file, "Total Fees Due: $%.2f\n", totalFeesDue);
    fprintf(file, "Total Expenses: $%.2f\n", totalExpenses);
    fprintf(file, "Net Profit/Loss: $%.2f\n", totalFeesPaid - totalExpenses);

    fclose(file);
    printf("Data successfully exported to 'school_report.txt'.\n");
}

// --- User Functions ---
void studentMenu(int userId){
    int choice;
    int studentIndex = findStudent(userId);
    if (studentIndex == -1)
    {
        printf("Student data not found.\n");
        return;
    }
    do
    {
        printf("\n--- Student Menu ---\n");
        printf("Welcome, %s!\n", students[studentIndex].name);
        printf("1. View my grades\n");
        printf("2. View my fee status\n");
        printf("3. View announcements\n");
        printf("4. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();
        switch (choice)
        {
        case 1:
        {
            printf("\n--- My Grades ---\n");
            printf("%-15s %-10s\n", "Subject", "Score");
            printf("------------------------\n");
            int found = 0;
            for (int i = 0; i < gradeCount; i++)
            {
                if (grades[i].studentId == userId)
                {
                    printf("%-15s %-10.2f\n", grades[i].subject, grades[i].score);
                    found = 1;
                }
            }
            if (!found)
                printf("No grades found.\n");
            break;
        }
        case 2:
            printf("\n--- My Fee Status ---\n");
            printf("Fees Paid: $%.2f\n", students[studentIndex].feesPaid);
            printf("Total Fees: $%.2f\n", students[studentIndex].totalFees);
            printf("Fees Due: $%.2f\n", students[studentIndex].totalFees - students[studentIndex].feesPaid);
            break;
        case 3:
            manageAnnouncements();
            break;
        case 4:
            printf("Logging out...\n");
            return;
        default:
            printf("Invalid choice.\n");
        }
    } while (1);
}

void teacherMenu(int userId){
    int choice;
    do
    {
        printf("\n--- Teacher Menu ---\n");
        printf("1. View all students\n");
        printf("2. View student grades\n");
        printf("3. View announcements\n");
        printf("4. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();
        switch (choice)
        {
        case 1:
            displayAllStudents();
            break;
        case 2:
            manageExamsAndResults();
            break;
        case 3:
            manageAnnouncements();
            break;
        case 4:
            printf("Logging out...\n");
            return;
        default:
            printf("Invalid choice.\n");
        }
    } while (1);
}

// --- Utility Functions ---
int findUser(char *username, char *password, char *role){
    for (int i = 0; i < userCount; i++)
    {
        if (strcmp(users[i].username, username) == 0 &&
            strcmp(users[i].password, password) == 0 &&
            strcmp(users[i].role, role) == 0)
        {
            return users[i].id;
        }
    }
    return -1;
}

int findStudent(int studentId){
    for (int i = 0; i < studentCount; i++)
    {
        if (students[i].id == studentId)
        {
            return i;
        }
    }
    return -1;
}

int findTeacher(int teacherId){
    for (int i = 0; i < teacherCount; i++)
    {
        if (teachers[i].id == teacherId)
        {
            return i;
        }
    }
    return -1;
}

void clearInputBuffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void createInitialData(){
    // Initial Admin user
    users[userCount++] = (User){1, "admin", "admin", "admin"};
    // for (int i = 0; i < userCount; i++)
    // {
    //     printf(users[i].id , "->id ,", users[i].username ,"->username ," , users[i].password ,"->password ," , users[i].role , "->role");
    // }

    printf("Initial data created. Please restart the program.\n");
}
