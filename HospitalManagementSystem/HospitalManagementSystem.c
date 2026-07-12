#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// --- Data Structures ---

// Struct for user authentication
typedef struct {
    char username[50];
    char password[50];
    char role[10]; // "admin" or "user"
} User;

// Struct for patient details
typedef struct {
    char patient_id[10];
    char name[50];
    char contact[15];
    char address[100];
    char medical_history[256];
    char username[50]; // Link to the user account
} Patient;

// Struct for a billing receipt
typedef struct {
    char receipt_id[10];
    char patient_id[10];
    char date[20];
    char service[50];
    double amount;
} Receipt;

// --- Function Prototypes ---
void login_page();
void admin_mode();
void user_mode(const char* username);
int authenticate(const char* username, const char* password, char* role);
void register_user();
int check_username_exists(const char* username);
void clear_input_buffer();

// Admin functions
void add_patient_record();
void view_all_patients();
void update_patient_record();
void generate_receipt();
void view_all_receipts();
void view_patient_details_by_id(const char* patient_id);

// User (Patient) functions
void view_my_details(const char* username);
void view_my_receipts(const char* username);
void update_my_medical_history(const char* username);

// --- Main Function ---
int main() {
    // Keep the program running so a user can return to the login page after logging out.
    while (1) {
        login_page();
    }
    return 0;
}

// --- Authentication and Login ---
void login_page() {
    char username[50];
    char password[50];
    char role[10];
    int choice;
    int attempts = 3;

    printf("\n--- Welcome to the Hospital Management System ---\n");
    printf("1. Admin Login\n");
    printf("2. User Login\n");
    printf("3. Register\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    
    clear_input_buffer();

    if (choice == 1) {
        strcpy(role, "admin");
    } else if (choice == 2) {
        strcpy(role, "user");
    } else if (choice == 3) {
        register_user();
        return; 
    } else if (choice == 4) {
        printf("Exiting the application. Goodbye!\n");
        exit(0);
    } else {
        printf("Invalid choice. Please try again.\n");
        return;
    }

    while (attempts > 0) {
        printf("\n--- %s Login ---", role);
        printf("\nEnter username: ");
        scanf("%s", username);
        printf("Enter password: ");
        scanf("%s", password);

        if (authenticate(username, password, role)) {
            printf("\nLogin successful! Welcome, %s.\n", username);
            if (strcmp(role, "admin") == 0) {
                admin_mode();
            } else {
                user_mode(username);
            }
            return;
        } else {
            attempts--;
            printf("\nAuthentication failed. %d attempts remaining.\n", attempts);
        }
    }
    printf("\nToo many failed attempts. Exiting.\n");
    exit(0);
}

int authenticate(const char* username, const char* password, char* role) {
    FILE* file = fopen("users.txt", "r");
    if (!file) {
        printf("Error: 'users.txt' file not found. Creating a default admin user.\n");
        file = fopen("users.txt", "w");
        fprintf(file, "admin admin_pass admin\n");
        fprintf(file, "testuser pass user\n");
        fclose(file);
        return 0;
    }

    User user_data;
    int found = 0;
    while (fscanf(file, "%s %s %s", user_data.username, user_data.password, user_data.role) != EOF) {
        if (strcmp(username, user_data.username) == 0 && strcmp(password, user_data.password) == 0 && strcmp(role, user_data.role) == 0) {
            strcpy(role, user_data.role);
            found = 1;
            break;
        }
    }
    fclose(file);
    return found;
}

void register_user() {
    char new_username[50];
    char new_password[50];
    char new_role[10];
    
    printf("\n--- New User Registration ---\n");
    printf("Enter a new username: ");
    scanf("%s", new_username);
    clear_input_buffer();

    if (check_username_exists(new_username)) {
        printf("Username '%s' already exists. Please choose a different one.\n", new_username);
        return; 
    }

    printf("Enter a password: ");
    scanf("%s", new_password);
    clear_input_buffer();
    
    printf("Enter desired role (admin or user): ");
    scanf("%s", new_role);
    clear_input_buffer();

    if (strcmp(new_role, "admin") != 0 && strcmp(new_role, "user") != 0) {
        printf("Invalid role. Defaulting to 'user'.\n");
        strcpy(new_role, "user");
    }

    FILE* file = fopen("users.txt", "a");
    if (!file) {
        printf("Error: Could not open 'users.txt' for writing.\n");
        return;
    }

    fprintf(file, "%s %s %s\n", new_username, new_password, new_role);
    fclose(file);
    printf("Registration successful! You can now log in.\n");
}

int check_username_exists(const char* username) {
    FILE* file = fopen("users.txt", "r");
    if (!file) {
        return 0; 
    }

    User user_data;
    while (fscanf(file, "%s %*s %*s", user_data.username) != EOF) {
        if (strcmp(username, user_data.username) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0; 
}

// --- Admin Mode ---
void admin_mode() {
    int choice;
    do {
        printf("\n--- Admin Dashboard ---\n");
        printf("1. Add New Patient Record\n");
        printf("2. View All Patient Records\n");
        printf("3. Update Patient Record\n");
        printf("4. Generate Receipt\n");
        printf("5. View All Receipts\n");
        printf("6. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clear_input_buffer();

        switch (choice) {
            case 1:
                add_patient_record();
                break;
            case 2:
                view_all_patients();
                break;
            case 3:
                update_patient_record();
                break;
            case 4:
                generate_receipt();
                break;
            case 5:
                view_all_receipts();
                break;
            case 6:
                printf("Logging out...\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);
}

void add_patient_record() {
    Patient new_patient;
    printf("\n--- Add New Patient Record ---\n");
    printf("Enter Patient ID: ");
    scanf("%s", new_patient.patient_id);
    clear_input_buffer();
    
    printf("Enter Patient Name: ");
    fgets(new_patient.name, sizeof(new_patient.name), stdin);
    new_patient.name[strcspn(new_patient.name, "\n")] = 0;

    printf("Enter Contact Number: ");
    scanf("%s", new_patient.contact);
    clear_input_buffer();

    printf("Enter Address: ");
    fgets(new_patient.address, sizeof(new_patient.address), stdin);
    new_patient.address[strcspn(new_patient.address, "\n")] = 0;

    printf("Enter Initial Medical History: ");
    fgets(new_patient.medical_history, sizeof(new_patient.medical_history), stdin);
    new_patient.medical_history[strcspn(new_patient.medical_history, "\n")] = 0;

    printf("Enter linked username: ");
    scanf("%s", new_patient.username);
    clear_input_buffer();
    
    // Save to file
    FILE* file = fopen("patients.txt", "a");
    if (!file) {
        printf("Error: Could not open 'patients.txt' for writing.\n");
        return;
    }
    fprintf(file, "%s|%s|%s|%s|%s|%s\n", 
            new_patient.patient_id, 
            new_patient.name, 
            new_patient.contact, 
            new_patient.address, 
            new_patient.medical_history,
            new_patient.username);
    fclose(file);
    printf("Patient record added successfully.\n");
}

void view_all_patients() {
    FILE* file = fopen("patients.txt", "r");
    if (!file) {
        printf("No patient records found.\n");
        return;
    }
    
    Patient patient;
    printf("\n--- All Patient Records ---\n");
    printf("+------------+----------------------+-----------------+-----------------+-----------------+--------------+\n");
    printf("| Patient ID | Name                 | Contact         | Address         | medical_history | username     |\n");
    printf("+------------+----------------------+-----------------+-----------------+-----------------+--------------+\n");
    
    while (fscanf(file, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n", 
                  patient.patient_id, 
                  patient.name, 
                  patient.contact, 
                  patient.address, 
                  patient.medical_history,
                  patient.username) != EOF) {
        printf("| %-10s | %-20s | %-15s | %-15s | %-15s | %-12s |\n", 
               patient.patient_id, patient.name, patient.contact, patient.address,patient.medical_history,patient.username);
    }
                                                
    printf("+------------+----------------------+-----------------+-----------------+-----------------+--------------+\n");
    fclose(file);
}

void update_patient_record() {
    char patient_id[10];
    Patient patients[100];
    int count = 0;
    
    printf("\n--- Update Patient Record ---\n");
    printf("Enter the Patient ID to update: ");
    scanf("%s", patient_id);
    clear_input_buffer();
    
    FILE* file = fopen("patients.txt", "r");
    if (!file) {
        printf("No patient records found.\n");
        return;
    }
    
    while (count < 100 && fscanf(file, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n", 
                                  patients[count].patient_id, 
                                  patients[count].name, 
                                  patients[count].contact, 
                                  patients[count].address, 
                                  patients[count].medical_history,
                                  patients[count].username) != EOF) {
        count++;
    }
    fclose(file);
    
    int found_index = -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(patients[i].patient_id, patient_id) == 0) {
            found_index = i;
            break;
        }
    }
    
    if (found_index != -1) {
        printf("Patient found. Enter new details:\n");
        printf("New Name: ");
        fgets(patients[found_index].name, sizeof(patients[found_index].name), stdin);
        patients[found_index].name[strcspn(patients[found_index].name, "\n")] = 0;
        
        printf("New Contact: ");
        scanf("%s", patients[found_index].contact);
        clear_input_buffer();
        
        printf("New Address: ");
        fgets(patients[found_index].address, sizeof(patients[found_index].address), stdin);
        patients[found_index].address[strcspn(patients[found_index].address, "\n")] = 0;
        
        printf("New Medical History: ");
        fgets(patients[found_index].medical_history, sizeof(patients[found_index].medical_history), stdin);
        patients[found_index].medical_history[strcspn(patients[found_index].medical_history, "\n")] = 0;

        printf("New linked username: ");
        scanf("%s", patients[found_index].username);
        clear_input_buffer();
        
        // Rewrite the file
        file = fopen("patients.txt", "w");
        if (!file) {
            printf("Error: Could not open 'patients.txt' for writing.\n");
            return;
        }
        for (int i = 0; i < count; i++) {
            fprintf(file, "%s|%s|%s|%s|%s|%s\n", 
                    patients[i].patient_id, 
                    patients[i].name, 
                    patients[i].contact, 
                    patients[i].address, 
                    patients[i].medical_history,
                    patients[i].username);
        }
        fclose(file);
        printf("Patient record updated successfully.\n");
    } else {
        printf("Patient with ID '%s' not found.\n", patient_id);
    }
}

void generate_receipt() {
    Receipt new_receipt;
    
    printf("\n--- Generate Receipt ---\n");
    printf("Enter Receipt ID: ");
    scanf("%s", new_receipt.receipt_id);
    clear_input_buffer();

    printf("Enter Patient ID: ");
    scanf("%s", new_receipt.patient_id);
    clear_input_buffer();

    printf("Enter Service Description: ");
    fgets(new_receipt.service, sizeof(new_receipt.service), stdin);
    new_receipt.service[strcspn(new_receipt.service, "\n")] = 0;

    printf("Enter Amount: ");
    scanf("%lf", &new_receipt.amount);
    clear_input_buffer();

    printf("Enter Date (YYYY-MM-DD): ");
    scanf("%s", new_receipt.date);
    clear_input_buffer();
    
    FILE* file = fopen("receipts.txt", "a");
    if (!file) {
        printf("Error: Could not open 'receipts.txt' for writing.\n");
        return;
    }
    
    fprintf(file, "%s|%s|%s|%s|%.2f\n", 
            new_receipt.receipt_id, 
            new_receipt.patient_id, 
            new_receipt.date, 
            new_receipt.service, 
            new_receipt.amount);
    fclose(file);
    printf("Receipt generated successfully.\n");
}

void view_all_receipts() {
    FILE* file = fopen("receipts.txt", "r");
    if (!file) {
        printf("No receipts found.\n");
        return;
    }
    
    Receipt receipt;
    printf("\n--- All Receipts ---\n");
    printf("+------------+------------+-----------------+----------------------------------+-----------+\n");
    printf("| Receipt ID | Patient ID | Date            | Service                          | Amount    |\n");
    printf("+------------+------------+-----------------+----------------------------------+-----------+\n");
    
    while (fscanf(file, "%[^|]|%[^|]|%[^|]|%[^|]|%lf\n", 
                  receipt.receipt_id, 
                  receipt.patient_id, 
                  receipt.date, 
                  receipt.service, 
                  &receipt.amount) != EOF) {
        printf("| %-10s | %-10s | %-15s | %-32s | %-9.2f |\n", 
               receipt.receipt_id, receipt.patient_id, receipt.date, receipt.service, receipt.amount);
    }
    
    printf("+------------+------------+-----------------+----------------------------------+-----------+\n");
    fclose(file);
}

void view_patient_details_by_id(const char* patient_id) {
    FILE* file = fopen("patients.txt", "r");
    if (!file) {
        printf("No patient records found.\n");
        return;
    }
    
    Patient patient;
    int found = 0;
    while (fscanf(file, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n", 
                  patient.patient_id, 
                  patient.name, 
                  patient.contact, 
                  patient.address, 
                  patient.medical_history,
                  patient.username) != EOF) {
        if (strcmp(patient.patient_id, patient_id) == 0) {
            printf("\n--- Patient Details ---\n");
            printf("Patient ID: %s\n", patient.patient_id);
            printf("Name: %s\n", patient.name);
            printf("Contact: %s\n", patient.contact);
            printf("Address: %s\n", patient.address);
            printf("Medical History: %s\n", patient.medical_history);
            found = 1;
            break;
        }
    }
    fclose(file);
    if (!found) {
        printf("Patient with ID '%s' not found.\n", patient_id);
    }
}


// --- User Mode ---
void user_mode(const char* username) {
    int choice;
    do {
        printf("\n--- Patient Dashboard ---\n");
        printf("1. View My Details\n");
        printf("2. View My Receipts\n");
        printf("3. Update My Medical History\n");
        printf("4. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clear_input_buffer();

        switch (choice) {
            case 1:
                view_my_details(username);
                break;
            case 2:
                view_my_receipts(username);
                break;
            case 3:
                update_my_medical_history(username);
                break;
            case 4:
                printf("Logging out...\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);
}

void view_my_details(const char* username) {
    FILE* file = fopen("patients.txt", "r");
    if (!file) {
        printf("No patient records found.\n");
        return;
    }
    
    Patient patient;
    int found = 0;
    while (fscanf(file, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n", 
                  patient.patient_id, 
                  patient.name, 
                  patient.contact, 
                  patient.address, 
                  patient.medical_history,
                  patient.username) != EOF) {
        if (strcmp(patient.username, username) == 0) {
            printf("\n--- Your Details ---\n");
            printf("Patient ID: %s\n", patient.patient_id);
            printf("Name: %s\n", patient.name);
            printf("Contact: %s\n", patient.contact);
            printf("Address: %s\n", patient.address);
            printf("Medical History: %s\n", patient.medical_history);
            found = 1;
            break;
        }
    }
    fclose(file);
    if (!found) {
        printf("Your patient record could not be found.\n");
    }
}

void view_my_receipts(const char* username) {
    FILE* patients_file = fopen("patients.txt", "r");
    if (!patients_file) {
        printf("Patient records not found.\n");
        return;
    }

    char patient_id[10] = "";
    Patient temp_patient;
    while (fscanf(patients_file, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n", 
                  temp_patient.patient_id, temp_patient.name, temp_patient.contact, 
                  temp_patient.address, temp_patient.medical_history, temp_patient.username) != EOF) {
        if (strcmp(temp_patient.username, username) == 0) {
            strcpy(patient_id, temp_patient.patient_id);
            break;
        }
    }
    fclose(patients_file);

    if (strlen(patient_id) == 0) {
        printf("Your patient record could not be found, so no receipts can be displayed.\n");
        return;
    }

    FILE* receipts_file = fopen("receipts.txt", "r");
    if (!receipts_file) {
        printf("No receipts found for your account.\n");
        return;
    }

    Receipt receipt;
    int found_receipts = 0;

    printf("\n--- Your Receipts ---\n");
    printf("+------------+-----------------+----------------------------------+-----------+\n");
    printf("| Receipt ID | Date            | Service                          | Amount    |\n");
    printf("+------------+-----------------+----------------------------------+-----------+\n");
    
    while (fscanf(receipts_file, "%[^|]|%[^|]|%[^|]|%[^|]|%lf\n", 
                  receipt.receipt_id, 
                  receipt.patient_id, 
                  receipt.date, 
                  receipt.service, 
                  &receipt.amount) != EOF) {
        if (strcmp(receipt.patient_id, patient_id) == 0) {
            printf("| %-10s | %-15s | %-32s | %-9.2f |\n", 
                   receipt.receipt_id, receipt.date, receipt.service, receipt.amount);
            found_receipts = 1;
        }
    }
    
    printf("+------------+-----------------+----------------------------------+-----------+\n");
    fclose(receipts_file);
    
    if (!found_receipts) {
        printf("No receipts found for your account.\n");
    }
}

void update_my_medical_history(const char* username) {
    Patient patients[100];
    int count = 0;

    FILE* file = fopen("patients.txt", "r");
    if (!file) {
        printf("No patient records found.\n");
        return;
    }
    
    while (count < 100 && fscanf(file, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n", 
                                  patients[count].patient_id, 
                                  patients[count].name, 
                                  patients[count].contact, 
                                  patients[count].address, 
                                  patients[count].medical_history,
                                  patients[count].username) != EOF) {
        count++;
    }
    fclose(file);
    
    int found_index = -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(patients[i].username, username) == 0) {
            found_index = i;
            break;
        }
    }

    if (found_index != -1) {
        printf("\n--- Update Your Medical History ---\n");
        printf("Current Medical History: %s\n", patients[found_index].medical_history);
        printf("Enter new Medical History: ");
        fgets(patients[found_index].medical_history, sizeof(patients[found_index].medical_history), stdin);
        patients[found_index].medical_history[strcspn(patients[found_index].medical_history, "\n")] = 0;
        
        // Rewrite the file
        file = fopen("patients.txt", "w");
        if (!file) {
            printf("Error: Could not open 'patients.txt' for writing.\n");
            return;
        }
        for (int i = 0; i < count; i++) {
            fprintf(file, "%s|%s|%s|%s|%s|%s\n", 
                    patients[i].patient_id, 
                    patients[i].name, 
                    patients[i].contact, 
                    patients[i].address, 
                    patients[i].medical_history,
                    patients[i].username);
        }
        fclose(file);
        printf("Medical history updated successfully.\n");
    } else {
        printf("Your patient record could not be found.\n");
    }
}

// --- Helper Function ---
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
