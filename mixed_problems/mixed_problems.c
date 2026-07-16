#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Structure to store history
typedef struct {
    int choice;
    char details[100];
} History;

History history[100];
int historyCount = 0;

// Function to store history
void addHistory(int choice, const char* details) {
    if (historyCount < 100) {
        history[historyCount].choice = choice;
        strcpy(history[historyCount].details, details);
        historyCount++;
    }
}

// Function to show history
void showHistory() {
    printf("\n--- History ---\n");
    printf("+----------------------------------------------------+\n");
    printf("| Option             | Details                       |\n");
    printf("+----------------------------------------------------+\n");
    for (int i = 0; i < historyCount; i++) {
        char optionName[50];
        switch (history[i].choice) {
            case 1:
                strcpy(optionName, "Factorial");
                break;
            case 2:
                strcpy(optionName, "Prime Check");
                break;
            case 3:
                strcpy(optionName, "Fibonacci");
                break;
            case 4:
                strcpy(optionName, "GCD/LCM");
                break;
            case 5:
                strcpy(optionName, "Reverse String");
                break;
            case 6:
                strcpy(optionName, "Palindrome Check");
                break;
            case 7:
                strcpy(optionName, "Count Vowels/Cons.");
                break;
            case 8:
                strcpy(optionName, "Swap Numbers");
                break;
            default:
                strcpy(optionName, "Unknown");
                break;
        }
        printf("| %-18s | %-11s |\n", optionName, history[i].details);
    }
    printf("+----------------------------------+\n");
}

// Function prototypes for menu options
void factorial();
void primeNumber();
void fibonacciSeries();
void gcdLcm();
void reverseString();
void palindromeChecker();
void countVowelsConsonants();
void swapNumbers();

int main() {
    int choice;
    do {
        printf("\n--- Menu ---\n");
        printf("1. Factorial of a Number\n");
        printf("2. Check Prime Number\n");
        printf("3. Generate Fibonacci Series\n");
        printf("4. Find GCD and LCM\n");
        printf("5. Reverse a String\n");
        printf("6. Palindrome Checker\n");
        printf("7. Count Vowels and Consonants in a String\n");
        printf("8. Swap Two Numbers\n");
        printf("9. Show History\n");
        printf("10. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                factorial();
                break;
            case 2:
                primeNumber();
                break;
            case 3:
                fibonacciSeries();
                break;
            case 4:
                gcdLcm();
                break;
            case 5:
                reverseString();
                break;
            case 6:
                palindromeChecker();
                break;
            case 7:
                countVowelsConsonants();
                break;
            case 8:
                swapNumbers();
                break;
            case 9:
                showHistory();
                break;
            case 10:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice! Please enter a number between 1 and 10.\n");
        }
    } while (choice != 10);

    return 0;
}

void factorial() {
    int n, i;
    long long fact = 1;
    char details[100];
    printf("Enter a non-negative integer: ");
    scanf("%d", &n);
    if (n < 0) {
        printf("Factorial of a negative number doesn't exist.\n");
        sprintf(details, "Input: %d, Result: N/A", n);
    } else {
        for (i = 1; i <= n; i++) {
            fact *= i;
        }
        printf("Factorial of %d is %lld\n", n, fact);
        sprintf(details, "Input: %d, Result: %lld", n, fact);
    }
    addHistory(1, details);
}

void primeNumber() {
    int n, i, isPrime = 1;
    char details[100];
    printf("Enter a positive integer: ");
    scanf("%d", &n);
    if (n <= 1) {
        isPrime = 0;
    } else {
        for (i = 2; i <= n / 2; i++) {
            if (n % i == 0) {
                isPrime = 0;
                break;
            }
        }
    }
    if (isPrime) {
        printf("%d is a prime number.\n", n);
        sprintf(details, "Input: %d, Result: Prime", n);
    } else {
        printf("%d is not a prime number.\n", n);
        sprintf(details, "Input: %d, Result: Not Prime", n);
    }
    addHistory(2, details);
}

void fibonacciSeries() {
    int n, i;
    long long t1 = 0, t2 = 1, nextTerm;
    char details[100];
    printf("Enter the number of terms: ");
    scanf("%d", &n);
    printf("Fibonacci Series: ");
    for (i = 1; i <= n; i++) {
        printf("%lld, ", t1);
        nextTerm = t1 + t2;
        t1 = t2;
        t2 = nextTerm;
    }
    printf("\n");
    sprintf(details, "Terms: %d", n);
    addHistory(3, details);
}

void gcdLcm() {
    int n1, n2, i, gcd, lcm;
    char details[100];
    printf("Enter two positive integers: ");
    scanf("%d %d", &n1, &n2);
    for (i = 1; i <= n1 && i <= n2; ++i) {
        if (n1 % i == 0 && n2 % i == 0)
            gcd = i;
    }
    lcm = (n1 * n2) / gcd;
    printf("GCD of %d and %d is %d\n", n1, n2, gcd);
    printf("LCM of %d and %d is %d\n", n1, n2, lcm);
    sprintf(details, "N1: %d, N2: %d, GCD: %d, LCM: %d", n1, n2, gcd, lcm);
    addHistory(4, details);
}

void reverseString() {
    char str[100], revStr[100];
    int len, i, j;
    char details[100];
    printf("Enter a string: ");
    getchar(); // Consume the newline character
    fgets(str, sizeof(str), stdin);
    str[strcspn(str, "\n")] = '\0'; // Remove trailing newline
    len = strlen(str);
    for (i = len - 1, j = 0; i >= 0; i--, j++) {
        revStr[j] = str[i];
    }
    revStr[j] = '\0';
    printf("Original string: %s\n", str);
    printf("Reversed string: %s\n", revStr);
    sprintf(details, "Original: %s, Reversed: %s", str, revStr);
    addHistory(5, details);
}

void palindromeChecker() {
    char str[100], tempStr[100];
    int len, i, j, isPalindrome = 1;
    char details[100];
    printf("Enter a string: ");
    getchar();
    fgets(str, sizeof(str), stdin);
    str[strcspn(str, "\n")] = '\0';
    strcpy(tempStr, str);
    len = strlen(tempStr);
    for (i = 0, j = len - 1; i < j; i++, j--) {
        if (tempStr[i] != tempStr[j]) {
            isPalindrome = 0;
            break;
        }
    }
    if (isPalindrome) {
        printf("'%s' is a palindrome.\n", str);
        sprintf(details, "Input: %s, Result: Palindrome", str);
    } else {
        printf("'%s' is not a palindrome.\n", str);
        sprintf(details, "Input: %s, Result: Not Palindrome", str);
    }
    addHistory(6, details);
}

void countVowelsConsonants() {
    char str[100];
    int vowels = 0, consonants = 0, i;
    char details[100];
    printf("Enter a string: ");
    getchar();
    fgets(str, sizeof(str), stdin);
    str[strcspn(str, "\n")] = '\0';
    for (i = 0; str[i] != '\0'; ++i) {
        if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z')) {
            if (str[i] == 'a' || str[i] == 'e' || str[i] == 'i' || str[i] == 'o' || str[i] == 'u' ||
                str[i] == 'A' || str[i] == 'E' || str[i] == 'I' || str[i] == 'O' || str[i] == 'U') {
                vowels++;
            } else {
                consonants++;
            }
        }
    }
    printf("Vowels: %d, Consonants: %d\n", vowels, consonants);
    sprintf(details, "Vowels: %d, Consonants: %d", vowels, consonants);
    addHistory(7, details);
}

void swapNumbers() {
    int a, b, temp;
    char details[100];
    printf("Enter two numbers: ");
    scanf("%d %d", &a, &b);
    printf("Original values: a = %d, b = %d\n", a, b);

    // Swap using a temporary variable
    temp = a;
    a = b;
    b = temp;
    printf("After swapping (with temp variable): a = %d, b = %d\n", a, b);

    // Reset and swap without a temporary variable
    printf("Re-entering the same numbers for the second swap.\n");
    printf("Original values: a = %d, b = %d\n", b, a);
    a = a + b;
    b = a - b;
    a = a - b;
    printf("After swapping (without temp variable): a = %d, b = %d\n", a, b);
    sprintf(details, "Swapped %d and %d", b, a);
    addHistory(8, details);
}