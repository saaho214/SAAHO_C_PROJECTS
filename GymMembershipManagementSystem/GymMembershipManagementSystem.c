#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// --- Data Structures ---

// Struct for user authentication and role
typedef struct {
    char username[50];
    char password[50];
    char role[10]; // "admin" or "member"
} User;

// Struct for a gym member's details
typedef struct {
    char member_id[10];
    char name[50];
    char contact[15];
    char email[50];
    char membership_plan[20];
    char workout_log[512]; // Simple log of workouts
    char username[50]; // Linked to user account
} Member;

// Struct for a membership plan
typedef struct {
    char plan_id[10];
    char plan_name[20];
    double price;
} MembershipPlan;

// Struct for staff
typedef struct {
    char staff_id[10];
    char name[50];
    char role[20]; // e.g., "Trainer", "Receptionist"
} Staff;

// Struct for a class schedule
typedef struct {
    char class_id[10];
    char name[50];
    char day[15];
    char time[10];
    char trainer[50];
} GymClass;

// Struct for a billing receipt
typedef struct {
    char receipt_id[10];
    char member_id[10];
    char date[20];
    char service[50];
    double amount;
} Receipt;

// --- Function Prototypes ---
void login_page();
void admin_mode();
void member_mode(const char* username);
int authenticate(const char* username, const char* password, char* role);
void register_user();
int check_username_exists(const char* username);
void clear_input_buffer();

// Admin functions
void add_member();
void view_all_members();
void update_member_details();
void manage_membership_plans();
void add_staff();
void view_all_staff();
void add_class();
void view_all_classes();
void generate_receipt();
void view_all_receipts();
void view_member_details_by_id(const char* member_id);

// Member functions
void view_my_profile(const char* username);
void update_my_profile(const char* username);
void view_class_schedule();
void book_class(const char* username);
void view_my_receipts(const char* username);
void log_workout(const char* username);
void view_my_workout_log(const char* username);

// --- Main Function ---
int main() {
    // Keeps the program running so a user can return to the login page after logging out.
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

    printf("\n--- Welcome to the Gym Management System ---\n");
    printf("1. Admin Login\n");
    printf("2. Member Login\n");
    printf("3. Register\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    
    clear_input_buffer();

    if (choice == 1) {
        strcpy(role, "admin");
    } else if (choice == 2) {
        strcpy(role, "member");
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
                member_mode(username);
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

// Authenticates user against data in users.txt
int authenticate(const char* username, const char* password, char* role) {
    FILE* file = fopen("users.txt", "r");
    if (!file) {
        printf("Error: 'users.txt' file not found. Creating a default admin user.\n");
        file = fopen("users.txt", "w");
        fprintf(file, "admin admin_pass admin\n");
        fprintf(file, "member1 pass member\n");
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

// Registers a new user account
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
    
    printf("Enter desired role (admin or member): ");
    scanf("%s", new_role);
    clear_input_buffer();

    if (strcmp(new_role, "admin") != 0 && strcmp(new_role, "member") != 0) {
        printf("Invalid role. Defaulting to 'member'.\n");
        strcpy(new_role, "member");
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

// Checks if a username already exists in the system
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
        printf("1. Add New Member\n");
        printf("2. View All Members\n");
        printf("3. Update Member Details\n");
        printf("4. Manage Membership Plans\n");
        printf("5. Add New Staff Member\n");
        printf("6. View All Staff\n");
        printf("7. Add a New Class\n");
        printf("8. View All Classes\n");
        printf("9. Generate Receipt\n");
        printf("10. View All Receipts\n");
        printf("11. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clear_input_buffer();

        switch (choice) {
            case 1: add_member(); break;
            case 2: view_all_members(); break;
            case 3: update_member_details(); break;
            case 4: manage_membership_plans(); break;
            case 5: add_staff(); break;
            case 6: view_all_staff(); break;
            case 7: add_class(); break;
            case 8: view_all_classes(); break;
            case 9: generate_receipt(); break;
            case 10: view_all_receipts(); break;
            case 11: printf("Logging out...\n"); return;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 11);
}

// Adds a new member to the system
void add_member() {
    Member new_member;
    printf("\n--- Add New Member ---\n");
    printf("Enter Member ID: ");
    scanf("%s", new_member.member_id);
    clear_input_buffer();
    
    printf("Enter Member Name: ");
    fgets(new_member.name, sizeof(new_member.name), stdin);
    new_member.name[strcspn(new_member.name, "\n")] = 0;

    printf("Enter Contact Number: ");
    scanf("%s", new_member.contact);
    clear_input_buffer();

    printf("Enter Email: ");
    scanf("%s", new_member.email);
    clear_input_buffer();

    printf("Enter Membership Plan (e.g., Monthly, Annual): ");
    fgets(new_member.membership_plan, sizeof(new_member.membership_plan), stdin);
    new_member.membership_plan[strcspn(new_member.membership_plan, "\n")] = 0;

    strcpy(new_member.workout_log, "No workouts logged yet.");

    printf("Enter linked username: ");
    scanf("%s", new_member.username);
    clear_input_buffer();
    
    FILE* file = fopen("members.txt", "a");
    if (!file) {
        printf("Error: Could not open 'members.txt' for writing.\n");
        return;
    }
    fprintf(file, "%s|%s|%s|%s|%s|%s|%s\n", 
            new_member.member_id, 
            new_member.name, 
            new_member.contact, 
            new_member.email, 
            new_member.membership_plan, 
            new_member.workout_log,
            new_member.username);
    fclose(file);
    printf("Member record added successfully.\n");
}

// Displays all member records in a tabular format
void view_all_members() {
    FILE* file = fopen("members.txt", "r");
    if (!file) {
        printf("No member records found.\n");
        return;
    }
    
    Member member;
    printf("\n--- All Members ---\n");
    printf("+------------+--------------------------------+-----------------+--------------------------------+\n");
    printf("| Member ID  | Name                           | Contact         | Membership Plan                |\n");
    printf("+------------+--------------------------------+-----------------+--------------------------------+\n");
    
    while (fscanf(file, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n", 
                  member.member_id, 
                  member.name, 
                  member.contact, 
                  member.email, 
                  member.membership_plan, 
                  member.workout_log,
                  member.username) != EOF) {
        printf("| %-10s | %-30s | %-15s | %-30s |\n", 
               member.member_id, member.name, member.contact, member.membership_plan);
    }
    
    printf("+------------+--------------------------------+-----------------+--------------------------------+\n");
    fclose(file);
}

// Allows admin to update an existing member's details
void update_member_details() {
    char member_id[10];
    Member members[100];
    int count = 0;
    
    printf("\n--- Update Member Details ---\n");
    printf("Enter the Member ID to update: ");
    scanf("%s", member_id);
    clear_input_buffer();
    
    FILE* file = fopen("members.txt", "r");
    if (!file) {
        printf("No member records found.\n");
        return;
    }
    
    while (count < 100 && fscanf(file, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n", 
                                  members[count].member_id, 
                                  members[count].name, 
                                  members[count].contact, 
                                  members[count].email, 
                                  members[count].membership_plan, 
                                  members[count].workout_log,
                                  members[count].username) != EOF) {
        count++;
    }
    fclose(file);
    
    int found_index = -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(members[i].member_id, member_id) == 0) {
            found_index = i;
            break;
        }
    }
    
    if (found_index != -1) {
        printf("Member found. Enter new details:\n");
        printf("New Name: ");
        fgets(members[found_index].name, sizeof(members[found_index].name), stdin);
        members[found_index].name[strcspn(members[found_index].name, "\n")] = 0;
        
        printf("New Contact: ");
        scanf("%s", members[found_index].contact);
        clear_input_buffer();
        
        printf("New Email: ");
        scanf("%s", members[found_index].email);
        clear_input_buffer();
        
        printf("New Membership Plan: ");
        fgets(members[found_index].membership_plan, sizeof(members[found_index].membership_plan), stdin);
        members[found_index].membership_plan[strcspn(members[found_index].membership_plan, "\n")] = 0;
        
        // Rewrite the file
        file = fopen("members.txt", "w");
        if (!file) {
            printf("Error: Could not open 'members.txt' for writing.\n");
            return;
        }
        for (int i = 0; i < count; i++) {
            fprintf(file, "%s|%s|%s|%s|%s|%s|%s\n", 
                    members[i].member_id, 
                    members[i].name, 
                    members[i].contact, 
                    members[i].email, 
                    members[i].membership_plan, 
                    members[i].workout_log,
                    members[i].username);
        }
        fclose(file);
        printf("Member record updated successfully.\n");
    } else {
        printf("Member with ID '%s' not found.\n", member_id);
    }
}

// Manages membership plans (add or update)
void manage_membership_plans() {
    MembershipPlan new_plan;
    printf("\n--- Add New Membership Plan ---\n");
    printf("Enter Plan ID: ");
    scanf("%s", new_plan.plan_id);
    clear_input_buffer();

    printf("Enter Plan Name: ");
    fgets(new_plan.plan_name, sizeof(new_plan.plan_name), stdin);
    new_plan.plan_name[strcspn(new_plan.plan_name, "\n")] = 0;

    printf("Enter Price: ");
    scanf("%lf", &new_plan.price);
    clear_input_buffer();

    FILE* file = fopen("plans.txt", "a");
    if (!file) {
        printf("Error: Could not open 'plans.txt' for writing.\n");
        return;
    }
    fprintf(file, "%s|%s|%.2f\n", new_plan.plan_id, new_plan.plan_name, new_plan.price);
    fclose(file);
    printf("Membership plan added successfully.\n");
}

// Adds a new staff member
void add_staff() {
    Staff new_staff;
    printf("\n--- Add New Staff Member ---\n");
    printf("Enter Staff ID: ");
    scanf("%s", new_staff.staff_id);
    clear_input_buffer();

    printf("Enter Staff Name: ");
    fgets(new_staff.name, sizeof(new_staff.name), stdin);
    new_staff.name[strcspn(new_staff.name, "\n")] = 0;

    printf("Enter Staff Role: ");
    fgets(new_staff.role, sizeof(new_staff.role), stdin);
    new_staff.role[strcspn(new_staff.role, "\n")] = 0;

    FILE* file = fopen("staff.txt", "a");
    if (!file) {
        printf("Error: Could not open 'staff.txt' for writing.\n");
        return;
    }
    fprintf(file, "%s|%s|%s\n", new_staff.staff_id, new_staff.name, new_staff.role);
    fclose(file);
    printf("Staff member added successfully.\n");
}

// Displays all staff members
void view_all_staff() {
    FILE* file = fopen("staff.txt", "r");
    if (!file) {
        printf("No staff records found.\n");
        return;
    }
    
    Staff staff;
    printf("\n--- All Staff Members ---\n");
    printf("+------------+--------------------------------+----------------------+\n");
    printf("| Staff ID   | Name                           | Role                 |\n");
    printf("+------------+--------------------------------+----------------------+\n");
    
    while (fscanf(file, "%[^|]|%[^|]|%[^\n]\n", 
                  staff.staff_id, 
                  staff.name, 
                  staff.role) != EOF) {
        printf("| %-10s | %-30s | %-20s |\n", 
               staff.staff_id, staff.name, staff.role);
    }
    
    printf("+------------+--------------------------------+----------------------+\n");
    fclose(file);
}

// Adds a new class to the schedule
void add_class() {
    GymClass new_class;
    printf("\n--- Add New Class ---\n");
    printf("Enter Class ID: ");
    scanf("%s", new_class.class_id);
    clear_input_buffer();

    printf("Enter Class Name: ");
    fgets(new_class.name, sizeof(new_class.name), stdin);
    new_class.name[strcspn(new_class.name, "\n")] = 0;

    printf("Enter Day: ");
    scanf("%s", new_class.day);
    clear_input_buffer();

    printf("Enter Time: ");
    scanf("%s", new_class.time);
    clear_input_buffer();

    printf("Enter Trainer: ");
    fgets(new_class.trainer, sizeof(new_class.trainer), stdin);
    new_class.trainer[strcspn(new_class.trainer, "\n")] = 0;

    FILE* file = fopen("classes.txt", "a");
    if (!file) {
        printf("Error: Could not open 'classes.txt' for writing.\n");
        return;
    }
    fprintf(file, "%s|%s|%s|%s|%s\n", new_class.class_id, new_class.name, new_class.day, new_class.time, new_class.trainer);
    fclose(file);
    printf("Class added successfully.\n");
}

// Displays the class schedule in a tabular format
void view_all_classes() {
    FILE* file = fopen("classes.txt", "r");
    if (!file) {
        printf("No classes found.\n");
        return;
    }
    
    GymClass gym_class;
    printf("\n--- Class Schedule ---\n");
    printf("+------------+--------------------------------+-----------------+------------+\n");
    printf("| Class ID   | Class Name                     | Day             | Time       |\n");
    printf("+------------+--------------------------------+-----------------+------------+\n");
    
    while (fscanf(file, "%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n", 
                  gym_class.class_id, 
                  gym_class.name, 
                  gym_class.day, 
                  gym_class.time, 
                  gym_class.trainer) != EOF) {
        printf("| %-10s | %-30s | %-15s | %-10s |\n", 
               gym_class.class_id, gym_class.name, gym_class.day, gym_class.time);
    }
    
    printf("+------------+--------------------------------+-----------------+------------+\n");
    fclose(file);
}

// Generates and stores a new billing receipt
void generate_receipt() {
    Receipt new_receipt;
    
    printf("\n--- Generate Receipt ---\n");
    printf("Enter Receipt ID: ");
    scanf("%s", new_receipt.receipt_id);
    clear_input_buffer();

    printf("Enter Member ID: ");
    scanf("%s", new_receipt.member_id);
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
            new_receipt.member_id, 
            new_receipt.date, 
            new_receipt.service, 
            new_receipt.amount);
    fclose(file);
    printf("Receipt generated successfully.\n");
}

// Displays all receipts in a tabular format
void view_all_receipts() {
    FILE* file = fopen("receipts.txt", "r");
    if (!file) {
        printf("No receipts found.\n");
        return;
    }
    
    Receipt receipt;
    printf("\n--- All Receipts ---\n");
    printf("+------------+------------+-----------------+----------------------------------+-----------+\n");
    printf("| Receipt ID | Member ID  | Date            | Service                          | Amount    |\n");
    printf("+------------+------------+-----------------+----------------------------------+-----------+\n");
    
    while (fscanf(file, "%[^|]|%[^|]|%[^|]|%[^|]|%lf\n", 
                  receipt.receipt_id, 
                  receipt.member_id, 
                  receipt.date, 
                  receipt.service, 
                  &receipt.amount) != EOF) {
        printf("| %-10s | %-10s | %-15s | %-32s | %-9.2f |\n", 
               receipt.receipt_id, receipt.member_id, receipt.date, receipt.service, receipt.amount);
    }
    
    printf("+------------+------------+-----------------+----------------------------------+-----------+\n");
    fclose(file);
}

// --- Member Mode ---
void member_mode(const char* username) {
    int choice;
    do {
        printf("\n--- Member Dashboard ---\n");
        printf("1. View My Profile\n");
        printf("2. Update My Profile\n");
        printf("3. View Class Schedule\n");
        printf("4. Book a Class\n");
        printf("5. View My Receipts\n");
        printf("6. Log a Workout\n");
        printf("7. View My Workout Log\n");
        printf("8. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clear_input_buffer();

        switch (choice) {
            case 1: view_my_profile(username); break;
            case 2: update_my_profile(username); break;
            case 3: view_class_schedule(); break;
            case 4: book_class(username); break;
            case 5: view_my_receipts(username); break;
            case 6: log_workout(username); break;
            case 7: view_my_workout_log(username); break;
            case 8: printf("Logging out...\n"); return;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 8);
}

// Displays the member's personal information
void view_my_profile(const char* username) {
    FILE* file = fopen("members.txt", "r");
    if (!file) {
        printf("No member records found.\n");
        return;
    }
    
    Member member;
    int found = 0;
    while (fscanf(file, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n", 
                  member.member_id, 
                  member.name, 
                  member.contact, 
                  member.email, 
                  member.membership_plan, 
                  member.workout_log,
                  member.username) != EOF) {
        if (strcmp(member.username, username) == 0) {
            printf("\n--- Your Profile ---\n");
            printf("Member ID: %s\n", member.member_id);
            printf("Name: %s\n", member.name);
            printf("Contact: %s\n", member.contact);
            printf("Email: %s\n", member.email);
            printf("Membership Plan: %s\n", member.membership_plan);
            found = 1;
            break;
        }
    }
    fclose(file);
    if (!found) {
        printf("Your member record could not be found.\n");
    }
}

// Allows a member to update their own contact information
void update_my_profile(const char* username) {
    Member members[100];
    int count = 0;

    FILE* file = fopen("members.txt", "r");
    if (!file) {
        printf("No member records found.\n");
        return;
    }
    
    while (count < 100 && fscanf(file, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n", 
                                  members[count].member_id, 
                                  members[count].name, 
                                  members[count].contact, 
                                  members[count].email, 
                                  members[count].membership_plan, 
                                  members[count].workout_log,
                                  members[count].username) != EOF) {
        count++;
    }
    fclose(file);
    
    int found_index = -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(members[i].username, username) == 0) {
            found_index = i;
            break;
        }
    }

    if (found_index != -1) {
        printf("\n--- Update Your Profile ---\n");
        printf("Current Contact: %s\n", members[found_index].contact);
        printf("Enter new Contact: ");
        scanf("%s", members[found_index].contact);
        clear_input_buffer();
        
        printf("Current Email: %s\n", members[found_index].email);
        printf("Enter new Email: ");
        scanf("%s", members[found_index].email);
        clear_input_buffer();
        
        // Rewrite the file
        file = fopen("members.txt", "w");
        if (!file) {
            printf("Error: Could not open 'members.txt' for writing.\n");
            return;
        }
        for (int i = 0; i < count; i++) {
            fprintf(file, "%s|%s|%s|%s|%s|%s|%s\n", 
                    members[i].member_id, 
                    members[i].name, 
                    members[i].contact, 
                    members[i].email, 
                    members[i].membership_plan, 
                    members[i].workout_log,
                    members[i].username);
        }
        fclose(file);
        printf("Profile updated successfully.\n");
    } else {
        printf("Your member record could not be found.\n");
    }
}

// Allows a member to book a class
void book_class(const char* username) {
    char class_id[10];
    printf("\n--- Book a Class ---\n");
    view_all_classes(); // Show the schedule first
    printf("Enter the Class ID to book: ");
    scanf("%s", class_id);
    clear_input_buffer();
    
    printf("Booking class %s for user %s...\n", class_id, username);
    // In a real system, this would write a booking record to a file.
    printf("Class booking simulated successfully!\n");
}

// Displays a member's own receipts
void view_my_receipts(const char* username) {
    FILE* members_file = fopen("members.txt", "r");
    if (!members_file) {
        printf("Member records not found.\n");
        return;
    }

    char member_id[10] = "";
    Member temp_member;
    while (fscanf(members_file, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n", 
                  temp_member.member_id, temp_member.name, temp_member.contact, 
                  temp_member.email, temp_member.membership_plan, temp_member.workout_log, temp_member.username) != EOF) {
        if (strcmp(temp_member.username, username) == 0) {
            strcpy(member_id, temp_member.member_id);
            break;
        }
    }
    fclose(members_file);

    if (strlen(member_id) == 0) {
        printf("Your member record could not be found.\n");
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
                  receipt.member_id, 
                  receipt.date, 
                  receipt.service, 
                  &receipt.amount) != EOF) {
        if (strcmp(receipt.member_id, member_id) == 0) {
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

// Allows a member to add a new workout entry
void log_workout(const char* username) {
    Member members[100];
    int count = 0;

    FILE* file = fopen("members.txt", "r");
    if (!file) {
        printf("No member records found.\n");
        return;
    }
    
    while (count < 100 && fscanf(file, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n", 
                                  members[count].member_id, 
                                  members[count].name, 
                                  members[count].contact, 
                                  members[count].email, 
                                  members[count].membership_plan, 
                                  members[count].workout_log,
                                  members[count].username) != EOF) {
        count++;
    }
    fclose(file);
    
    int found_index = -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(members[i].username, username) == 0) {
            found_index = i;
            break;
        }
    }

    if (found_index != -1) {
        char new_workout[100];
        printf("\n--- Log a Workout ---\n");
        printf("Enter your workout details (e.g., 'Legs, 3x10 squats'): ");
        fgets(new_workout, sizeof(new_workout), stdin);
        new_workout[strcspn(new_workout, "\n")] = 0;

        // Append to the existing log
        strcat(members[found_index].workout_log, "\n- ");
        strcat(members[found_index].workout_log, new_workout);
        
        // Rewrite the file
        file = fopen("members.txt", "w");
        if (!file) {
            printf("Error: Could not open 'members.txt' for writing.\n");
            return;
        }
        for (int i = 0; i < count; i++) {
            fprintf(file, "%s|%s|%s|%s|%s|%s|%s\n", 
                    members[i].member_id, 
                    members[i].name, 
                    members[i].contact, 
                    members[i].email, 
                    members[i].membership_plan, 
                    members[i].workout_log,
                    members[i].username);
        }
        fclose(file);
        printf("Workout logged successfully.\n");
    } else {
        printf("Your member record could not be found.\n");
    }
}

// Displays a member's workout log
void view_my_workout_log(const char* username) {
    FILE* file = fopen("members.txt", "r");
    if (!file) {
        printf("No member records found.\n");
        return;
    }
    
    Member member;
    int found = 0;
    while (fscanf(file, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n", 
                  member.member_id, 
                  member.name, 
                  member.contact, 
                  member.email, 
                  member.membership_plan, 
                  member.workout_log,
                  member.username) != EOF) {
        if (strcmp(member.username, username) == 0) {
            printf("\n--- Your Workout Log ---\n");
            printf("%s\n", member.workout_log);
            found = 1;
            break;
        }
    }
    fclose(file);
    if (!found) {
        printf("Your member record could not be found.\n");
    }
}

// FIX: Added the missing function definition for view_class_schedule()
void view_class_schedule() {
    view_all_classes();
}

// --- Helper Function ---
// Clears the input buffer to prevent issues with subsequent scanf calls.
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
