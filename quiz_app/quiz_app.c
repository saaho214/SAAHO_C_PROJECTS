#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> // For sleep()

// --- Data Structures ---
// A struct to hold user information.
typedef struct {
    char username[50];
    char password[50];
    char role[10]; // "admin" or "user"
} User;

// A struct to hold a single question and its options.
typedef struct {
    char question[256];
    char options[4][100];
    int correct_option; // 0 for A, 1 for B, 2 for C, 3 for D
} Question;

// A struct to hold a quiz result for reporting.
typedef struct {
    char username[50];
    char topic[50];
    int score;
    int total_questions;
    double time_taken;
    char date_str[64];
} QuizResult;


// --- Function Prototypes ---
void login_page();
void admin_mode();
void user_mode(const char* username);
int authenticate(const char* username, const char* password, char* role);
void register_user();
int check_username_exists(const char* username);
void take_quiz(const char* username);
void display_report(const char* username, int score, double time_taken, int total_questions, const char* topic);
void view_all_users();
void view_all_results();
void view_user_details();
void change_user_details();
void set_exam_status(int status);
int get_exam_status();
void set_quiz_and_add_questions();
void get_quiz_settings(int* num_questions, double* time_limit);
void view_my_results(const char* username); // Function to view a user's own results
void clear_input_buffer(); // Helper function to clear input buffer

// --- Main Function ---
int main() {
    // Keep the program running so a user can return to the login page after logging out.
    while (1) {
        login_page();
    }
    return 0;
}

// --- Login Page ---
void login_page() {
    char username[50];
    char password[50];
    char role[10];
    int choice;
    int attempts = 3;

    printf("\n--- Welcome to the Quiz Application ---\n");
    printf("1. Admin Login\n");
    printf("2. User Login\n");
    printf("3. Register\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    
    clear_input_buffer();

    if (choice == 1) {
        strcpy(role, "admin");
    } else if (choice == 2) {
        strcpy(role, "user");
    } else if (choice == 3) {
        register_user();
        return; // Return to main loop which will call login_page() again
    } else {
        printf("Invalid choice. Exiting.\n");
        exit(0);
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
            return; // Return to main loop
        } else {
            attempts--;
            printf("\nAuthentication failed. %d attempts remaining.\n", attempts);
        }
    }
    printf("\nToo many failed attempts. Exiting.\n");
    exit(0);
}

// --- User Registration ---
void register_user() {
    char new_username[50];
    char new_password[50];
    char new_role[10];

    printf("\n--- New User Registration ---\n");
    printf("Enter a new username: ");
    scanf("%s", new_username);
    clear_input_buffer();

    // Check if username already exists
    if (check_username_exists(new_username)) {
        printf("Username '%s' already exists. Please choose a different one.\n", new_username);
        return; // Return to login page
    }

    printf("Enter a password: ");
    scanf("%s", new_password);
    clear_input_buffer();
    
    printf("Enter desired role (admin or user): ");
    scanf("%s", new_role);
    clear_input_buffer();

    // Input validation for role
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

// Check if a username is already in use
int check_username_exists(const char* username) {
    FILE* file = fopen("users.txt", "r");
    if (!file) {
        return 0; // File doesn't exist, so username is available
    }

    User user_data;
    while (fscanf(file, "%s %*s %*s", user_data.username) != EOF) {
        if (strcmp(username, user_data.username) == 0) {
            fclose(file);
            return 1; // Username found
        }
    }
    fclose(file);
    return 0; // Username not found
}

// --- Authentication Logic ---
int authenticate(const char* username, const char* password, char* role) {
    FILE* file = fopen("users.txt", "r");
    if (!file) {
        printf("Error: 'users.txt' file not found. Creating a default admin user.\n");
        file = fopen("users.txt", "w");
        fprintf(file, "admin admin_pass admin\n");
        fprintf(file, "user user_pass user\n");
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

// --- Admin Mode ---
void admin_mode() {
    int choice;
    do {
        printf("\n--- Admin Mode ---\n");
        printf("1. View all candidates (names only)\n");
        printf("2. View all quiz results\n");
        printf("3. View all user details (including passwords)\n");
        printf("4. Change user details\n");
        printf("5. Set exam status (Open/Close)\n");
        printf("6. Set Quiz Questions and Settings\n");
        printf("7. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        clear_input_buffer();

        switch (choice) {
            case 1:
                view_all_users();
                break;
            case 2:
                view_all_results();
                break;
            case 3:
                view_user_details();
                break;
            case 4:
                change_user_details();
                break;
            case 5: {
                int status;
                printf("Enter 1 to open the exam, 0 to close: ");
                scanf("%d", &status);
                set_exam_status(status);
                clear_input_buffer();
                break;
            }
            case 6:
                set_quiz_and_add_questions();
                break;
            case 7:
                printf("Logging out...\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 7);
}

// New: Function for admin to set quiz settings and add questions
void set_quiz_and_add_questions() {
    char topic_file[100];
    int subject_choice, topic_choice;
    int num_questions;
    double time_limit_minutes;
    char question_text[256];
    char options[4][100];
    int correct_option;
    char input_buffer[100];

    printf("\n--- Set Quiz Questions and Settings ---\n");
    printf("1. C Language\n");
    printf("2. Python\n");
    printf("3. Java\n");
    printf("4. C++\n");
    printf("Enter subject choice: ");
    fgets(input_buffer, sizeof(input_buffer), stdin);
    sscanf(input_buffer, "%d", &subject_choice);
    
    printf("\nChoose a topic (enter number 1-7):\n");
    printf("1. C Basics\n");
    printf("2. C Pointers\n");
    printf("Enter topic choice: ");
    fgets(input_buffer, sizeof(input_buffer), stdin);
    sscanf(input_buffer, "%d", &topic_choice);

    switch (subject_choice) {
        case 1:
            if (topic_choice == 1) strcpy(topic_file, "c_topic_1_basics.txt");
            else if (topic_choice == 2) strcpy(topic_file, "c_topic_2_pointers.txt");
            else { printf("Invalid topic choice.\n"); return; }
            break;
        case 2:
        case 3:
        case 4:
            printf("Quizzes for this language are not yet configurable.\n");
            return;
        default:
            printf("Invalid subject choice.\n");
            return;
    }

    printf("Enter the number of questions to add: ");
    fgets(input_buffer, sizeof(input_buffer), stdin);
    sscanf(input_buffer, "%d", &num_questions);
    
    printf("Enter the time limit for the quiz in minutes: ");
    fgets(input_buffer, sizeof(input_buffer), stdin);
    sscanf(input_buffer, "%lf", &time_limit_minutes);

    // Save quiz settings to a file
    FILE* settings_file = fopen("quiz_settings.txt", "w");
    if (settings_file) {
        fprintf(settings_file, "%d %.2lf", num_questions, time_limit_minutes);
        fclose(settings_file);
        printf("Quiz settings saved successfully.\n");
    } else {
        printf("Error saving quiz settings.\n");
    }

    // Open questions file in append mode to add new questions
    FILE* file = fopen(topic_file, "a");
    if (!file) {
        printf("Error: Could not open question file '%s' for writing.\n", topic_file);
        return;
    }
    
    for (int i = 0; i < num_questions; i++) {
        printf("\nAdding Question %d:\n", i + 1);
        printf("Enter the question text (max 255 chars): ");
        fgets(question_text, sizeof(question_text), stdin);
        question_text[strcspn(question_text, "\n")] = 0;

        printf("Enter Option A: ");
        fgets(options[0], sizeof(options[0]), stdin);
        options[0][strcspn(options[0], "\n")] = 0;

        printf("Enter Option B: ");
        fgets(options[1], sizeof(options[1]), stdin);
        options[1][strcspn(options[1], "\n")] = 0;

        printf("Enter Option C: ");
        fgets(options[2], sizeof(options[2]), stdin);
        options[2][strcspn(options[2], "\n")] = 0;

        printf("Enter Option D: ");
        fgets(options[3], sizeof(options[3]), stdin);
        options[3][strcspn(options[3], "\n")] = 0;

        printf("Enter the correct option number (0 for A, 1 for B, 2 for C, 3 for D): ");
        fgets(input_buffer, sizeof(input_buffer), stdin);
        sscanf(input_buffer, "%d", &correct_option);

        fprintf(file, "%s\n%s\n%s\n%s\n%s\n%d\n", question_text, options[0], options[1], options[2], options[3], correct_option);
    }

    fclose(file);
    printf("\nQuestions added to quiz file successfully.\n");
}

// New: Function to get quiz settings from file
void get_quiz_settings(int* num_questions, double* time_limit) {
    FILE* file = fopen("quiz_settings.txt", "r");
    if (file) {
        fscanf(file, "%d %lf", num_questions, time_limit);
        fclose(file);
    } else {
        printf("Quiz settings file not found. Using default values (15 questions, 10 min).\n");
        *num_questions = 15;
        *time_limit = 10.0;
    }
}

// View all registered users.
void view_all_users() {
    FILE* file = fopen("users.txt", "r");
    if (!file) {
        printf("Error: 'users.txt' not found.\n");
        return;
    }

    User user_data;
    printf("\n--- Registered Candidates ---\n");
    printf("%-20s %-10s\n", "Username", "Role");
    printf("----------------------------------\n");
    while (fscanf(file, "%s %*s %s", user_data.username, user_data.role) != EOF) {
        printf("%-20s %-10s\n", user_data.username, user_data.role);
    }
    fclose(file);
}

// New: View all user details, including passwords
void view_user_details() {
    FILE* file = fopen("users.txt", "r");
    if (!file) {
        printf("Error: 'users.txt' not found.\n");
        return;
    }

    User user_data;
    printf("\n--- All User Credentials ---\n");
    printf("%-20s %-20s %-10s\n", "Username", "Password", "Role");
    printf("--------------------------------------------------\n");
    while (fscanf(file, "%s %s %s", user_data.username, user_data.password, user_data.role) != EOF) {
        printf("%-20s %-20s %-10s\n", user_data.username, user_data.password, user_data.role);
    }
    fclose(file);
}

// New: Change user details
void change_user_details() {
    char target_username[50];
    User users[100];
    int count = 0;

    printf("\n--- Change User Details ---\n");
    printf("Enter username to modify: ");
    scanf("%s", target_username);
    clear_input_buffer();

    // Read all users into a temporary array
    FILE* file = fopen("users.txt", "r");
    if (!file) {
        printf("Error: 'users.txt' not found.\n");
        return;
    }
    while (count < 100 && fscanf(file, "%s %s %s", users[count].username, users[count].password, users[count].role) != EOF) {
        count++;
    }
    fclose(file);

    int user_found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(users[i].username, target_username) == 0) {
            user_found = 1;
            printf("User '%s' found. Enter new details.\n", target_username);
            
            // Get new username, password, and role
            char new_username[50];
            printf("Enter new username: ");
            scanf("%s", new_username);
            clear_input_buffer();

            // Check if the new username is already taken by a different user
            if (check_username_exists(new_username) && strcmp(new_username, target_username) != 0) {
                printf("New username '%s' is already in use. Aborting change.\n", new_username);
                return;
            }

            char new_password[50];
            printf("Enter new password: ");
            scanf("%s", new_password);
            clear_input_buffer();
            
            char new_role[10];
            printf("Enter new role (admin or user): ");
            scanf("%s", new_role);
            clear_input_buffer();
            if (strcmp(new_role, "admin") != 0 && strcmp(new_role, "user") != 0) {
                printf("Invalid role. Keeping original role.\n");
                strcpy(new_role, users[i].role);
            }

            // Update details
            strcpy(users[i].username, new_username);
            strcpy(users[i].password, new_password);
            strcpy(users[i].role, new_role);
            
            break;
        }
    }

    if (!user_found) {
        printf("User '%s' not found.\n", target_username);
        return;
    }

    // Rewrite the entire file with updated details
    file = fopen("users.txt", "w");
    if (!file) {
        printf("Error: Could not open 'users.txt' for writing.\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %s %s\n", users[i].username, users[i].password, users[i].role);
    }
    fclose(file);
    printf("User details updated successfully.\n");
}

// View all quiz results.
void view_all_results() {
    FILE* file = fopen("results.txt", "r");
    if (!file) {
        printf("No quiz results found yet.\n");
        return;
    }

    QuizResult result;
    int found_results = 0;

    printf("\n--- All Quiz Results ---\n");
    while (fscanf(file, "%s %s %d %d %lf %s", result.username, result.topic, &result.score, &result.total_questions, &result.time_taken, result.date_str) == 6) {
        double percentage = (double)result.score / result.total_questions * 100.0;
        char status[10];
        if (percentage >= 60.0) {
            strcpy(status, "PASS");
        } else {
            strcpy(status, "FAIL");
        }
        
        printf("\n--- Candidate Report Card ---\n");
        printf("Candidate ID: %s\n", result.username);
        printf("Subject:      %s\n", result.topic);
        printf("Date:         %s\n", result.date_str);
        printf("-----------------------------\n");
        printf("Score:        %d / %d\n", result.score, result.total_questions);
        printf("Performance:  %.2f%%\n", percentage);
        printf("Status:       %s\n", status);
        printf("Time Taken:   %.2f seconds\n", result.time_taken);
        printf("-----------------------------\n");
        found_results = 1;
    }
    fclose(file);
    
    if (!found_results) {
        printf("No results found to display.\n");
    }
}


// Set exam status (open/close).
void set_exam_status(int status) {
    FILE* file = fopen("exam_status.txt", "w");
    if (!file) {
        printf("Error: Could not create 'exam_status.txt'.\n");
        return;
    }
    fprintf(file, "%d", status);
    fclose(file);
    printf("Exam status updated to: %s\n", status ? "OPEN" : "CLOSED");
}

// Get exam status.
int get_exam_status() {
    FILE* file = fopen("exam_status.txt", "r");
    if (!file) {
        return 0; // Default to closed if file not found
    }
    int status;
    fscanf(file, "%d", &status);
    fclose(file);
    return status;
}

// --- User Mode ---
void user_mode(const char* username) {
    int choice;
    do {
        printf("\n--- User Mode ---\n");
        printf("1. Take a Quiz\n");
        printf("2. View My Results\n");
        printf("3. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clear_input_buffer();

        switch (choice) {
            case 1:
                if (get_exam_status() == 1) {
                    take_quiz(username);
                } else {
                    printf("\nThe exam is currently closed by the admin. Please try again later.\n");
                }
                break;
            case 2:
                view_my_results(username);
                break;
            case 3:
                printf("Logging out...\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);
}

// New: View a user's own results
void view_my_results(const char* username) {
    FILE* file = fopen("results.txt", "r");
    if (!file) {
        printf("You have not taken any quizzes yet.\n");
        return;
    }

    QuizResult result;
    int found_results = 0;

    printf("\n--- Your Quiz Results ---\n");
    while (fscanf(file, "%s %s %d %d %lf %s", result.username, result.topic, &result.score, &result.total_questions, &result.time_taken, result.date_str) == 6) {
        if (strcmp(result.username, username) == 0) {
            double percentage = (double)result.score / result.total_questions * 100.0;
            char status[10];
            if (percentage >= 60.0) {
                strcpy(status, "PASS");
            } else {
                strcpy(status, "FAIL");
            }
            
            printf("\n--- Report Card ---\n");
            printf("Candidate ID: %s\n", result.username);
            printf("Subject:      %s\n", result.topic);
            printf("Date:         %s\n", result.date_str);
            printf("---------------------\n");
            printf("Score:        %d / %d\n", result.score, result.total_questions);
            printf("Performance:  %.2f%%\n", percentage);
            printf("Status:       %s\n", status);
            printf("Time Taken:   %.2f seconds\n", result.time_taken);
            printf("---------------------\n");
            found_results = 1;
        }
    }

    fclose(file);
    if (!found_results) {
        printf("You have no quiz results to display.\n");
    }
}


// --- Quiz Logic ---
void take_quiz(const char* username) {
    char topic_file[100];
    int subject_choice, topic_choice;
    int num_questions;
    double time_limit;
    
    // Get quiz settings from file
    get_quiz_settings(&num_questions, &time_limit);

    // Choose subject
    printf("\n--- Take a Quiz ---\n");
    printf("1. C Language\n");
    printf("2. Python\n");
    printf("3. Java\n");
    printf("4. C++\n");
    printf("Enter subject choice: ");
    scanf("%d", &subject_choice);
    clear_input_buffer();
    
    // Choose topic
    printf("\nChoose a topic (enter number 1-7):\n");
    // For this example, we'll only use 2 for C.
    printf("1. C Basics\n");
    printf("2. C Pointers\n");
    printf("Enter topic choice: ");
    scanf("%d", &topic_choice);
    clear_input_buffer();

    char topic_name[50];
    switch (subject_choice) {
        case 1:
            if (topic_choice == 1) {
                strcpy(topic_file, "c_topic_1_basics.txt");
                strcpy(topic_name, "C_Basics");
            }
            else if (topic_choice == 2) {
                strcpy(topic_file, "c_topic_2_pointers.txt");
                strcpy(topic_name, "C_Pointers");
            }
            else { printf("Invalid topic choice.\n"); return; }
            break;
        case 2:
        case 3:
        case 4:
            printf("Quizzes for this language are not yet available.\n");
            return;
        default:
            printf("Invalid subject choice.\n");
            return;
    }

    FILE* file = fopen(topic_file, "r");
    if (!file) {
        printf("Error: Question file '%s' not found.\n", topic_file);
        return;
    }

    Question questions[100]; // Increased size to handle more questions
    int question_count = 0;
    
    // Read questions from file
    while (question_count < num_questions && fscanf(file, " %[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%d\n",
        questions[question_count].question,
        questions[question_count].options[0],
        questions[question_count].options[1],
        questions[question_count].options[2],
        questions[question_count].options[3],
        &questions[question_count].correct_option) == 6) {
        question_count++;
    }
    fclose(file);

    if (question_count == 0) {
        printf("No questions loaded from the file.\n");
        return;
    }

    printf("\nQuiz is starting. You have %.0f minutes.\n", time_limit);
    printf("Press ENTER to continue...");
    getchar(); // Wait for user to press ENTER

    int score = 0;
    clock_t start_time = clock();
    double time_limit_seconds = time_limit * 60.0; // convert to seconds

    for (int i = 0; i < question_count; i++) {
        double elapsed_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
        if (elapsed_time > time_limit_seconds) {
            printf("\nTime's up! The quiz has ended.\n");
            break;
        }

        printf("\n----------------------------------\n");
        printf("Question %d/%d (Time left: %.0f sec)\n", i + 1, question_count, time_limit_seconds - elapsed_time);
        printf("%s\n", questions[i].question);
        printf("A. %s\n", questions[i].options[0]);
        printf("B. %s\n", questions[i].options[1]);
        printf("C. %s\n", questions[i].options[2]);
        printf("D. %s\n", questions[i].options[3]);

        char answer;
        printf("Enter your answer (A, B, C, or D): ");
        scanf(" %c", &answer);
        clear_input_buffer();

        int user_choice;
        if (answer >= 'a' && answer <= 'd') answer -= 32; // Convert to uppercase
        switch (answer) {
            case 'A': user_choice = 0; break;
            case 'B': user_choice = 1; break;
            case 'C': user_choice = 2; break;
            case 'D': user_choice = 3; break;
            default: user_choice = -1; break;
        }

        if (user_choice == questions[i].correct_option) {
            printf("Correct! 🎉\n");
            score++;
        } else {
            printf("Incorrect. The correct answer was: %s\n", questions[i].options[questions[i].correct_option]);
        }
        sleep(1); // Pause for a second for better user experience
    }
    
    double final_time_taken = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    if (final_time_taken > time_limit_seconds) {
        final_time_taken = time_limit_seconds;
    }

    display_report(username, score, final_time_taken, question_count, topic_name);
}

// --- Report Generation ---
void display_report(const char* username, int score, double time_taken, int total_questions, const char* topic) {
    printf("\n\n--- Quiz Report for %s ---\n", username);
    printf("Total Questions: %d\n", total_questions);
    printf("Correct Answers: %d\n", score);
    printf("Incorrect Answers: %d\n", total_questions - score);
    
    double percentage = (float)score / total_questions * 100;
    printf("Your Score: %.2f%%\n", percentage);
    printf("Time Taken: %.2f seconds\n", time_taken);
    
    // Determine and display the status
    char status[10];
    if (percentage >= 60.0) {
        strcpy(status, "PASS");
    } else {
        strcpy(status, "FAIL");
    }
    printf("Status: %s\n", status);
    
    printf("--------------------------------------\n");

    // Save result to file
    FILE* file = fopen("results.txt", "a");
    if (file) {
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        char s[64];
        strftime(s, sizeof(s), "%Y-%m-%d_%H:%M:%S", tm);
        fprintf(file, "%s %s %d %d %.2f %s\n", username, topic, score, total_questions, time_taken, s);
        fclose(file);
    }
}

// Helper function to clear the input buffer
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
