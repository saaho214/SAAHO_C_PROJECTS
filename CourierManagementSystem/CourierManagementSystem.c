#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the Courier struct for the linked list node
typedef struct Courier {
    int trackingID;
    char senderName[50];
    char senderAddress[50];
    char receiverName[50];
    char receiverAddress[50];
    char status[20];
    struct Courier* next;
} Courier;

// Define a User struct to store credentials and role
typedef struct User {
    char username[20];
    char password[20];
    char role[10]; // "admin" or "user"
} User;

// Enum to define user roles
typedef enum {
    ADMIN,
    USER,
    INVALID
} UserRole;

// Function prototypes for Courier Management
void addCourier(Courier** head);
void trackCourier(const char* filename);
void updateStatus(const char* filename);
void deleteCourier(const char* filename);
void displayAllCouriers(const char* filename);
void saveCourierData(Courier* head, const char* filename);
void loadCourierData(Courier** head, const char* filename);
void freeCourierList(Courier** head);

// Function prototypes for User Management
UserRole login();
void registerUser();

// Global file names for data persistence
const char* COURIER_DATA_FILE = "courier_data.txt";
const char* USER_DATA_FILE = "user_credentials.txt";

int main() {
    Courier* head = NULL;
    int choice;
    UserRole userRole;
    int auth_choice;

    loadCourierData(&head, COURIER_DATA_FILE);

    // Initial authentication loop
    do {
        printf("\n--- Welcome to the Courier System ---\n");
        printf("1. Login\n");
        printf("2. Register\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &auth_choice);

        switch (auth_choice) {
            case 1:
                userRole = login();
                if (userRole != INVALID) {
                    // Break the loop if login is successful
                    goto logged_in;
                }
                break;
            case 2:
                registerUser();
                break;
            case 3:
                printf("Exiting the application.\n");
                freeCourierList(&head);
                return 0; // Exit program
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (1);

logged_in:; // Label to jump to after successful login

    // Main menu loop based on user role
    do {
        if (userRole == ADMIN) {
            // Admin menu with all functionalities
            printf("\n--- Admin Menu ---\n");
            printf("1. Add New Courier\n");
            printf("2. Track Courier\n");
            printf("3. Update Courier Status\n");
            printf("4. Delete Courier\n");
            printf("5. Display All Couriers\n");
            printf("6. Save & Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    addCourier(&head);
                    break;
                case 2:
                    trackCourier(COURIER_DATA_FILE);
                    break;
                case 3:
                    updateStatus(COURIER_DATA_FILE);
                    break;
                case 4:
                    deleteCourier(COURIER_DATA_FILE);
                    break;
                case 5:
                    displayAllCouriers(COURIER_DATA_FILE);
                    break;
                case 6:
                    // Save data to the file before exiting
                    saveCourierData(head, COURIER_DATA_FILE);
                    printf("Data saved successfully. Exiting...\n");
                    break;
                default:
                    printf("Invalid choice. Please try again.\n");
            }
        } else { // userRole == USER
            // User menu with limited functionalities
            printf("\n--- User Menu ---\n");
             printf("1. Add New Courier\n");
            printf("2. Track Courier\n");
            printf("3. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    addCourier(&head);
                    break;
                case 2:
                    trackCourier(COURIER_DATA_FILE);
                    break;
                case 3:
                saveCourierData(head, COURIER_DATA_FILE);
                    printf("Exiting...\n");
                    break;
                default:
                    printf("Invalid choice. Please try again.\n");
            }
        }
    } while ((userRole == ADMIN && choice != 6) || (userRole == USER && choice != 3));
    freeCourierList(&head);
    return 0;
}

/**
 * @brief Registers a new user account and saves credentials to a file.
 */
void registerUser() {
    User newUser;
    int role_choice;

    printf("\n--- New User Registration ---\n");
    printf("Enter a new username: ");
    scanf(" %s", newUser.username);
    printf("Enter a password: ");
    scanf(" %s", newUser.password);
    printf("Select Role:\n");
    printf("1. Admin\n");
    printf("2. User\n");
    printf("Enter your choice: ");
    scanf("%d", &role_choice);

    if (role_choice == 1) {
        strcpy(newUser.role, "admin");
    } else {
        strcpy(newUser.role, "user");
    }

    FILE* file = fopen(USER_DATA_FILE, "a");
    if (file == NULL) {
        printf("Error opening user credentials file for writing.\n");
        return;
    }

    fprintf(file, "%s %s %s\n", newUser.username, newUser.password, newUser.role);
    fclose(file);
    printf("Registration successful! You can now log in.\n");
}

/**
 * @brief Handles user login by checking credentials against a file.
 */
UserRole login() {
    char username[20];
    char password[20];
    char stored_username[20], stored_password[20], stored_role[10];

    printf("\n--- Login ---\n");
    printf("Username: ");
    scanf(" %s", username);
    printf("Password: ");
    scanf(" %s", password);

    FILE* file = fopen(USER_DATA_FILE, "r");
    if (file == NULL) {
        printf("Error: No user credentials found. Please register first.\n");
        return INVALID;
    }

    // Read credentials from the file and compare
    while (fscanf(file, "%s %s %s", stored_username, stored_password, stored_role) != EOF) {
        if (strcmp(username, stored_username) == 0 && strcmp(password, stored_password) == 0) {
            fclose(file);
            printf("Login successful.\n");
            if (strcmp(stored_role, "admin") == 0) {
                return ADMIN;
            } else {
                return USER;
            }
        }
    }

    fclose(file);
    printf("Invalid username or password.\n");
    return INVALID;
}

/**
 * @brief Adds a new courier node to the beginning of the linked list.
 */
void addCourier(Courier** head) {
    // Allocate memory for the new node
    Courier* newCourier = (Courier*)malloc(sizeof(Courier));
    if (newCourier == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    // Get courier details from the user
    printf("Enter Tracking ID: ");
    scanf("%d", &newCourier->trackingID);
    printf("Enter Sender's Name: ");
    scanf(" %[^\n]s", newCourier->senderName);
    printf("Enter Sender's Address: ");
    scanf(" %[^\n]s", newCourier->senderAddress);
    printf("Enter Receiver's Name: ");
    scanf(" %[^\n]s", newCourier->receiverName);
    printf("Enter Receiver's Address: ");
    scanf(" %[^\n]s", newCourier->receiverAddress);
    
    // Set initial status and link the new node
    strcpy(newCourier->status, "Booked");
    printf("Courier added with status: %s\n", newCourier->status);

    newCourier->next = *head;
    *head = newCourier;
}

/**
 * @brief Searches for and displays a courier's details by tracking ID.
 */
void trackCourier(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open data file.\n");
        return;
    }

    int searchID;
    printf("Enter Tracking ID to track: ");
    scanf("%d", &searchID);

    int id;
    char sender[50], address1[50], receiver[50], address2[50], status[20];
    int found = 0;

    while (fscanf(file, "%d | %[^|] | %[^|] | %[^|] | %[^|] | %[^\n]\n", 
              &id, sender, address1, receiver, address2, status) == 6) {
        if (id == searchID) {
            printf("\n--- Courier Details (From File) ---\n");
            printf("Tracking ID: %d\n", id);
            printf("Sender: %s\n", sender);
            printf("Address1 (Sender): %s\n", address1); 
            printf("Receiver: %s\n", receiver);
            printf("Address2 (Receiver): %s\n", address2);
            printf("Status: %s\n", status);
            found = 1;
            break; // Stop scanning once found
        }
    }

    if (!found) {
        printf("Courier with ID %d not found in file.\n", searchID);
    }
    fclose(file);
}

/**
 * @brief Updates the status of an existing courier.
 */
void updateStatus(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open data file.\n");
        return;
    }

    int targetID;
    printf("Enter Tracking ID to update status: ");
    scanf("%d", &targetID);

    FILE* tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Error creating temporary file.\n");
        fclose(file);
        return;
    }

    int id;
    char sender[50], address1[50], receiver[50], address2[50], status[20];
    int found = 0;

    while (fscanf(file, "%d | %[^|] | %[^|] | %[^|] | %[^|] | %[^\n]\n", 
              &id, sender, address1, receiver, address2, status) == 6) {
        if (id == targetID) {
            found = 1;
            printf("Current Status: %s\n", status);
            printf("Enter new status: ");
            scanf(" %[^\n]", status); // Read the updated status string
        }
        // Write record (modified or clean) to temp file
        fprintf(tempFile, "%d\n%s\n%s\n%s\n%s\n%s\n", 
                id, sender, address1, receiver, address2, status);
    }

    fclose(file);
    fclose(tempFile);

    if (found) {
        remove(filename);
        rename("temp.txt", filename);
        printf("Status updated directly inside file.\n");
    } else {
        remove("temp.txt");
        printf("Courier with ID %d not found.\n", targetID);
    }
}

/**
 * @brief Deletes a courier from the linked list by ID.
 */
void deleteCourier(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open data file.\n");
        return;
    }

    int targetID;
    printf("Enter Tracking ID to delete from file: ");
    scanf("%d", &targetID);

    // Create a temporary file to hold everything we want to keep
    FILE* tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Error creating temporary file.\n");
        fclose(file);
        return;
    }

    int id;
    char sender[50], address1[50], receiver[50], address2[50], status[20];
    int found = 0;

    while (fscanf(file, "%d | %[^|] | %[^|] | %[^|] | %[^|] | %[^\n]\n", 
              &id, sender, address1, receiver, address2, status) == 6) {
        if (id == targetID) {
            found = 1; // Skip writing this entry to target it for deletion
            continue;
        }
        // Write kept records to temporary storage
        fprintf(tempFile, "%d\n%s\n%s\n%s\n%s\n%s\n", 
                id, sender, address1, receiver, address2, status);
    }

    fclose(file);
    fclose(tempFile);

    if (found) {
        remove(filename);          // Delete old database file
        rename("temp.txt", filename); // Swap temporary file to main database
        printf("Courier with ID %d deleted directly from file.\n", targetID);
    } else {
        remove("temp.txt"); // Clean up temporary file if nothing changed
        printf("Courier with ID %d not found.\n", targetID);
    }
}


/**
 * @brief Displays all courier records directly from the file without using a linked list.
 */
void displayAllCouriers(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("No existing data file found.\n");
        return;
    }

    int id;
    char sender[50], address1[50], receiver[50], address2[50], status[20];
    int recordCount = 0;

    printf("\n--- All Courier Records (Direct from File) ---\n");

    while (fscanf(file, "%d | %[^|] | %[^|] | %[^|] | %[^|] | %[^\n]\n", 
              &id, sender, address1, receiver, address2, status) == 6) {
        
        printf("ID: %d | Sender: %s | Sender Addr: %s | Receiver: %s | Receiver Addr: %s | Status: %s\n",
               id, sender, address1, receiver, address2, status);
        
        recordCount++;
    }

    if (recordCount == 0) {
        printf("No courier records found in file.\n");
    } else {
        printf("----------------------------------------------\n");
        printf("Total Records Found: %d\n", recordCount);
    }

    fclose(file);
}

/**
 * @brief Saves the entire linked list to a file for persistence.
 */
void saveCourierData(Courier* head, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    Courier* current = head;
    while (current != NULL) {
        // FIXED: Replaced duplicate senderAddress with receiverAddress
        fprintf(file, "%d | %s | %s | %s | %s | %s\n",
                current->trackingID, current->senderName, current->senderAddress, 
                current->receiverName, current->receiverAddress, current->status);
        current = current->next;
    }
    fclose(file);
}

/**
 * @brief Loads courier data from a file into a new linked list.
 */
void loadCourierData(Courier** head, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("No existing data file found. Starting with an empty list.\n");
        return;
    }

    freeCourierList(head); 

    int id;
    char sender[50], Address1[50], receiver[50], Address2[50], status[20];
    
    while (fscanf(file, "%d | %[^|] | %[^|] | %[^|] | %[^|] | %[^\n]\n", 
                  &id, sender, Address1, receiver, Address2, status) == 6) {
        
        Courier* newCourier = (Courier*)malloc(sizeof(Courier));
        if (newCourier == NULL) {
            printf("Memory allocation failed during loading!\n");
            break;
        }
        
        newCourier->trackingID = id;
        strcpy(newCourier->senderName, sender);
        strcpy(newCourier->senderAddress, Address1);
        strcpy(newCourier->receiverName, receiver);
        strcpy(newCourier->receiverAddress, Address2);
        strcpy(newCourier->status, status);
        
        newCourier->next = *head;
        *head = newCourier;
    }
    fclose(file);
}

/**
 * @brief Frees all dynamically allocated memory for the linked list.
 */
void freeCourierList(Courier** head) {
    Courier* current = *head;
    Courier* next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    *head = NULL;
}
