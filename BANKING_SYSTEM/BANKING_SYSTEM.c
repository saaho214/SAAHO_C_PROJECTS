#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ACCOUNTS 100
#define MAX_TRANSACTIONS 500
#define ADMIN_PASSWORD "admin"
#define MIN_ACC_NUM 1000000000 // 10-digit number
#define MAX_ACC_NUM 9999999999 // 10-digit number

// Structure for an account
struct Account {
    long long accountNumber;
    char name[50];
    char password[20];
    double balance;
};

// Structure for a transaction
struct Transaction {
    long long accountNumber;
    char type[15]; // "Deposit", "Withdrawal", "Interest", "Loan"
    double amount;
};

// Global arrays and counters
struct Account accounts[MAX_ACCOUNTS];
int accountCount = 0;

struct Transaction transactions[MAX_TRANSACTIONS];
int transactionCount = 0;

// Function prototypes
void createAccount();
void viewAccountDetails();
void deposit();
void withdraw();
void updateAccount();
void deleteAccount();
void viewAllAccounts();
void viewTransactionHistory();
void calculateInterest();
void applyForLoan();
void adminLogin();

// Helper functions
int searchAccount(long long accNum);
int verifyPassword(long long accNum);
void recordTransaction(long long accNum, const char* type, double amount);
long long generateUniqueAccountNumber();
void deleteTransactionsForAccount(long long accNum);
void viewTotalFunds();
void adminMenu();

int main() {
    int choice;
    srand(time(NULL)); // Seed the random number generator once

    do {
        printf("\n\n+---------------------------------------------------+\n");
        printf("|              Simple Banking System                |\n");
        printf("+---------------------------------------------------+\n");
        printf("| 1. Create New Account                             |\n");
        printf("| 2. View Account Details                           |\n");
        printf("| 3. Deposit Money                                  |\n");
        printf("| 4. Withdraw Money                                 |\n");
        printf("| 5. Update Account Name                            |\n");
        printf("| 6. Delete Account                                 |\n");
        printf("| 7. View All Accounts                              |\n");
        printf("| 8. View Transaction History                       |\n");
        printf("| 9. Calculate Interest                             |\n");
        printf("| 10. Apply for a Loan                              |\n");
        printf("| 11. Admin Login                                   |\n");
        printf("| 12. Exit                                          |\n");
        printf("+---------------------------------------------------+\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createAccount(); break;
            case 2: viewAccountDetails(); break;
            case 3: deposit(); break;
            case 4: withdraw(); break;
            case 5: updateAccount(); break;
            case 6: deleteAccount(); break;
            case 7: viewAllAccounts(); break;
            case 8: viewTransactionHistory(); break;
            case 9: calculateInterest(); break;
            case 10: applyForLoan(); break;
            case 11: adminLogin(); break;
            case 12: printf("Exiting program. Goodbye!\n"); break;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 12);

    return 0;
}

// Helper: Find an account by number and return its index
int searchAccount(long long accNum) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accNum) {
            return i;
        }
    }
    return -1;
}

// Helper: Verify password for an account
int verifyPassword(long long accNum) {
    char password[20];
    printf("password  is : " , ADMIN_PASSWORD);
    printf("Enter password for account %lld: ", accNum);
    scanf("%s", password);
    int index = searchAccount(accNum);
    if (index != -1 && strcmp(accounts[index].password, password) == 0) {
        return 1;
    }
    printf("Incorrect password.\n");
    return 0;
}

// Helper: Record a transaction
void recordTransaction(long long accNum, const char* type, double amount) {
    if (transactionCount < MAX_TRANSACTIONS) {
        transactions[transactionCount].accountNumber = accNum;
        strcpy(transactions[transactionCount].type, type);
        transactions[transactionCount].amount = amount;
        transactionCount++;
    } else {
        printf("Transaction history is full. Cannot record new transaction.\n");
    }
}

// Helper: Generate a unique 10-digit account number
long long generateUniqueAccountNumber() {
    long long newAccNum;
    do {
        newAccNum = (long long)rand() * (RAND_MAX + 1) + rand();
        if (newAccNum < MIN_ACC_NUM) {
            newAccNum = MIN_ACC_NUM + (newAccNum % (MAX_ACC_NUM - MIN_ACC_NUM + 1));
        }
    } while (searchAccount(newAccNum) != -1);
    return newAccNum;
}

// Helper: Delete all transactions for a specific account
void deleteTransactionsForAccount(long long accNum) {
    int i, j;
    for (i = 0; i < transactionCount; i++) {
        if (transactions[i].accountNumber == accNum) {
            // Shift remaining transactions to fill the gap
            for (j = i; j < transactionCount - 1; j++) {
                transactions[j] = transactions[j + 1];
            }
            transactionCount--;
            i--; // Recheck the current index as it's now a new transaction
        }
    }
}

// 1. Create a new account with a random number
void createAccount() {
    if (accountCount >= MAX_ACCOUNTS) {
        printf("Maximum number of accounts reached.\n");
        return;
    }

    long long newAccNum = generateUniqueAccountNumber();

    accounts[accountCount].accountNumber = newAccNum;
    printf("\nYour new account number is: %lld\n", newAccNum);

    printf("Enter Account Holder Name: ");
    scanf(" %[^\n]", accounts[accountCount].name);

    printf("Enter a password for the account: ");
    scanf("%s", accounts[accountCount].password);

    printf("Enter Initial Deposit: ");
    scanf("%lf", &accounts[accountCount].balance);

    if (accounts[accountCount].balance < 0) {
        printf("Initial deposit cannot be negative. Account creation failed.\n");
        return;
    }

    accountCount++;
    recordTransaction(newAccNum, "Deposit", accounts[accountCount - 1].balance);
    printf("Account created successfully!\n");
}

// 2. View details of a specific account in a receipt-like format
void viewAccountDetails() {
    long long accNum;
    printf("\n--- View Account Details ---\n");
    printf("Enter Account Number: ");
    scanf("%lld", &accNum);

    int index = searchAccount(accNum);
    if (index != -1) {
        printf("\n+---------------------------------------------------+\n");
        printf("|                  Account Statement                |\n");
        printf("+---------------------------------------------------+\n");
        printf("| Account Number:   %-32lld|\n", accounts[index].accountNumber);
        printf("| Account Holder:   %-32s|\n", accounts[index].name);
        printf("| Current Balance:  $%-31.2lf|\n", accounts[index].balance);
        printf("+---------------------------------------------------+\n");
    } else {
        printf("Account not found.\n");
    }
}

// 3. Deposit money into an account
void deposit() {
    long long accNum;
    double amount;
    printf("\n--- Deposit Money ---\n");
    printf("Enter Account Number: ");
    scanf("%lld", &accNum);

    int index = searchAccount(accNum);
    if (index != -1 && verifyPassword(accNum)) {
        printf("Enter amount to deposit: ");
        scanf("%lf", &amount);
        if (amount > 0) {
            accounts[index].balance += amount;
            recordTransaction(accNum, "Deposit", amount);
            printf("Deposit successful. New balance: $%.2lf\n", accounts[index].balance);
        } else {
            printf("Invalid amount. Please enter a positive value.\n");
        }
    } else if (index == -1) {
        printf("Account not found.\n");
    }
}

// 4. Withdraw money from an account
void withdraw() {
    long long accNum;
    double amount;
    printf("\n--- Withdraw Money ---\n");
    printf("Enter Account Number: ");
    scanf("%lld", &accNum);

    int index = searchAccount(accNum);
    if (index != -1 && verifyPassword(accNum)) {
        printf("Enter amount to withdraw: ");
        scanf("%lf", &amount);
        if (amount > 0 && amount <= accounts[index].balance) {
            accounts[index].balance -= amount;
            recordTransaction(accNum, "Withdrawal", amount);
            printf("Withdrawal successful. New balance: $%.2lf\n", accounts[index].balance);
        } else if (amount <= 0) {
            printf("Invalid amount. Please enter a positive value.\n");
        } else {
            printf("Insufficient balance. Cannot withdraw.\n");
        }
    } else if (index == -1) {
        printf("Account not found.\n");
    }
}

// 5. Update the name on an account
void updateAccount() {
    long long accNum;
    printf("\n--- Update Account Name ---\n");
    printf("Enter Account Number: ");
    scanf("%lld", &accNum);

    int index = searchAccount(accNum);
    if (index != -1 && verifyPassword(accNum)) {
        printf("Enter new name for the account holder: ");
        scanf(" %[^\n]", accounts[index].name);
        printf("Account updated successfully.\n");
    } else if (index == -1) {
        printf("Account not found.\n");
    }
}

// 6. Delete an account and its transactions
void deleteAccount() {
    long long accNum;
    printf("\n--- Delete Account ---\n");
    printf("Enter Account Number to delete: ");
    scanf("%lld", &accNum);

    int index = searchAccount(accNum);
    if (index != -1 && verifyPassword(accNum)) {
        // First, delete all transactions associated with this account
        deleteTransactionsForAccount(accNum);

        // Then, shift remaining accounts to fill the gap
        for (int i = index; i < accountCount - 1; i++) {
            accounts[i] = accounts[i+1];
        }
        accountCount--;
        printf("Account %lld and all associated data deleted successfully.\n", accNum);
    } else if (index == -1) {
        printf("Account not found.\n");
    }
}

// 7. View all accounts in a tabular format
void viewAllAccounts() {
    if (accountCount == 0) {
        printf("\nNo accounts in the system.\n");
        return;
    }

    printf("\n+-----------------------------------------------------------------+\n");
    printf("|                      All Accounts in the System                   |\n");
    printf("+-----------------+--------------------+--------------------------+\n");
    printf("| Account Number  | Account Holder     | Balance                  |\n");
    printf("+-----------------+--------------------+--------------------------+\n");
    for (int i = 0; i < accountCount; i++) {
        printf("| %-15lld | %-18s | $%-24.2lf|\n",
               accounts[i].accountNumber, accounts[i].name, accounts[i].balance);
    }
    printf("+-----------------+--------------------+--------------------------+\n");
}

// 8. View transaction history for a specific account
void viewTransactionHistory() {
    long long accNum;
    printf("\n--- View Transaction History ---\n");
    printf("Enter Account Number: ");
    scanf("%lld", &accNum);

    printf("\n+-----------------------------------+\n");
    printf("|     Transactions for Account %lld    |\n", accNum);
    printf("+-----------------+-----------------+\n");
    printf("| Type            | Amount          |\n");
    printf("+-----------------+-----------------+\n");

    int found = 0;
    for (int i = 0; i < transactionCount; i++) {
        if (transactions[i].accountNumber == accNum) {
            printf("| %-15s | $%-13.2lf |\n", transactions[i].type, transactions[i].amount);
            found = 1;
        }
    }
    if (!found) {
        printf("| No transactions found for this account.   |\n");
    }
    printf("+-----------------+-----------------+\n");
}

// 9. Calculate and add interest
void calculateInterest() {
    long long accNum;
    double rate;
    printf("\n--- Calculate Interest ---\n");
    printf("Enter Account Number: ");
    scanf("%lld", &accNum);

    int index = searchAccount(accNum);
    if (index != -1) {
        printf("Enter annual interest rate (e.g., 0.05 for 5%%): ");
        scanf("%lf", &rate);
        if (rate > 0) {
            double interest = accounts[index].balance * rate;
            accounts[index].balance += interest;
            recordTransaction(accNum, "Interest", interest);
            printf("Interest of $%.2lf added. New balance: $%.2lf\n", interest, accounts[index].balance);
        } else {
            printf("Invalid interest rate.\n");
        }
    } else {
        printf("Account not found.\n");
    }
}

// 10. Apply for a loan
void applyForLoan() {
    long long accNum;
    double loanAmount;
    printf("\n--- Loan Application ---\n");
    printf("Enter Account Number: ");
    scanf("%lld", &accNum);

    int index = searchAccount(accNum);
    if (index != -1) {
        printf("Enter desired loan amount: ");
        scanf("%lf", &loanAmount);

        if (loanAmount > 0 && loanAmount <= accounts[index].balance * 5) {
            accounts[index].balance += loanAmount;
            recordTransaction(accNum, "Loan", loanAmount);
            printf("Loan of $%.2lf approved and deposited. New balance: $%.2lf\n", loanAmount, accounts[index].balance);
        } else if (loanAmount <= 0) {
            printf("Invalid loan amount.\n");
        } else {
            printf("Loan application denied. Loan amount exceeds eligibility criteria.\n");
        }
    } else {
        printf("Account not found.\n");
    }
}

// 11. Admin login and menu
void adminLogin() {
    char password[20];
    printf("\n--- Admin Login ---\n");
    printf("Enter admin password: ");
    scanf("%s", password);
    if (strcmp(password, ADMIN_PASSWORD) == 0) {
        adminMenu();
    } else {
        printf("Incorrect admin password.\n");
    }
}

void adminMenu() {
    int adminChoice;
    do {
        printf("\n\n+---------------------------+\n");
        printf("|        Admin Menu        |\n");
        printf("+---------------------------+\n");
        printf("| 1. View All Accounts      |\n");
        printf("| 2. View Total Bank Funds  |\n");
        printf("| 3. Back to Main Menu      |\n");
        printf("+---------------------------+\n");
        printf("Enter your choice: ");
        scanf("%d", &adminChoice);
        switch (adminChoice) {
            case 1: viewAllAccounts(); break;
            case 2: viewTotalFunds(); break;
            case 3: printf("Returning to main menu.\n"); break;
            default: printf("Invalid choice.\n"); break;
        }
    } while (adminChoice != 3);
}

void viewTotalFunds() {
    double total = 0;
    for (int i = 0; i < accountCount; i++) {
        total += accounts[i].balance;
    }
    printf("\nTotal funds in all bank accounts: $%.2lf\n", total);
}