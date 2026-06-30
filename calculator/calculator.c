#include <stdio.h>
#include <stdlib.h>
// The following line is added to ensure M_PI is defined in math.h
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>
#include <ctype.h>

// --- Preprocessor Directives and Global Variables ---

// Define the maximum number of history entries
#define MAX_HISTORY 10

// Define the clear screen command based on the operating system
#ifdef _WIN32
#define CLEAR_SCREEN "cls"
#else
#define CLEAR_SCREEN "clear"
#endif

// A simple structure to hold a history entry as a string
typedef struct {
    char entry[100];
} History;

History history[MAX_HISTORY];
int history_count = 0;
int history_index = 0; // The current index to write to in the circular buffer

int precision = 2; // Default precision for output

// --- Function Prototypes ---

void clearScreen();
void displayMenu();
void addHistory(char* entry);
void showHistory();
void handleBasicOperations();
void handleScientificOperations();
void setPrecision();
int isValidNumber(const char* str);
int getValidDouble(const char* prompt, double* num);

// --- Main Function ---

int main() {
    int choice;
    do {
        clearScreen();
        displayMenu();

        if (scanf("%d", &choice) != 1) {
            // Clear the input buffer if scanf fails
            while (getchar() != '\n');
            choice = 0; // Set to an invalid choice to trigger the loop
        }

        switch (choice) {
            case 1:
                handleBasicOperations();
                break;
            case 2:
                handleScientificOperations();
                break;
            case 3:
                showHistory();
                break;
            case 4:
                setPrecision();
                break;
            case 5:
                printf("Exiting the calculator. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
        if (choice != 5) {
            printf("\nPress Enter to continue...");
            while (getchar() != '\n'); // Clear any remaining newline
            getchar(); // Wait for user to press Enter
        }
    } while (choice != 5);

    return 0;
}

// --- Function Implementations ---

/**
 * @brief Clears the console screen.
 */
void clearScreen() {
    system(CLEAR_SCREEN);
}

/**
 * @brief Displays the main menu of the calculator.
 */
void displayMenu() {
    printf("========================================\n");
    printf("             C Calculator\n");
    printf("========================================\n");
    printf("1. Basic Operations (+, -, *, /, %%)\n");
    printf("2. Scientific Operations\n");
    printf("3. View History\n");
    printf("4. Set Output Precision\n");
    printf("5. Exit\n");
    printf("----------------------------------------\n");
    printf("Current precision: %d decimal places\n", precision);
    printf("Enter your choice: ");
}

/**
 * @brief Adds a new calculation entry to the history log.
 * @param entry The string representing the calculation.
 */
void addHistory(char* entry) {
    snprintf(history[history_index].entry, sizeof(history[history_index].entry), "%s", entry);
    history_index = (history_index + 1) % MAX_HISTORY;
    if (history_count < MAX_HISTORY) {
        history_count++;
    }
}

/**
 * @brief Displays the calculation history.
 */
void showHistory() {
    clearScreen();
    printf("========================================\n");
    printf("           Calculation History\n");
    printf("========================================\n");

    if (history_count == 0) {
        printf("No calculations in history yet.\n");
        return;
    }

    int start_index = (history_count < MAX_HISTORY) ? 0 : history_index;
    for (int i = 0; i < history_count; i++) {
        int index = (start_index + i) % MAX_HISTORY;
        printf("%d. %s\n", i + 1, history[index].entry);
    }
}

/**
 * @brief Handles basic arithmetic operations.
 */
void handleBasicOperations() {
    int choice;
    double num1, num2, result;
    char history_entry[100];
    do {
        clearScreen();
        printf("--- Basic Operations ---\n");
        printf("1. Addition\n2. Subtraction\n3. Multiplication\n4. Division\n5. Modulus\n6. Back to main menu\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input.\n");
            continue; // Go back to the start of the loop
        }
        
        while (getchar() != '\n'); // Clear the input buffer

        if (choice < 1 || choice > 6) {
            printf("Invalid choice. Please try again.\n");
            continue; // Go back to the start of the loop
        }

        if (choice == 6) break;

        if (!getValidDouble("Enter first number: ", &num1)) continue;
        if (!getValidDouble("Enter second number: ", &num2)) continue;

        switch (choice) {
            case 1:
                result = num1 + num2;
                snprintf(history_entry, sizeof(history_entry), "%.*f + %.*f = %.*f", precision, num1, precision, num2, precision, result);
                printf("%s\n", history_entry);
                addHistory(history_entry);
                break;
            case 2:
                result = num1 - num2;
                snprintf(history_entry, sizeof(history_entry), "%.*f - %.*f = %.*f", precision, num1, precision, num2, precision, result);
                printf("%s\n", history_entry);
                addHistory(history_entry);
                break;
            case 3:
                result = num1 * num2;
                snprintf(history_entry, sizeof(history_entry), "%.*f * %.*f = %.*f", precision, num1, precision, num2, precision, result);
                printf("%s\n", history_entry);
                addHistory(history_entry);
                break;
            case 4:
                if (num2 == 0) {
                    printf("Error: Division by zero is not allowed.\n");
                } else {
                    result = num1 / num2;
                    snprintf(history_entry, sizeof(history_entry), "%.*f / %.*f = %.*f", precision, num1, precision, num2, precision, result);
                    printf("%s\n", history_entry);
                    addHistory(history_entry);
                }
                break;
            case 5:
                if (num2 == 0) {
                    printf("Error: Modulus by zero is not allowed.\n");
                } else {
                    long long int_num1 = (long long)num1;
                    long long int_num2 = (long long)num2;
                    long long int_result = int_num1 % int_num2;
                    snprintf(history_entry, sizeof(history_entry), "%lld %% %lld = %lld", int_num1, int_num2, int_result);
                    printf("%s\n", history_entry);
                    addHistory(history_entry);
                }
                break;
        }
        printf("\nPress Enter to continue with basic operations...");
        getchar();
    } while (choice != 6);
}

/**
 * @brief Handles scientific and advanced mathematical operations.
 */
void handleScientificOperations() {
    int choice;
    double num, result, num2;
    char history_entry[100];

    do {
        clearScreen();
        printf("--- Scientific Operations ---\n");
        printf("1. Exponents (x^y)\n2. Square Root\n3. Factorial\n4. Absolute Value\n");
        printf("5. Ceiling\n6. Floor\n7. Sine\n8. Cosine\n9. Tangent\n10. Natural Logarithm (ln)\n11. Base-10 Logarithm (log)\n12. Back to main menu\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input.\n");
            continue; // Go back to the start of the loop
        }

        while (getchar() != '\n'); // Clear the input buffer

        if (choice < 1 || choice > 12) {
            printf("Invalid choice. Please try again.\n");
            continue; // Go back to the start of the loop
        }

        if (choice == 12) break;

        if (choice == 1) { // Exponents require two numbers
            if (!getValidDouble("Enter base number: ", &num)) continue;
            if (!getValidDouble("Enter exponent: ", &num2)) continue;
            result = pow(num, num2);
            snprintf(history_entry, sizeof(history_entry), "pow(%.*f, %.*f) = %.*f", precision, num, precision, num2, precision, result);
            printf("%s\n", history_entry);
            addHistory(history_entry);
        } else { // Most other functions require one number
            if (!getValidDouble("Enter a number: ", &num)) continue;

            switch (choice) {
                case 2:
                    if (num < 0) {
                        printf("Error: Cannot calculate the square root of a negative number.\n");
                    } else {
                        result = sqrt(num);
                        snprintf(history_entry, sizeof(history_entry), "sqrt(%.*f) = %.*f", precision, num, precision, result);
                        printf("%s\n", history_entry);
                        addHistory(history_entry);
                    }
                    break;
                case 3:
                    if (num < 0 || num != (int)num) {
                        printf("Error: Factorial is only defined for non-negative integers.\n");
                    } else {
                        long long fact_result = 1;
                        for (int i = 1; i <= num; i++) {
                            fact_result *= i;
                        }
                        snprintf(history_entry, sizeof(history_entry), "factorial(%.0f) = %lld", num, fact_result);
                        printf("%s\n", history_entry);
                        addHistory(history_entry);
                    }
                    break;
                case 4:
                    result = fabs(num);
                    snprintf(history_entry, sizeof(history_entry), "abs(%.*f) = %.*f", precision, num, precision, result);
                    printf("%s\n", history_entry);
                    addHistory(history_entry);
                    break;
                case 5:
                    result = ceil(num);
                    snprintf(history_entry, sizeof(history_entry), "ceil(%.*f) = %.*f", precision, num, precision, result);
                    printf("%s\n", history_entry);
                    addHistory(history_entry);
                    break;
                case 6:
                    result = floor(num);
                    snprintf(history_entry, sizeof(history_entry), "floor(%.*f) = %.*f", precision, num, precision, result);
                    printf("%s\n", history_entry);
                    addHistory(history_entry);
                    break;
                case 7:
                    result = sin(num * M_PI / 180.0); // Convert degrees to radians
                    snprintf(history_entry, sizeof(history_entry), "sin(%.*f) = %.*f", precision, num, precision, result);
                    printf("%s\n", history_entry);
                    addHistory(history_entry);
                    break;
                case 8:
                    result = cos(num * M_PI / 180.0); // Convert degrees to radians
                    snprintf(history_entry, sizeof(history_entry), "cos(%.*f) = %.*f", precision, num, precision, result);
                    printf("%s\n", history_entry);
                    addHistory(history_entry);
                    break;
                case 9:
                    if (fmod(num, 90.0) == 0.0 && fmod((int)num / 90.0, 2) != 0) {
                        printf("Error: Tangent is undefined for this angle.\n");
                    } else {
                        result = tan(num * M_PI / 180.0); // Convert degrees to radians
                        snprintf(history_entry, sizeof(history_entry), "tan(%.*f) = %.*f", precision, num, precision, result);
                        printf("%s\n", history_entry);
                        addHistory(history_entry);
                    }
                    break;
                case 10:
                    if (num <= 0) {
                        printf("Error: Natural logarithm is only defined for positive numbers.\n");
                    } else {
                        result = log(num);
                        snprintf(history_entry, sizeof(history_entry), "ln(%.*f) = %.*f", precision, num, precision, result);
                        printf("%s\n", history_entry);
                        addHistory(history_entry);
                    }
                    break;
                case 11:
                    if (num <= 0) {
                        printf("Error: Base-10 logarithm is only defined for positive numbers.\n");
                    } else {
                        result = log10(num);
                        snprintf(history_entry, sizeof(history_entry), "log(%.*f) = %.*f", precision, num, precision, result);
                        printf("%s\n", history_entry);
                        addHistory(history_entry);
                    }
                    break;
            }
        }
        printf("\nPress Enter to continue with scientific operations...");
        getchar();
    } while (choice != 12);
}

/**
 * @brief Allows the user to set the output precision.
 */
void setPrecision() {
    clearScreen();
    int new_precision;
    printf("========================================\n");
    printf("         Set Output Precision\n");
    printf("========================================\n");
    printf("Enter the number of decimal places (0-10): ");

    if (scanf("%d", &new_precision) != 1 || new_precision < 0 || new_precision > 10) {
        while (getchar() != '\n');
        printf("Invalid input. Precision must be an integer between 0 and 10.\n");
    } else {
        precision = new_precision;
        printf("Precision updated to %d decimal places.\n", precision);
    }
}

/**
 * @brief Checks if a string represents a valid number.
 * @param str The string to check.
 * @return 1 if valid, 0 otherwise.
 */
int isValidNumber(const char* str) {
    int decimal_point_count = 0;
    if (*str == '-') str++; // Skip a leading minus sign
    if (!isdigit(*str) && *str != '.') return 0; // Check the first character
    while (*str) {
        if (*str == '.') {
            decimal_point_count++;
            if (decimal_point_count > 1) return 0;
        } else if (!isdigit(*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}

/**
 * @brief Prompts the user for a double and validates the input.
 * @param prompt The message to display to the user.
 * @param num A pointer to the variable where the number will be stored.
 * @return 1 on success, 0 on failure.
 */
int getValidDouble(const char* prompt, double* num) {
    char input[50];
    printf("%s", prompt);
    if (scanf("%s", input) != 1) {
        while (getchar() != '\n');
        printf("Error: Invalid input format. Please enter a valid number.\n");
        return 0;
    }
    while (getchar() != '\n'); // Clear the input buffer

    if (!isValidNumber(input)) {
        printf("Error: Invalid input. Please enter a valid number.\n");
        return 0;
    }

    *num = atof(input);
    return 1;
}
