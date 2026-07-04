#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_HISTORY 50
#define FILENAME_MAX_LEN 50
#define OP_NAME_LEN 20
#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "admin123"

// --- Data Structures ---

// Event data structure
typedef struct {
    int id;
    char name[50];
    char description[100];
    int year, month, day;
    char priority[10];
    int isRecurring;
    int reminderDays;
} Event;

// Linked list node for events
typedef struct EventNode {
    Event event;
    struct EventNode *next;
} EventNode;

// User data structure
typedef struct {
    char username[20];
    char password[20];
} User;

// Linked list node for users
typedef struct UserNode {
    User user;
    struct UserNode *next;
} UserNode;

// History log data structure
typedef struct {
    char operation[OP_NAME_LEN];
    char details[100];
    char userMode[10];
    char timestamp[30];
} HistoryEntry;

// --- Global Variables ---
UserNode *userHead = NULL; // Head of the linked list for users
EventNode *eventHead = NULL; // Head of the linked list for the current user's events

HistoryEntry history[MAX_HISTORY];
int history_count = 0;
char currentUsername[20] = "";

// --- Function Prototypes ---
// Mode-specific menus
void mainMenu();
void adminMenu();
void userMenu();

// User Authentication (Linked List)
int registerUser();
int loginUser();
void saveUsers();
void loadUsers();
void deleteUser(); 
void displayConnectedUsers();
int isUserAdmin(const char* username);
void freeUserList();

// Event Operations (Linked List)
void addEvent();
void searchAndFilterEvents();
void updateEvent();
void deleteEvent();
void displayAllEvents();
void displayEvent(const Event *e);
void sortEventsByPriority();

// File Handling
void saveEventsToFile(const char* filename);
void loadEventsFromFile(const char* filename);
void freeEventList();

// Utility Functions
void checkReminders();
void displayHistory();
void logOperation(const char *op, const char *details, const char *mode);
void clearInputBuffer();

// --- Main Function ---
int main() {
    loadUsers();
    mainMenu();
    freeUserList(); // Clean up memory before exiting
    return 0;
}

// --- Menu Functions ---

void mainMenu() {
    printf("initial ADMIN username : %s - password : %s ",ADMIN_USERNAME,ADMIN_PASSWORD);
    int choice;
    do {
        printf("\n--- Event Management System ---\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1: 
                registerUser(); 
                break;
            case 2: 
                if (loginUser()) {
                    if (isUserAdmin(currentUsername)) {
                        adminMenu();
                    } else {
                        userMenu();
                    }
                }
                break;
            case 3: 
                printf("Exiting program. Goodbye!\n"); 
                break;
            default: 
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);
}

void adminMenu() {
    int choice;
    printf("Admin login successful. Welcome, %s!\n", currentUsername);
    loadEventsFromFile(currentUsername);

    do {
        printf("\n--- Admin Mode Menu ---\n");
        printf("1. Add a New Event\n");
        printf("2. Update an Existing Event\n");
        printf("3. Delete an Event\n");
        printf("4. Display All Events\n");
        printf("5. Search and Filter Events\n");
        printf("6. Sort Events by Priority\n");
        printf("7. Check for Event Reminders\n");
        printf("8. View History\n");
        printf("9. Delete a User Account\n");
        printf("10. Display Connected Users\n");
        printf("11. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1: addEvent(); break;
            case 2: updateEvent(); break;
            case 3: deleteEvent(); break;
            case 4: displayAllEvents(); break;
            case 5: searchAndFilterEvents(); break;
            case 6: sortEventsByPriority(); break;
            case 7: checkReminders(); break;
            case 8: displayHistory(); break;
            case 9: deleteUser(); break;
            case 10: displayConnectedUsers(); break;
            case 11:
                saveEventsToFile(currentUsername);
                freeEventList();
                strcpy(currentUsername, "");
                printf("Logging out of Admin Mode.\n");
                break;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 11);
}

void userMenu() {
    int choice;
    printf("User login successful. Welcome, %s!\n", currentUsername);
    loadEventsFromFile(currentUsername);

    do {
        printf("\n--- User Mode Menu ---\n");
        printf("1. Display All Events\n");
        printf("2. Search and Filter Events\n");
        printf("3. Sort Events by Priority\n");
        printf("4. Check for Event Reminders\n");
        printf("5. View History\n");
        printf("6. Display Connected Users\n"); // NEW option
        printf("7. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1: displayAllEvents(); break;
            case 2: searchAndFilterEvents(); break;
            case 3: sortEventsByPriority(); break;
            case 4: checkReminders(); break;
            case 5: displayHistory(); break;
            case 6: displayConnectedUsers(); break; // NEW case
            case 7:
                saveEventsToFile(currentUsername);
                freeEventList();
                strcpy(currentUsername, "");
                printf("Logging out of User Mode.\n");
                break;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 7);
}

// --- User Authentication (Linked List) ---

void saveUsers() {
    FILE *fp = fopen("users.txt", "w");
    if (fp == NULL) {
        printf("Error: Could not save user data.\n");
        return;
    }
    UserNode *current = userHead;
    int userCount = 0;
    while(current != NULL) {
        userCount++;
        current = current->next;
    }
    fprintf(fp, "%d\n", userCount);
    
    current = userHead;
    while(current != NULL) {
        fprintf(fp, "%s %s\n",
        current->user.username,
        current->user.password);
        current = current->next;
    }
    
    fclose(fp);
}

void loadUsers() {
    FILE *fp = fopen("users.txt", "r");
    if (fp == NULL) {
        userHead = NULL;
        return;
    }
    
    int userCount = 0;
    fscanf(fp, "%d", &userCount);
    
    if (userCount > 0) {
        User tempUser;
        for(int i=0;i<userCount;i++)
        {
            fscanf(fp,"%19s %19s",
                tempUser.username,
                tempUser.password);

            UserNode *newNode = malloc(sizeof(UserNode));

            newNode->user = tempUser;
            newNode->next = userHead;
            userHead = newNode;
        }
    }
    
    fclose(fp);
}

int isUserAdmin(const char* username) {
    return strcmp(username, ADMIN_USERNAME) == 0;
}

int registerUser() {
    UserNode *newNode = (UserNode *)malloc(sizeof(UserNode));
    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        logOperation("Register", "Failed (Memory error)", "Guest");
        return 0;
    }

    printf("\n--- User Registration ---\n");
    printf("Enter a new username: ");
    fgets(newNode->user.username, sizeof(newNode->user.username), stdin);
    newNode->user.username[strcspn(newNode->user.username, "\n")] = 0;

    UserNode *current = userHead;
    while (current != NULL) {
        if (strcmp(current->user.username, newNode->user.username) == 0) {
            printf("Username already exists. Please choose a different one.\n");
            free(newNode);
            logOperation("Register", "Failed (Username exists)", "Guest");
            return 0;
        }
        current = current->next;
    }

    printf("Enter a new password: ");
    fgets(newNode->user.password, sizeof(newNode->user.password), stdin);
    newNode->user.password[strcspn(newNode->user.password, "\n")] = 0;
    
    newNode->next = userHead;
    userHead = newNode;
    saveUsers();
    
    printf("Registration successful! You can now log in.\n");
    logOperation("Register", "New user created", "Guest");
    return 1;
}

int loginUser() {
    char username[20], password[20];
    printf("\n--- User Login ---\n");
    printf("Enter username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;

    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;

    UserNode *current = userHead;
    while (current != NULL) {
        if (strcmp(current->user.username, username) == 0 && strcmp(current->user.password, password) == 0) {
            strcpy(currentUsername, username);
            logOperation("Login", "Successful", isUserAdmin(username) ? "Admin" : "User");
            return 1;
        }
        current = current->next;
    }
    
    if (strcmp(username, ADMIN_USERNAME) == 0 && strcmp(password, ADMIN_PASSWORD) == 0) {
        strcpy(currentUsername, ADMIN_USERNAME);
        logOperation("Login", "Successful (Admin)", "Admin");
        return 1;
    }

    printf("Invalid username or password.\n");
    logOperation("Login", "Failed (Invalid credentials)", "Guest");
    return 0;
}

void deleteUser() {
    char userToDelete[20];
    printf("\n--- Delete User Account ---\n");
    printf("Enter the username to delete: ");
    fgets(userToDelete, sizeof(userToDelete), stdin);
    userToDelete[strcspn(userToDelete, "\n")] = 0;

    if (strcmp(userToDelete, ADMIN_USERNAME) == 0) {
        printf("Error: The admin account cannot be deleted.\n");
        logOperation("Delete User", "Failed (Attempted to delete admin)", "Admin");
        return;
    }
    
    UserNode *current = userHead;
    UserNode *prev = NULL;

    while (current != NULL && strcmp(current->user.username, userToDelete) != 0) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Error: User '%s' not found.\n", userToDelete);
        logOperation("Delete User", "Failed (User not found)", "Admin");
        return;
    }

    // Construct the filename for the user's events data
    char filename[FILENAME_MAX_LEN];
    sprintf(filename, "%s_events.txt", userToDelete);
    
    // Delete the user's event data file
    if (remove(filename) == 0) {
        printf("Successfully deleted user's data file: %s\n", filename);
    } else {
        printf("Warning: Could not find or delete user's data file: %s\n", filename);
    }

    if (prev == NULL) {
        userHead = current->next;
    } else {
        prev->next = current->next;
    }

    printf("User '%s' and their data have been deleted successfully.\n", userToDelete);
    logOperation("Delete User", "Successfully deleted user", "Admin");
    free(current);
    saveUsers();
}

void displayConnectedUsers() {
    printf("\n--- Currently Connected User ---\n");
    if (strlen(currentUsername) > 0) {
        printf("Current user: %s\n", currentUsername);
    } else {
        printf("No user is currently logged in.\n");
    }
    logOperation("Connected Users", "Displaying connected user", isUserAdmin(currentUsername) ? "Admin" : "User");
}

void freeUserList() {
    UserNode *current = userHead;
    UserNode *next;
    while(current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    userHead = NULL;
}

// --- Event Operations (Linked List) ---

void addEvent() {
    EventNode *newNode = (EventNode *)malloc(sizeof(EventNode));
    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        logOperation("Add Event", "Failed (Memory error)", "Admin");
        return;
    }

    printf("\n--- Add New Event ---\n");
    printf("Enter Event ID: ");
    scanf("%d", &newNode->event.id);
    clearInputBuffer();

    printf("Enter Event Name: ");
    fgets(newNode->event.name, sizeof(newNode->event.name), stdin);
    newNode->event.name[strcspn(newNode->event.name, "\n")] = 0;

    printf("Enter Event Description: ");
    fgets(newNode->event.description, sizeof(newNode->event.description), stdin);
    newNode->event.description[strcspn(newNode->event.description, "\n")] = 0;
    
    printf("Enter Event Date (YYYY MM DD): ");
    scanf("%d %d %d", &newNode->event.year, &newNode->event.month, &newNode->event.day);
    clearInputBuffer();
    
    printf("Enter Event Priority (High/Medium/Low): ");
    fgets(newNode->event.priority, sizeof(newNode->event.priority), stdin);
    newNode->event.priority[strcspn(newNode->event.priority, "\n")] = 0;

    printf("Is this a recurring event? (1 for Yes, 0 for No): ");
    scanf("%d", &newNode->event.isRecurring);
    
    printf("How many days before the event for a reminder? ");
    scanf("%d", &newNode->event.reminderDays);

    newNode->next = eventHead;
    eventHead = newNode;
    
    char details[100];
    sprintf(details, "ID: %d, Name: %s", newNode->event.id, newNode->event.name);
    logOperation("Add Event", details, "Admin");
    printf("Event added successfully.\n");
}

void updateEvent() {
    if (eventHead == NULL) {
        printf("No events to update.\n");
        logOperation("Update Event", "Failed (No events)", "Admin");
        return;
    }
    int idToUpdate;
    printf("\n--- Update Event ---\n");
    printf("Enter the ID of the event to update: ");
    scanf("%d", &idToUpdate);
    clearInputBuffer();

    EventNode *current = eventHead;
    while (current != NULL) {
        if (current->event.id == idToUpdate) {
            printf("Event found. Enter new details:\n");
            
            printf("Enter New Name: ");
            fgets(current->event.name, sizeof(current->event.name), stdin);
            current->event.name[strcspn(current->event.name, "\n")] = 0;

            printf("Enter New Description: ");
            fgets(current->event.description, sizeof(current->event.description), stdin);
            current->event.description[strcspn(current->event.description, "\n")] = 0;
            
            printf("Enter New Date (YYYY MM DD): ");
            scanf("%d %d %d", &current->event.year, &current->event.month, &current->event.day);
            clearInputBuffer();
            
            printf("Enter New Priority (High/Medium/Low): ");
            fgets(current->event.priority, sizeof(current->event.priority), stdin);
            current->event.priority[strcspn(current->event.priority, "\n")] = 0;

            printf("Is this a recurring event? (1 for Yes, 0 for No): ");
            scanf("%d", &current->event.isRecurring);
            
            printf("How many days before the event for a reminder? ");
            scanf("%d", &current->event.reminderDays);

            char details[100];
            sprintf(details, "Updated ID: %d, Name: %s", current->event.id, current->event.name);
            logOperation("Update Event", details, "Admin");
            printf("Event updated successfully.\n");
            return;
        }
        current = current->next;
    }
    printf("Event with ID %d not found.\n", idToUpdate);
    char details[50];
    sprintf(details, "Failed (ID: %d not found)", idToUpdate);
    logOperation("Update Event", details, "Admin");
}

void deleteEvent() {
    if (eventHead == NULL) {
        printf("No events to delete.\n");
        logOperation("Delete Event", "Failed (No events)", "Admin");
        return;
    }
    int idToDelete;
    printf("\n--- Delete Event ---\n");
    printf("Enter the ID of the event to delete: ");
    scanf("%d", &idToDelete);
    clearInputBuffer();

    EventNode *current = eventHead;
    EventNode *prev = NULL;

    while (current != NULL && current->event.id != idToDelete) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Event with ID %d not found.\n", idToDelete);
        char details[50];
        sprintf(details, "Failed (ID: %d not found)", idToDelete);
        logOperation("Delete Event", details, "Admin");
        return;
    }

    if (prev == NULL) {
        eventHead = current->next;
    } else {
        prev->next = current->next;
    }

    char details[100];
    sprintf(details, "Deleted ID: %d, Name: %s", current->event.id, current->event.name);
    logOperation("Delete Event", details, "Admin");
    free(current);
    printf("Event deleted successfully.\n");
}

void searchAndFilterEvents() {
    if (eventHead == NULL) {
        printf("No events to search.\n");
        logOperation("Search/Filter", "Failed (No events)", "Both");
        return;
    }
    int choice;
    printf("\n--- Search and Filter Events ---\n");
    printf("1. Search by Name\n");
    printf("2. Search by Priority\n");
    printf("3. Show all recurring events\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    clearInputBuffer();

    EventNode *current = eventHead;
    int found = 0;

    printf("\n+-----+----------------------+----------------------+-----------------+-----------------+\n");
    printf("| ID  | Name                 | Date                 | Priority        | Recurring       |\n");
    printf("+-----+----------------------+----------------------+-----------------+-----------------+\n");

    if (choice == 1) {
        char searchName[50];
        printf("Enter event name to search: ");
        fgets(searchName, sizeof(searchName), stdin);
        searchName[strcspn(searchName, "\n")] = 0;
        
        while(current != NULL) {
            if (strstr(current->event.name, searchName) != NULL) {
                displayEvent(&current->event);
                found = 1;
            }
            current = current->next;
        }
        char details[50];
        sprintf(details, "Name: %s", searchName);
        logOperation("Search", found ? details : "Failed (Not found)", "Both");
    } else if (choice == 2) {
        char searchPriority[10];
        printf("Enter priority to filter (High/Medium/Low): ");
        fgets(searchPriority, sizeof(searchPriority), stdin);
        searchPriority[strcspn(searchPriority, "\n")] = 0;

        while(current != NULL) {
            if (strcmp(current->event.priority, searchPriority) == 0) {
                displayEvent(&current->event);
                found = 1;
            }
            current = current->next;
        }
        char details[50];
        sprintf(details, "Priority: %s", searchPriority);
        logOperation("Filter", found ? details : "Failed (Not found)", "Both");
    } else if (choice == 3) {
         while(current != NULL) {
            if (current->event.isRecurring) {
                displayEvent(&current->event);
                found = 1;
            }
            current = current->next;
        }
        logOperation("Filter", found ? "Recurring events" : "Failed (No recurring events)", "Both");
    } else {
        printf("Invalid search choice.\n");
        logOperation("Search/Filter", "Failed (Invalid choice)", "Both");
    }

    printf("+-----+----------------------+----------------------+-----------------+-----------------+\n");
    if (!found) {
        printf("No matching events found.\n");
    }
}

void displayAllEvents() {
    if (eventHead == NULL) {
        printf("No events to display.\n");
        logOperation("Display", "Failed (No events)", "Both");
        return;
    }
    printf("\n--- All Events for %s ---\n", currentUsername);
    printf("+-----+----------------------+----------------------+-----------------+-----------------+\n");
    printf("| ID  | Name                 | Date                 | Priority        | Recurring       |\n");
    printf("+-----+----------------------+----------------------+-----------------+-----------------+\n");
    EventNode *current = eventHead;
    while (current != NULL) {
        displayEvent(&current->event);
        current = current->next;
    }
    printf("+-----+----------------------+----------------------+-----------------+-----------------+\n");
    logOperation("Display", "All events shown", "Both");
}

void displayEvent(const Event *e) {
    char date[20];
    sprintf(date, "%04d-%02d-%02d", e->year, e->month, e->day);
    printf("| %-3d | %-20s | %-20s | %-15s | %-15s |\n",
           e->id, e->name, date, e->priority, e->isRecurring ? "Yes" : "No");
}

void sortEventsByPriority() {
    if (eventHead == NULL || eventHead->next == NULL) {
        printf("Not enough events to sort.\n");
        logOperation("Sort", "Failed (Not enough events)", "Both");
        return;
    }
    int swapped;
    EventNode *ptr1;
    EventNode *lptr = NULL;

    do {
        swapped = 0;
        ptr1 = eventHead;

        while (ptr1->next != lptr) {
            int priority1_val = (strcmp(ptr1->event.priority, "High") == 0) ? 3 :
                                (strcmp(ptr1->event.priority, "Medium") == 0) ? 2 : 1;
            int priority2_val = (strcmp(ptr1->next->event.priority, "High") == 0) ? 3 :
                                (strcmp(ptr1->next->event.priority, "Medium") == 0) ? 2 : 1;

            if (priority1_val < priority2_val) {
                Event temp = ptr1->event;
                ptr1->event = ptr1->next->event;
                ptr1->next->event = temp;
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
    
    printf("Events sorted by priority (High to Low).\n");
    logOperation("Sort", "Events sorted by priority", "Both");
}

// --- File Handling ---

void saveEventsToFile(const char* username) {
    // char filename[FILENAME_MAX_LEN];
    // sprintf(filename, "%s_events.txt", username);
    char filename[] = "events.txt";

    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error: Could not save events to file '%s'.\n", filename);
        logOperation("Save", "Failed (File error)", "Admin");
        return;
    }

    int eventCount = 0;
    EventNode *current = eventHead;
    while(current != NULL) {
        eventCount++;
        current = current->next;
    }
    fprintf(fp,"%d\n",eventCount);

    current = eventHead;
    while(current != NULL) {
        fprintf(fp,"%d|%s|%s|%d|%d|%d|%s|%d|%d\n",
        current->event.id,
        current->event.name,
        current->event.description,
        current->event.year,
        current->event.month,
        current->event.day,
        current->event.priority,
        current->event.isRecurring,
        current->event.reminderDays);
        current = current->next;
    }
    
    fclose(fp);
    printf("Events saved to '%s' successfully.\n", filename);
    logOperation("Save", "Events saved to file", "Admin");
}

void loadEventsFromFile(const char* username) {
    freeEventList();
    
    // char filename[FILENAME_MAX_LEN];
    // sprintf(filename, "%s_events.txt", username);
    char filename[] = "events.txt";

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("No existing event file found for '%s'.\n", username);
        logOperation("Load", "Failed (File not found)", "Both");
        return;
    }

    int eventCount = 0;
    fscanf(fp,"%d\n",&eventCount);

    if (eventCount > 0) {
        Event temp;
        for(int i=0;i<eventCount;i++)
        {
            fscanf(fp,
            "%d|%49[^|]|%99[^|]|%d|%d|%d|%9[^|]|%d|%d\n",
            &temp.id,
            temp.name,
            temp.description,
            &temp.year,
            &temp.month,
            &temp.day,
            temp.priority,
            &temp.isRecurring,
            &temp.reminderDays);

            EventNode *newNode = malloc(sizeof(EventNode));

            newNode->event = temp;
            newNode->next = eventHead;
            eventHead = newNode;
        }
        printf("Events loaded from '%s' successfully.\n", filename);
        logOperation("Load", "Events loaded from file", "Both");
    } else {
        printf("No events found in file '%s'.\n", filename);
    }
    
    fclose(fp);
}

void freeEventList() {
    EventNode *current = eventHead;
    EventNode *next;
    while(current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    eventHead = NULL;
}

// --- Utility Functions ---

void checkReminders() {
    if (eventHead == NULL) {
        printf("No events to check for reminders.\n");
        logOperation("Reminders", "Failed (No events)", "Both");
        return;
    }

    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    int currentDay = local->tm_mday;
    int currentMonth = local->tm_mon + 1;
    int currentYear = local->tm_year + 1900;

    printf("\n--- Upcoming Event Reminders ---\n");
    EventNode *current = eventHead;
    int found = 0;

    while(current != NULL) {
        int daysUntilEvent = (current->event.day - currentDay) +
                             (current->event.month - currentMonth) * 30 +
                             (current->event.year - currentYear) * 365;
        
        if (daysUntilEvent > 0 && daysUntilEvent <= current->event.reminderDays) {
            printf("REMINDER: Event '%s' is in %d day(s).\n", current->event.name, daysUntilEvent);
            found = 1;
        }
        current = current->next;
    }
    
    if (!found) {
        printf("No upcoming reminders found.\n");
    }
    logOperation("Reminders", found ? "Checked for reminders" : "No reminders found", "Both");
}

void displayHistory() {
    printf("\n\n+-----------------------------------------------------------------------------------------+\n");
    printf("|                                  History of Operations                                  |\n");
    printf("+----------------------+---------------------------------+-----------+----------------------+\n");
    printf("| Operation            | Details                         | Mode      | Timestamp            |\n");
    printf("+----------------------+---------------------------------+-----------+----------------------+\n");
    if (history_count == 0) {
        printf("| %-87s |\n", "No operations recorded yet.");
    } else {
        for (int i = 0; i < history_count; i++) {
            printf("| %-20s | %-31s | %-9s | %-20s |\n",
                   history[i].operation, history[i].details, history[i].userMode, history[i].timestamp);
        }
    }
    printf("+----------------------+---------------------------------+-----------+----------------------+\n\n");
}

void logOperation(const char *op, const char *details, const char *mode) {
    if (history_count < MAX_HISTORY) {
        time_t now = time(NULL);
        struct tm *local = localtime(&now);
        char timestamp[30];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", local);

        strcpy(history[history_count].operation, op);
        strcpy(history[history_count].details, details);
        strcpy(history[history_count].userMode, mode);
        strcpy(history[history_count].timestamp, timestamp);
        history_count++;
    } else {
        printf("History log is full.\n");
    }
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
