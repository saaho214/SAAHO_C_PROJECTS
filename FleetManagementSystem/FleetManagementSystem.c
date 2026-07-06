#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define maximum capacities for arrays
#define MAX_VEHICLES 50
#define MAX_USERS 50
#define MAX_MAINTENANCE_RECORDS 200

// Define structs for data storage
typedef struct {
    int vehicleID;
    char licensePlate[15];
    char model[50];
    int year;
    int currentMileage;
} Vehicle;

typedef struct {
    int recordID;
    int vehicleID;
    char maintenanceType[50];
    char maintenanceDate[20];
    float cost;
} MaintenanceRecord;

typedef struct {
    char username[20];
    char password[20];
    char role[10];
} User;

// Global arrays and counters
Vehicle vehicles[MAX_VEHICLES];
int vehicleCount = 0;

MaintenanceRecord maintenanceRecords[MAX_MAINTENANCE_RECORDS];
int maintenanceCount = 0;

User users[MAX_USERS];
int userCount = 0;

// File names for data persistence
const char* VEHICLE_FILE = "vehicles.txt";
const char* MAINTENANCE_FILE = "maintenance.txt";
const char* USER_FILE = "users.txt";

// Function prototypes
void loadData();
void saveData();
void displayMainMenu();
void adminMenu();
void userMenu(const char* loggedInUsername);

// Authentication Functions
void registerUser();
int loginUser(char* loggedInUsername);

// Admin Functions
void addVehicle();
void updateVehicle();
void deleteVehicle();
void addMaintenanceRecord();
void displayAllVehicles();
void displayAllMaintenanceRecords();
void displayAllUsers();

// User Functions
void searchVehicle();
void viewVehicleDetails(const char* licensePlate);

// Utility Functions
int generateVehicleID();
int generateMaintenanceID();
void printVehicleTable(const int* indices, int count);
void printMaintenanceTable(const int* indices, int count);
void printUserTable();

int main() {
    loadData();
    int choice;
    char loggedInUsername[20];
    
    while(1) {
        displayMainMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        if (choice == 1) {
            int loginResult = loginUser(loggedInUsername);
            if (loginResult == 1) { // Admin login
                adminMenu();
            } else if (loginResult == 2) { // User login
                userMenu(loggedInUsername);
            } else {
                printf("Login failed. Please try again.\n");
            }
        } else if (choice == 2) {
            registerUser();
        } else if (choice == 3) {
            saveData();
            printf("Data saved. Exiting program...\n");
            break;
        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }
    
    return 0;
}

// ======================= AUTHENTICATION =======================

/**
 * @brief Displays the main menu for login, registration, and exit.
 */
void displayMainMenu() {
    printf("\n--- Fleet Management System ---\n");
    printf("1. Login\n");
    printf("2. Register\n");
    printf("3. Exit\n");
}

/**
 * @brief Registers a new user account and saves it to the user file.
 */
void registerUser() {
    if (userCount >= MAX_USERS) {
        printf("Cannot register, maximum user limit reached.\n");
        return;
    }
    
    User newUser;
    int roleChoice;
    
    printf("\n--- User Registration ---\n");
    printf("Enter username: ");
    scanf(" %s", newUser.username);
    
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, newUser.username) == 0) {
            printf("Username already exists. Please choose a different one.\n");
            return;
        }
    }
    
    printf("Enter password: ");
    scanf(" %s", newUser.password);
    printf("Select role (1 for Admin, 2 for User): ");
    scanf("%d", &roleChoice);
    
    if (roleChoice == 1) {
        strcpy(newUser.role, "admin");
    } else {
        strcpy(newUser.role, "user");
    }
    
    users[userCount] = newUser;
    userCount++;
    printf("Registration successful!\n");
    saveData();
}

/**
 * @brief Handles user login and authenticates against the user file.
 * @return 1 for Admin, 2 for User, 0 for invalid credentials.
 */
int loginUser(char* loggedInUsername) {
    char username[20], password[20];
    printf("\n--- Login ---\n");
    printf("Enter username: ");
    scanf(" %s", username);
    printf("Enter password: ");
    scanf(" %s", password);
    
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            strcpy(loggedInUsername, username);
            if (strcmp(users[i].role, "admin") == 0) {
                printf("Login successful as Admin.\n");
                return 1; // Admin
            } else {
                printf("Login successful as User.\n");
                return 2; // User
            }
        }
    }
    return 0; // Failed
}

// ======================= ADMIN FUNCTIONS =======================

/**
 * @brief Displays and handles the admin-specific menu.
 */
void adminMenu() {
    int choice;
    do {
        printf("\n--- Admin Menu ---\n");
        printf("1. Add a new vehicle\n");
        printf("2. Update an existing vehicle\n");
        printf("3. Delete a vehicle\n");
        printf("4. Add maintenance record\n");
        printf("5. Display all vehicles\n");
        printf("6. Display all maintenance records\n");
        printf("7. Display all users\n");
        printf("8. Log out\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: addVehicle(); break;
            case 2: updateVehicle(); break;
            case 3: deleteVehicle(); break;
            case 4: addMaintenanceRecord(); break;
            case 5: displayAllVehicles(); break;
            case 6: displayAllMaintenanceRecords(); break;
            case 7: displayAllUsers(); break;
            case 8: printf("Logging out...\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 8);
}

/**
 * @brief Adds a new vehicle record to the array.
 */
void addVehicle() {
    if (vehicleCount >= MAX_VEHICLES) {
        printf("Cannot add more vehicles, limit reached.\n");
        return;
    }
    
    printf("\n--- Add New Vehicle ---\n");
    vehicles[vehicleCount].vehicleID = generateVehicleID();
    printf("Enter License Plate: ");
    scanf(" %s", vehicles[vehicleCount].licensePlate);
    printf("Enter Model: ");
    scanf(" %s", vehicles[vehicleCount].model);
    printf("Enter Year: ");
    scanf("%d", &vehicles[vehicleCount].year);
    printf("Enter Current Mileage: ");
    scanf("%d", &vehicles[vehicleCount].currentMileage);
    
    vehicleCount++;
    printf("Vehicle added successfully! ID: %d\n", vehicles[vehicleCount-1].vehicleID);
    saveData();
}

/**
 * @brief Updates a vehicle's mileage based on its license plate.
 */
void updateVehicle() {
    char plate[15];
    printf("\n--- Update Vehicle ---\n");
    printf("Enter License Plate to update: ");
    scanf(" %s", plate);
    
    for (int i = 0; i < vehicleCount; i++) {
        if (strcmp(vehicles[i].licensePlate, plate) == 0) {
            printf("Enter new Current Mileage: ");
            scanf("%d", &vehicles[i].currentMileage);
            printf("Vehicle updated successfully!\n");
            saveData();
            return;
        }
    }
    printf("Vehicle not found.\n");
}

/**
 * @brief Deletes a vehicle record from the array.
 */
void deleteVehicle() {
    char plate[15];
    printf("\n--- Delete Vehicle ---\n");
    printf("Enter License Plate to delete: ");
    scanf(" %s", plate);
    
    int foundIndex = -1;
    for (int i = 0; i < vehicleCount; i++) {
        if (strcmp(vehicles[i].licensePlate, plate) == 0) {
            foundIndex = i;
            break;
        }
    }
    
    if (foundIndex != -1) {
        // Shift elements to fill the gap
        for (int i = foundIndex; i < vehicleCount - 1; i++) {
            vehicles[i] = vehicles[i+1];
        }
        vehicleCount--;
        printf("Vehicle deleted successfully.\n");
        saveData();
    } else {
        printf("Vehicle not found.\n");
    }
}

/**
 * @brief Adds a new maintenance record to the array.
 */
void addMaintenanceRecord() {
    if (maintenanceCount >= MAX_MAINTENANCE_RECORDS) {
        printf("Cannot add more maintenance records, limit reached.\n");
        return;
    }

    int vehicleID;
    printf("\n--- Add Maintenance Record ---\n");
    printf("Enter Vehicle ID for the record: ");
    scanf("%d", &vehicleID);

    int found = 0;
    for(int i = 0; i < vehicleCount; i++) {
        if(vehicles[i].vehicleID == vehicleID) {
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Vehicle with ID %d not found. Record not added.\n", vehicleID);
        return;
    }

    maintenanceRecords[maintenanceCount].recordID = generateMaintenanceID();
    maintenanceRecords[maintenanceCount].vehicleID = vehicleID;
    printf("Enter Maintenance Type (e.g., OilChange, TireRotation): ");
    scanf(" %s", maintenanceRecords[maintenanceCount].maintenanceType);
    printf("Enter Date (e.g., YYYY-MM-DD): ");
    scanf(" %s", maintenanceRecords[maintenanceCount].maintenanceDate);
    printf("Enter Cost: ");
    scanf("%f", &maintenanceRecords[maintenanceCount].cost);
    
    maintenanceCount++;
    printf("Maintenance record added successfully!\n");
    saveData();
}

/**
 * @brief Displays all vehicle records in a tabular format.
 */
void displayAllVehicles() {
    if (vehicleCount == 0) {
        printf("No vehicles to display.\n");
        return;
    }
    int indices[MAX_VEHICLES];
    for(int i = 0; i < vehicleCount; i++) {
        indices[i] = i;
    }
    printf("\n--- All Vehicles ---\n");
    printVehicleTable(indices, vehicleCount);
}

/**
 * @brief Displays all maintenance records in a tabular format.
 */
void displayAllMaintenanceRecords() {
    if (maintenanceCount == 0) {
        printf("No maintenance records to display.\n");
        return;
    }
    int indices[MAX_MAINTENANCE_RECORDS];
    for(int i = 0; i < maintenanceCount; i++) {
        indices[i] = i;
    }
    printf("\n--- All Maintenance Records ---\n");
    printMaintenanceTable(indices, maintenanceCount);
}

/**
 * @brief Displays all user accounts in a tabular format.
 */
void displayAllUsers() {
    if (userCount == 0) {
        printf("No users to display.\n");
        return;
    }
    printf("\n--- All System Users ---\n");
    printUserTable();
}

// ======================= USER FUNCTIONS =======================

/**
 * @brief Displays and handles the user-specific menu.
 */
void userMenu(const char* loggedInUsername) {
    int choice;
    do {
        printf("\n--- User Menu ---\n");
        printf("1. display all vehicle\n");
        printf("2. Search for a vehicle\n");
        printf("3. View a vehicle's full details\n");
        printf("4. Log out\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        if (choice == 1) {
            displayAllVehicles();
        } else if (choice == 2) {
            searchVehicle();
        } else if (choice == 3) {
            char plate[15];
            printf("Enter License Plate to view details: ");
            scanf(" %s", plate);
            viewVehicleDetails(plate);
        } else if (choice == 4) {
            printf("Logging out...\n");
        } else {
            printf("Invalid choice.\n");
        }
    } while (choice != 4);
}

/**
 * @brief Searches for a vehicle by license plate and displays basic info.
 */
void searchVehicle() {
    char plate[15];
    printf("\n--- Search Vehicle ---\n");
    printf("Enter License Plate: ");
    scanf(" %s", plate);
    
    int foundIndex = -1;
    for (int i = 0; i < vehicleCount; i++) {
        if (strcmp(vehicles[i].licensePlate, plate) == 0) {
            foundIndex = i;
            break;
        }
    }
    
    if (foundIndex != -1) {
        printf("Vehicle found!\n");
        int indices[1] = {foundIndex};
        printVehicleTable(indices, 1);
    } else {
        printf("Vehicle not found.\n");
    }
}

/**
 * @brief Displays a vehicle's full details, including its maintenance history.
 */
void viewVehicleDetails(const char* licensePlate) {
    int vehicleID = -1;
    char foundLicensePlate[15];

    // Find the vehicle's ID
    for (int i = 0; i < vehicleCount; i++) {
        if (strcmp(vehicles[i].licensePlate, licensePlate) == 0) {
            vehicleID = vehicles[i].vehicleID;
            break;
        }
    }
    
    if (vehicleID == -1) {
        printf("Vehicle with license plate '%s' not found.\n", licensePlate);
        return;
    }
    
    printf("\n--- Vehicle Details for %s ---\n", licensePlate);
    int indices[1];
    for(int i = 0; i < vehicleCount; i++) {
        if(vehicles[i].vehicleID == vehicleID) {
            indices[0] = i;
            printVehicleTable(indices, 1);
            break;
        }
    }

    // Find and display maintenance history for this vehicle
    int maintenanceIndices[MAX_MAINTENANCE_RECORDS];
    int maintenanceFoundCount = 0;
    
    for (int i = 0; i < maintenanceCount; i++) {
        if (maintenanceRecords[i].vehicleID == vehicleID) {
            maintenanceIndices[maintenanceFoundCount] = i;
            maintenanceFoundCount++;
        }
    }

    if (maintenanceFoundCount > 0) {
        printf("\n--- Maintenance History ---\n");
        printMaintenanceTable(maintenanceIndices, maintenanceFoundCount);
    } else {
        printf("No maintenance records found for this vehicle.\n");
    }
}

// ======================= DATA PERSISTENCE =======================

/**
 * @brief Loads all data from files into the global arrays.
 */
void loadData() {
    FILE* fptr;

    // Load vehicles
    fptr = fopen(VEHICLE_FILE, "r");
    if (fptr != NULL) {
        while (fscanf(fptr, "%d %s %s %d %d\n", 
                &vehicles[vehicleCount].vehicleID, vehicles[vehicleCount].licensePlate, 
                vehicles[vehicleCount].model, &vehicles[vehicleCount].year, 
                &vehicles[vehicleCount].currentMileage) == 5) {
            vehicleCount++;
        }
        fclose(fptr);
    }
    
    // Load maintenance records
    fptr = fopen(MAINTENANCE_FILE, "r");
    if (fptr != NULL) {
        while (fscanf(fptr, "%d %d %s %s %f\n",
                &maintenanceRecords[maintenanceCount].recordID, 
                &maintenanceRecords[maintenanceCount].vehicleID, 
                maintenanceRecords[maintenanceCount].maintenanceType,
                maintenanceRecords[maintenanceCount].maintenanceDate,
                &maintenanceRecords[maintenanceCount].cost) == 5) {
            maintenanceCount++;
        }
        fclose(fptr);
    }
    
    // Load users
    fptr = fopen(USER_FILE, "r");
    if (fptr != NULL) {
        while (fscanf(fptr, "%s %s %s\n", users[userCount].username, 
                users[userCount].password, users[userCount].role) == 3) {
            userCount++;
        }
        fclose(fptr);
    }
}

/**
 * @brief Saves all data from global arrays to files.
 */
void saveData() {
    FILE* fptr;

    // Save vehicles
    fptr = fopen(VEHICLE_FILE, "w");
    if (fptr != NULL) {
        for (int i = 0; i < vehicleCount; i++) {
            fprintf(fptr, "%d %s %s %d %d\n", 
                    vehicles[i].vehicleID, vehicles[i].licensePlate, 
                    vehicles[i].model, vehicles[i].year, vehicles[i].currentMileage);
        }
        fclose(fptr);
    }
    
    // Save maintenance records
    fptr = fopen(MAINTENANCE_FILE, "w");
    if (fptr != NULL) {
        for (int i = 0; i < maintenanceCount; i++) {
            fprintf(fptr, "%d %d %s %s %.2f\n",
                    maintenanceRecords[i].recordID, 
                    maintenanceRecords[i].vehicleID, 
                    maintenanceRecords[i].maintenanceType,
                    maintenanceRecords[i].maintenanceDate,
                    maintenanceRecords[i].cost);
        }
        fclose(fptr);
    }

    // Save users
    fptr = fopen(USER_FILE, "w");
    if (fptr != NULL) {
        for (int i = 0; i < userCount; i++) {
            fprintf(fptr, "%s %s %s\n", users[i].username, users[i].password, users[i].role);
        }
        fclose(fptr);
    }
}

// ======================= UTILITY FUNCTIONS =======================

/**
 * @brief Generates a new unique ID for a vehicle.
 * @return A new unique vehicle ID.
 */
int generateVehicleID() {
    if (vehicleCount == 0) return 101;
    return vehicles[vehicleCount-1].vehicleID + 1;
}

/**
 * @brief Generates a new unique ID for a maintenance record.
 * @return A new unique maintenance record ID.
 */
int generateMaintenanceID() {
    if (maintenanceCount == 0) return 1001;
    return maintenanceRecords[maintenanceCount-1].recordID + 1;
}

/**
 * @brief Prints a table of vehicle records.
 */
void printVehicleTable(const int* indices, int count) {
    printf("+------------+---------------+--------------------+--------+---------+\n");
    printf("| Vehicle ID | License Plate |       Model        |  Year  | Mileage |\n");
    printf("+------------+---------------+--------------------+--------+---------+\n");
    for (int i = 0; i < count; i++) {
        int idx = indices[i];
        printf("| %-10d | %-13s | %-18s | %-6d | %-7d |\n",
               vehicles[idx].vehicleID, vehicles[idx].licensePlate, vehicles[idx].model,
               vehicles[idx].year, vehicles[idx].currentMileage);
    }
    printf("+------------+---------------+--------------------+--------+---------+\n");
}

/**
 * @brief Prints a table of maintenance records.
 */
void printMaintenanceTable(const int* indices, int count) {
    printf("+------------+---------------+--------------------+------------+---------+\n");
    printf("| Record ID  | Vehicle ID    |  Maintenance Type  |    Date    |   Cost  |\n");
    printf("+------------+---------------+--------------------+------------+---------+\n");
    for (int i = 0; i < count; i++) {
        int idx = indices[i];
        printf("| %-10d | %-13d | %-18s | %-10s | %-7.2f |\n",
               maintenanceRecords[idx].recordID, maintenanceRecords[idx].vehicleID,
               maintenanceRecords[idx].maintenanceType, maintenanceRecords[idx].maintenanceDate,
               maintenanceRecords[idx].cost);
    }
    printf("+------------+---------------+--------------------+------------+---------+\n");
}

/**
 * @brief Prints a table of user records.
 */
void printUserTable() {
    printf("+--------------------+------------+\n");
    printf("|      Username      |    Role    |\n");
    printf("+--------------------+------------+\n");
    for (int i = 0; i < userCount; i++) {
        printf("| %-18s | %-10s |\n", users[i].username, users[i].role);
    }
    printf("+--------------------+------------+\n");
}
