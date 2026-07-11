#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define maximum capacities for arrays
#define MAX_FLIGHTS 100
#define MAX_USERS 50
#define MAX_RESERVATIONS 500

// Define structs for data storage
typedef struct {
    int flightNumber;
    char origin[50];
    char destination[50];
    char departureTime[20];
    int totalSeats;
    int availableSeats;
} Flight;

typedef struct {
    char username[20];
    char password[20];
    char role[10];
} User;

typedef struct {
    int reservationID;
    int flightNumber;
    char passengerName[50];
} Reservation;

// Global arrays and counters
Flight flights[MAX_FLIGHTS];
int flightCount = 0;

User users[MAX_USERS];
int userCount = 0;

Reservation reservations[MAX_RESERVATIONS];
int reservationCount = 0;

// File names for data persistence
const char* FLIGHT_FILE = "flights.txt";
const char* USER_FILE = "users.txt";
const char* RESERVATION_FILE = "reservations.txt";

// Function prototypes
void loadData();
void saveData();
void displayMainMenu();
void adminMenu();
void userMenu();

// Authentication Functions
void registerUser();
int loginUser(char* loggedInUsername);

// Admin Functions
void addFlight();
void updateFlight();
void deleteFlight();
void displayAllFlights();
void displayAllReservations();
void displayAllUsers(); // New function prototype

// User Functions
void searchFlights();
void bookReservation(const char* username);
void cancelReservation(const char* username);
void displayMyReservations(const char* username);

// Utility Functions
int generateReservationID();
void printFlightTable(int count, const int* indices);
void printReservationTable(int count, const int* indices);
void printUserTable(); // New function prototype

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

void displayMainMenu() {
    printf("\n--- Flight Reservation System ---\n");
    printf("1. Login\n");
    printf("2. Register\n");
    printf("3. Exit\n");
}

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
                return 1;
            } else {
                printf("Login successful as User.\n");
                return 2;
            }
        }
    }
    return 0; // Failed
}

// ======================= ADMIN FUNCTIONS =======================

void adminMenu() {
    int choice;
    do {
        printf("\n--- Admin Menu ---\n");
        printf("1. Add a new flight\n");
        printf("2. Update an existing flight\n");
        printf("3. Delete a flight\n");
        printf("4. Display all flights\n");
        printf("5. Display all reservations\n");
        printf("6. Display all users\n"); // New menu option
        printf("7. Log out\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: addFlight(); break;
            case 2: updateFlight(); break;
            case 3: deleteFlight(); break;
            case 4: displayAllFlights(); break;
            case 5: displayAllReservations(); break;
            case 6: displayAllUsers(); break; // Call the new function
            case 7: printf("Logging out...\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 7);
}

void addFlight() {
    if (flightCount >= MAX_FLIGHTS) {
        printf("Cannot add more flights, limit reached.\n");
        return;
    }
    
    printf("\n--- Add New Flight ---\n");
    printf("Enter Flight Number: ");
    scanf("%d", &flights[flightCount].flightNumber);
    printf("Enter Origin: ");
    scanf(" %s", flights[flightCount].origin);
    printf("Enter Destination: ");
    scanf(" %s", flights[flightCount].destination);
    printf("Enter Departure Time (e.g., 14:30): ");
    scanf(" %s", flights[flightCount].departureTime);
    printf("Enter Total Seats: ");
    scanf("%d", &flights[flightCount].totalSeats);
    
    flights[flightCount].availableSeats = flights[flightCount].totalSeats;
    flightCount++;
    printf("Flight added successfully!\n");
    saveData();
}

void updateFlight() {
    int flightNum;
    printf("\n--- Update Flight ---\n");
    printf("Enter Flight Number to update: ");
    scanf("%d", &flightNum);
    
    for (int i = 0; i < flightCount; i++) {
        if (flights[i].flightNumber == flightNum) {
            printf("Enter new Departure Time (e.g., 14:30): ");
            scanf(" %s", flights[i].departureTime);
            printf("Flight updated successfully!\n");
            saveData();
            return;
        }
    }
    printf("Flight not found.\n");
}

void deleteFlight() {
    int flightNum;
    printf("\n--- Delete Flight ---\n");
    printf("Enter Flight Number to delete: ");
    scanf("%d", &flightNum);
    
    int foundIndex = -1;
    for (int i = 0; i < flightCount; i++) {
        if (flights[i].flightNumber == flightNum) {
            foundIndex = i;
            break;
        }
    }
    
    if (foundIndex != -1) {
        // Shift elements to fill the gap
        for (int i = foundIndex; i < flightCount - 1; i++) {
            flights[i] = flights[i+1];
        }
        flightCount--;
        printf("Flight deleted successfully.\n");
        saveData();
    } else {
        printf("Flight not found.\n");
    }
}

void displayAllFlights() {
    if (flightCount == 0) {
        printf("No flights to display.\n");
        return;
    }
    int indices[MAX_FLIGHTS];
    for(int i = 0; i < flightCount; i++) {
        indices[i] = i;
    }
    printf("\n--- All Flights ---\n");
    printFlightTable(flightCount, indices);
}

void displayAllReservations() {
    if (reservationCount == 0) {
        printf("No reservations to display.\n");
        return;
    }
    int indices[MAX_RESERVATIONS];
    for(int i = 0; i < reservationCount; i++) {
        indices[i] = i;
    }
    printf("\n--- All Reservations ---\n");
    printReservationTable(reservationCount, indices);
}

// New function to display all users
void displayAllUsers() {
    if (userCount == 0) {
        printf("No users to display.\n");
        return;
    }
    printf("\n--- All System Users ---\n");
    printUserTable();
}

// ======================= USER FUNCTIONS =======================

void userMenu(const char *username) {
    int choice;
    
    do {
        printf("\n--- User Menu ---\n");
        printf("1. Search flights\n");
        printf("2. Book a reservation\n");
        printf("3. Cancel a reservation\n");
        printf("4. Display my reservations\n");
        printf("5. Log out\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: searchFlights(); break;
            case 2: bookReservation(username); break;
            case 3: cancelReservation(username); break;
            case 4: displayMyReservations(username); break;
            case 5: printf("Logging out...\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 5);
}

void searchFlights() {
    char origin[50], destination[50];
    printf("\n--- Search Flights ---\n");
    printf("Enter Origin: ");
    scanf(" %s", origin);
    printf("Enter Destination: ");
    scanf(" %s", destination);
    
    int foundIndices[MAX_FLIGHTS];
    int foundCount = 0;
    
    for (int i = 0; i < flightCount; i++) {
        if (strcmp(flights[i].origin, origin) == 0 && strcmp(flights[i].destination, destination) == 0) {
            foundIndices[foundCount] = i;
            foundCount++;
        }
    }
    
    if (foundCount > 0) {
        printf("Found %d flight(s).\n", foundCount);
        printFlightTable(foundCount, foundIndices);
    } else {
        printf("No flights found for this route.\n");
    }
}

void bookReservation(const char* username) {
    if (reservationCount >= MAX_RESERVATIONS) {
        printf("Cannot book, maximum reservation limit reached.\n");
        return;
    }
    displayAllFlights();
    int flightNum;
    printf("\n--- Book Reservation ---\n");
    printf("Enter the Flight Number you wish to book: ");
    scanf("%d", &flightNum);
    
    int foundFlightIndex = -1;
    for (int i = 0; i < flightCount; i++) {
        if (flights[i].flightNumber == flightNum) {
            foundFlightIndex = i;
            break;
        }
    }
    
    if (foundFlightIndex == -1) {
        printf("Flight not found.\n");
        return;
    }
    
    if (flights[foundFlightIndex].availableSeats <= 0) {
        printf("Sorry, this flight is full.\n");
        return;
    }
    
    // Create new reservation
    reservations[reservationCount].reservationID = generateReservationID();
    reservations[reservationCount].flightNumber = flightNum;
    strcpy(reservations[reservationCount].passengerName, username);
    
    flights[foundFlightIndex].availableSeats--;
    reservationCount++;
    
    printf("Reservation successful! Your reservation ID is: %d\n", reservations[reservationCount-1].reservationID);
    saveData();
}

void cancelReservation(const char* username) {
    int resID;
    printf("\n--- Cancel Reservation ---\n");
    printf("Enter your Reservation ID to cancel: ");
    scanf("%d", &resID);
    
    int foundIndex = -1;
    for (int i = 0; i < reservationCount; i++) {
        if (reservations[i].reservationID == resID) {
            foundIndex = i;
            break;
        }
    }
    
    if (foundIndex != -1) {
        int flightNum = reservations[foundIndex].flightNumber;
        
        // Find the corresponding flight and increment available seats
        for (int i = 0; i < flightCount; i++) {
            if (flights[i].flightNumber == flightNum) {
                flights[i].availableSeats++;
                break;
            }
        }
        
        // Shift reservations to fill the gap
        for (int i = foundIndex; i < reservationCount - 1; i++) {
            reservations[i] = reservations[i+1];
        }
        reservationCount--;
        printf("Reservation cancelled successfully.\n");
        saveData();
    } else {
        printf("Reservation not found.\n");
    }
}

void displayMyReservations(const char* username) {
    int foundIndices[MAX_RESERVATIONS];
    int foundCount = 0;
    
    for (int i = 0; i < reservationCount; i++) {
        if (strcmp(reservations[i].passengerName, username) == 0) {
            foundIndices[foundCount] = i;
            foundCount++;
        }
    }
    
    if (foundCount > 0) {
        printf("\n--- My Reservations ---\n");
        printReservationTable(foundCount, foundIndices);
    } else {
        printf("You have no reservations.\n");
    }
}


// ======================= DATA PERSISTENCE =======================

void loadData() {
    // Load flights
    FILE* fptr = fopen(FLIGHT_FILE, "r");
    if (fptr != NULL) {
        while (fscanf(fptr, "%d %s %s %s %d %d\n", 
                &flights[flightCount].flightNumber, flights[flightCount].origin, 
                flights[flightCount].destination, flights[flightCount].departureTime, 
                &flights[flightCount].totalSeats, &flights[flightCount].availableSeats) == 6) {
            flightCount++;
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

    // Load reservations
    fptr = fopen(RESERVATION_FILE, "r");
    if (fptr != NULL) {
        while (fscanf(fptr, "%d %d %s\n", &reservations[reservationCount].reservationID,
                &reservations[reservationCount].flightNumber, reservations[reservationCount].passengerName) == 3) {
            reservationCount++;
        }
        fclose(fptr);
    }
}

void saveData() {
    // Save flights
    FILE* fptr = fopen(FLIGHT_FILE, "w");
    if (fptr != NULL) {
        for (int i = 0; i < flightCount; i++) {
            fprintf(fptr, "%d %s %s %s %d %d\n", 
                    flights[i].flightNumber, flights[i].origin, 
                    flights[i].destination, flights[i].departureTime, 
                    flights[i].totalSeats, flights[i].availableSeats);
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

    // Save reservations
    fptr = fopen(RESERVATION_FILE, "w");
    if (fptr != NULL) {
        for (int i = 0; i < reservationCount; i++) {
            fprintf(fptr, "%d %d %s\n", reservations[i].reservationID,
                    reservations[i].flightNumber, reservations[i].passengerName);
        }
        fclose(fptr);
    }
}

// ======================= UTILITY FUNCTIONS =======================

int generateReservationID() {
    // Simple ID generation based on current reservation count
    if (reservationCount == 0) return 1000;
    return reservations[reservationCount-1].reservationID + 1;
}

void printFlightTable(int count, const int* indices) {
    printf("+------------+--------------------+--------------------+--------------------+----------+----------+\n");
    printf("| Flight No. |       Origin       |     Destination    |   Departure Time   |  Total   | Available|\n");
    printf("+------------+--------------------+--------------------+--------------------+----------+----------+\n");
    for (int i = 0; i < count; i++) {
        int idx = indices[i];
        printf("| %-10d | %-18s | %-18s | %-18s | %-8d | %-8d |\n",
               flights[idx].flightNumber, flights[idx].origin, flights[idx].destination,
               flights[idx].departureTime, flights[idx].totalSeats, flights[idx].availableSeats);
    }
    printf("+------------+--------------------+--------------------+--------------------+----------+----------+\n");
}

void printReservationTable(int count, const int* indices) {
    printf("+---------------+------------+--------------------+\n");
    printf("| Reservation ID| Flight No. |   Passenger Name   |\n");
    printf("+---------------+------------+--------------------+\n");
    for (int i = 0; i < count; i++) {
        int idx = indices[i];
        printf("| %-13d | %-10d | %-18s |\n",
               reservations[idx].reservationID, reservations[idx].flightNumber, reservations[idx].passengerName);
    }
    printf("+---------------+------------+--------------------+\n");
}

// New function to display user data in a tabular format
void printUserTable() {
    printf("+--------------------+------------+\n");
    printf("|      Username      |    Role    |\n");
    printf("+--------------------+------------+\n");
    for (int i = 0; i < userCount; i++) {
        printf("| %-18s | %-10s |\n", users[i].username, users[i].role);
    }
    printf("+--------------------+------------+\n");
}