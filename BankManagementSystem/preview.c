#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

// --- Data Structure Definitions ---
#define MAX_RECORDS 100
#define MAX_TRANSACTIONS 500

// Main user structure for authentication and roles
typedef struct {
    int id;
    char username[50];
    char password[50];
    char role[20]; // "customer" or "admin"
} User;

// Structure for a bank account
typedef struct {
    int id;
    int userId; // Links to the User who owns this account
    char accountType[20]; // e.g., "savings", "current"
    double balance;
} Account;

// Structure for a loan
typedef struct {
    int id;
    int accountId;
    double principal;
    double interestRate;
    double remainingBalance;
} Loan;

// Structure for a transaction
typedef struct {
    int transactionId;
    int accountId;
    char type[20]; // "deposit", "withdrawal", "transfer"
    double amount;
    char date[20];
} Transaction;

// Structure for an audit trail entry
typedef struct {
    int logId;
    char timestamp[30];
    char action[100]; // Describes the action taken
} AuditLog;

// --- Global Arrays and Counters ---
User users[MAX_RECORDS];
Account accounts[MAX_RECORDS];
Loan loans[MAX_RECORDS];
Transaction transactions[MAX_TRANSACTIONS];
AuditLog auditLogs[MAX_RECORDS];

int userCount = 0;
int accountCount = 0;
int loanCount = 0;
int transactionCount = 0;
int auditLogCount = 0;

// --- Function Prototypes ---
void loadData();
void saveData();
void clearInputBuffer();
void logAction(char* action);

// Main menu and login
void mainMenu();
void login();
void registration();
void adminMenu();
void customerMenu(int userId);

// Admin functions
void manageAccounts();
void manageLoans();
void viewAutomatedReports();
void viewAuditTrail();
void exportDataToFile();

// Customer functions
void processTransactions(int userId);
void viewTransactionHistory(int userId);
void manageChequeBook(int userId);

// Utility functions
int findUser(char* username, char* password, char* role);
int findAccount(int accountId);
int findAccountByUserId(int userId);

// --- Main Function ---
void createInitialData();

int main() {
    loadData();

    // Create initial data if files don't exist
    if (userCount == 0) {
        createInitialData();
        saveData();
    }
    
    mainMenu();
    saveData();
    return 0;
}

// --- Data Persistence Functions (File I/O) ---
void loadData() {
    FILE *file;

    // Load Users
    if ((file = fopen("users.txt", "r"))) {
        fscanf(file, "%d\n", &userCount);
        for (int i = 0; i < userCount; i++) {
            fscanf(file, "%d %s %s %s\n", &users[i].id, users[i].username, users[i].password, users[i].role);
        }
        fclose(file);
    }

    // Load Accounts
    if ((file = fopen("accounts.txt", "r"))) {
        fscanf(file, "%d\n", &accountCount);
        for (int i = 0; i < accountCount; i++) {
            fscanf(file, "%d %d %s %lf\n", &accounts[i].id, &accounts[i].userId, accounts[i].accountType, &accounts[i].balance);
        }
        fclose(file);
    }

    // Load Loans
    if ((file = fopen("loans.txt", "r"))) {
        fscanf(file, "%d\n", &loanCount);
        for (int i = 0; i < loanCount; i++) {
            fscanf(file, "%d %d %lf %lf %lf\n", &loans[i].id, &loans[i].accountId, &loans[i].principal, &loans[i].interestRate, &loans[i].remainingBalance);
        }
        fclose(file);
    }

    // Load Transactions
        if ((file = fopen("transactions.txt", "r"))) {
            fscanf(file, "%d\n", &transactionCount);
            for (int i = 0; i < transactionCount; i++) {
                char timePart[20];
                fscanf(file, "%d %d %s %lf %s %s\n", 
                    &transactions[i].transactionId, 
                    &transactions[i].accountId, 
                    transactions[i].type, 
                    &transactions[i].amount, 
                    transactions[i].date,
                    timePart);
                strcat(transactions[i].date, " ");
                strcat(transactions[i].date, timePart);
            }
            fclose(file);
        }

    // Load Audit Logs
    if ((file = fopen("audit_logs.txt", "r"))) {
        fscanf(file, "%d\n", &auditLogCount);
        for (int i = 0; i < auditLogCount; i++) {
            fscanf(file, "%d %s %[^\n]\n", &auditLogs[i].logId, auditLogs[i].timestamp, auditLogs[i].action);
        }
        fclose(file);
    }
}

void saveData() {
    FILE *file;

    // Save Users
    if ((file = fopen("users.txt", "w"))) {
        fprintf(file, "%d\n", userCount);
        for (int i = 0; i < userCount; i++) {
            fprintf(file, "%d %s %s %s\n", users[i].id, users[i].username, users[i].password, users[i].role);
        }
        fclose(file);
    }

    // Save Accounts
    if ((file = fopen("accounts.txt", "w"))) {
        fprintf(file, "%d\n", accountCount);
        for (int i = 0; i < accountCount; i++) {
            fprintf(file, "%d %d %s %.2f\n", accounts[i].id, accounts[i].userId, accounts[i].accountType, accounts[i].balance);
        }
        fclose(file);
    }

    // Save Loans
    if ((file = fopen("loans.txt", "w"))) {
        fprintf(file, "%d\n", loanCount);
        for (int i = 0; i < loanCount; i++) {
            fprintf(file, "%d %d %.2f %.2f %.2f\n", loans[i].id, loans[i].accountId, loans[i].principal, loans[i].interestRate, loans[i].remainingBalance);
        }
        fclose(file);
    }

    // Save Transactions
    if ((file = fopen("transactions.txt", "w"))) {
        fprintf(file, "%d\n", transactionCount);
        for (int i = 0; i < transactionCount; i++) {
            fprintf(file, "%d %d %s %.2f %s\n", transactions[i].transactionId, transactions[i].accountId, transactions[i].type, transactions[i].amount, transactions[i].date);
        }
        fclose(file);
    }

    // Save Audit Logs
    if ((file = fopen("audit_logs.txt", "w"))) {
        fprintf(file, "%d\n", auditLogCount);
        for (int i = 0; i < auditLogCount; i++) {
            fprintf(file, "%d %s %s\n", auditLogs[i].logId, auditLogs[i].timestamp, auditLogs[i].action);
        }
        fclose(file);
    }
}

// --- Main Menu and Authentication ---
void mainMenu() {
    int choice;
    do {
        printf("\n--- Bank Management System ---\n");
        printf("1. Login\n");
        printf("2. Register New Customer\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1: login(); break;
            case 2: registration(); break;
            case 3: printf("Exiting program. Goodbye!\n"); return;
            default: printf("Invalid choice.\n");
        }
    } while (1);
}

void login() {
    char username[50], password[50], role[20];
    int userId;
    printf("\n--- Login ---\n");
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);
    printf("Enter role (admin/customer): ");
    scanf("%s", role);

    userId = findUser(username, password, role);
    if (userId != -1) {
        printf("\nLogin successful! Welcome, %s.\n", username);
        if (strcmp(role, "admin") == 0) {
            logAction("Admin logged in.");
            adminMenu();
        }
        else if (strcmp(role, "customer") == 0) {
            logAction("Customer logged in.");
            customerMenu(userId);
        }
    } else {
        printf("\nInvalid credentials or role.\n");
    }
}

void registration() {
    if (userCount >= MAX_RECORDS) {
        printf("Error: Maximum user limit reached.\n");
        return;
    }
    printf("\n--- New Customer Registration ---\n");
    printf("Enter a new username: ");
    scanf("%s", users[userCount].username);
    printf("Enter a new password: ");
    scanf("%s", users[userCount].password);
    strcpy(users[userCount].role, "customer");
    users[userCount].id = userCount + 1;

    // Automatically create a savings account for the new customer
    accounts[accountCount].id = accountCount + 1;
    accounts[accountCount].userId = users[userCount].id;
    strcpy(accounts[accountCount].accountType, "savings");
    accounts[accountCount].balance = 0.0;
    accountCount++;

    userCount++;
    printf("Registration successful! Your Customer ID is: %d\n", users[userCount-1].id);
    saveData();
    logAction("New customer registered.");
}

// --- Admin Functions ---
void adminMenu() {
    int choice;
    do {
        printf("\n--- Admin Menu ---\n");
        printf("1. Manage Accounts\n");
        printf("2. Manage Loans\n");
        printf("3. View Automated Reports\n");
        printf("4. View Audit Trail\n");
        printf("5. Export Data to File\n");
        printf("6. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1: manageAccounts(); break;
            case 2: manageLoans(); break;
            case 3: viewAutomatedReports(); break;
            case 4: viewAuditTrail(); break;
            case 5: exportDataToFile(); break;
            case 6: printf("Logging out...\n"); return;
            default: printf("Invalid choice.\n");
        }
    } while (1);
}

void manageAccounts() {
    int choice, id;
    int accountIndex;
    printf("\n--- Account Management ---\n");
    printf("1. View all accounts\n");
    printf("2. View account by ID\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    clearInputBuffer();

    if (choice == 1) {
        printf("\n--- All Accounts ---\n");
        if (accountCount == 0) {
            printf("No accounts found.\n");
            return;
        }
        printf("%-5s %-10s %-15s\n", "ID", "User ID", "Balance");
        printf("-------------------------------\n");
        for (int i = 0; i < accountCount; i++) {
            printf("%-5d %-10d $%-14.2f\n", accounts[i].id, accounts[i].userId, accounts[i].balance);
        }
    } else if (choice == 2) {
        printf("Enter Account ID: ");
        scanf("%d", &id);
        accountIndex = findAccount(id);
        if (accountIndex != -1) {
            printf("\n--- Account Details ---\n");
            printf("Account ID: %d\n", accounts[accountIndex].id);
            printf("User ID: %d\n", accounts[accountIndex].userId);
            printf("Account Type: %s\n", accounts[accountIndex].accountType);
            printf("Balance: $%.2f\n", accounts[accountIndex].balance);
        } else {
            printf("Account not found.\n");
        }
    } else {
        printf("Invalid choice.\n");
    }
}

void manageLoans() {
    int choice;
    int id;
    printf("\n--- Loan Management ---\n");
    printf("1. Issue a new loan\n");
    printf("2. View all loans\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    clearInputBuffer();

    if (choice == 1) {
        if (loanCount >= MAX_RECORDS) {
            printf("Error: Max loan limit reached.\n");
            return;
        }
        printf("Enter Account ID for the loan: ");
        scanf("%d", &id);
        if (findAccount(id) == -1) {
            printf("Account not found.\n");
            return;
        }
        loans[loanCount].id = loanCount + 1;
        loans[loanCount].accountId = id;
        printf("Enter loan principal amount: ");
        scanf("%lf", &loans[loanCount].principal);
        loans[loanCount].remainingBalance = loans[loanCount].principal;
        printf("Enter interest rate (e.g., 0.05 for 5%%): ");
        scanf("%lf", &loans[loanCount].interestRate);
        loanCount++;
        printf("Loan issued successfully.\n");
        logAction("New loan issued.");
    } else if (choice == 2) {
        printf("\n--- All Loans ---\n");
        if (loanCount == 0) {
            printf("No loans found.\n");
            return;
        }
        printf("%-5s %-10s %-15s %-15s\n", "ID", "Acct ID", "Principal", "Remaining");
        printf("---------------------------------------------\n");
        for (int i = 0; i < loanCount; i++) {
            printf("%-5d %-10d $%-14.2f $%-14.2f\n", loans[i].id, loans[i].accountId, loans[i].principal, loans[i].remainingBalance);
        }
    } else {
        printf("Invalid choice.\n");
    }
}

void viewAutomatedReports() {
    double totalDeposits = 0.0;
    double totalWithdrawals = 0.0;
    double totalLoanPrincipal = 0.0;

    for (int i = 0; i < accountCount; i++) {
        totalDeposits += accounts[i].balance; 
    }
    for (int i = 0; i < loanCount; i++) {
        totalLoanPrincipal += loans[i].principal;
    }
    for (int i = 0; i < transactionCount; i++) {
        if (strcmp(transactions[i].type, "withdrawal") == 0) {
            totalWithdrawals += transactions[i].amount;
        }
    }

    printf("\n--- Automated Financial Report ---\n");
    printf("Total Accounts: %d\n", accountCount);
    printf("Total Loans Issued: %d\n", loanCount);
    printf("Total Deposits: $%.2f\n", totalDeposits);
    printf("Total Withdrawals: $%.2f\n", totalWithdrawals);
    printf("Total Loan Principal: $%.2f\n", totalLoanPrincipal);
    printf("Total Net Balance: $%.2f\n", totalDeposits - totalWithdrawals);
}

void viewAuditTrail() {
    printf("\n--- Audit Trail ---\n");
    if (auditLogCount == 0) {
        printf("No actions logged yet.\n");
        return;
    }
    printf("%-5s %-25s %-40s\n", "ID", "Timestamp", "Action");
    printf("----------------------------------------------------------------\n");
    for (int i = 0; i < auditLogCount; i++) {
        printf("%-5d %-25s %-40s\n", auditLogs[i].logId, auditLogs[i].timestamp, auditLogs[i].action);
    }
}

void exportDataToFile() {
    FILE *file = fopen("bank_report.txt", "w");
    if (file == NULL) {
        printf("Error creating file.\n");
        return;
    }
    
    fprintf(file, "--- Bank Management System Report ---\n\n");
    
    fprintf(file, "USER INFORMATION\n");
    fprintf(file, "%-5s %-20s %-15s\n", "ID", "Username", "Role");
    fprintf(file, "----------------------------------\n");
    for (int i = 0; i < userCount; i++) {
        fprintf(file, "%-5d %-20s %-15s\n", users[i].id, users[i].username, users[i].role);
    }
    fprintf(file, "\n");
    
    fprintf(file, "ACCOUNT INFORMATION\n");
    fprintf(file, "%-5s %-10s %-15s %-15s\n", "ID", "User ID", "Type", "Balance");
    fprintf(file, "--------------------------------------------------\n");
    for (int i = 0; i < accountCount; i++) {
        fprintf(file, "%-5d %-10d %-15s $%-14.2f\n", accounts[i].id, accounts[i].userId, accounts[i].accountType, accounts[i].balance);
    }
    
    fclose(file);
    printf("Data successfully exported to 'bank_report.txt'.\n");
    logAction("Data exported to file.");
}

// --- Customer Functions ---
void customerMenu(int userId) {
    int choice;
    do {
        printf("\n--- Customer Menu ---\n");
        printf("1. View Profile & Account Info\n");
        printf("2. Process Transactions\n");
        printf("3. View Transaction History\n");
        printf("4. View Loan Status\n");
        printf("5. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1: {
                int accountIndex = findAccountByUserId(userId);
                if (accountIndex != -1) {
                    printf("\n--- Your Profile ---\n");
                    printf("Account ID: %d\n", accounts[accountIndex].id);
                    printf("Balance: $%.2f\n", accounts[accountIndex].balance);
                } else {
                    printf("No account found for this user.\n");
                }
                break;
            }
            case 2: processTransactions(userId); break;
            case 3: viewTransactionHistory(userId); break;
            case 4: {
                printf("\n--- Your Loan Status ---\n");
                int accountIndex = findAccountByUserId(userId);
                int found = 0;
                for (int i = 0; i < loanCount; i++) {
                    if (loans[accountIndex].accountId == findAccountByUserId(userId)) {
                        printf("Loan ID: %d, Principal: $%.2f, Remaining: $%.2f\n", loans[i].id, loans[i].principal, loans[i].remainingBalance);
                        found = 1;
                    }
                }
                if (!found) {
                    printf("No loans found for your account.\n");
                }
                break;
            }
            case 5: printf("Logging out...\n"); return;
            default: printf("Invalid choice.\n");
        }
    } while (1);
}

void processTransactions(int userId) {
    int choice;
    double amount;
    int accountIndex = findAccountByUserId(userId);
    if (accountIndex == -1) {
        printf("Account not found.\n");
        return;
    }
    printf("\n--- Transactions ---\n");
    printf("1. Deposit\n");
    printf("2. Withdraw\n");
    printf("3. Transfer\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    
    if (choice == 1) {
        printf("Enter amount to deposit: ");
        scanf("%lf", &amount);
        accounts[accountIndex].balance += amount;
        printf("Deposit successful. New balance: $%.2f\n", accounts[accountIndex].balance);
        if (transactionCount < MAX_TRANSACTIONS) {
            transactions[transactionCount].transactionId = transactionCount + 1;
            transactions[transactionCount].accountId = accounts[accountIndex].id;
            strcpy(transactions[transactionCount].type, "deposit");
            transactions[transactionCount].amount = amount;
            time_t t = time(NULL);
            strftime(transactions[transactionCount].date, sizeof(transactions[transactionCount].date), "%Y-%m-%d %H:%M:%S", localtime(&t));
            transactionCount++;
            logAction("Customer made a deposit.");
        }
        saveData();
    } else if (choice == 2) {
        printf("Enter amount to withdraw: ");
        scanf("%lf", &amount);
        if (accounts[accountIndex].balance >= amount) {
            accounts[accountIndex].balance -= amount;
            printf("Withdrawal successful. New balance: $%.2f\n", accounts[accountIndex].balance);
            if (transactionCount < MAX_TRANSACTIONS) {
                transactions[transactionCount].transactionId = transactionCount + 1;
                transactions[transactionCount].accountId = accounts[accountIndex].id;
                strcpy(transactions[transactionCount].type, "withdrawal");
                transactions[transactionCount].amount = amount;
                time_t t = time(NULL);
                strftime(transactions[transactionCount].date, sizeof(transactions[transactionCount].date), "%Y-%m-%d %H:%M:%S", localtime(&t));
                transactionCount++;
                logAction("Customer made a withdrawal.");
            }
            saveData();
        } else {
            printf("Insufficient balance.\n");
        }
    } else if (choice == 3) {
        int targetAccountId;
        printf("Enter amount to transfer: ");
        scanf("%lf", &amount);
        printf("Enter target account ID: ");
        scanf("%d", &targetAccountId);
        int targetAccountIndex = findAccount(targetAccountId);
        if (targetAccountIndex != -1 && accounts[accountIndex].balance >= amount) {
            accounts[accountIndex].balance -= amount;
            accounts[targetAccountIndex].balance += amount;
            saveData();
            printf("Transfer successful. New balance: $%.2f\n", accounts[accountIndex].balance);
            if (transactionCount < MAX_TRANSACTIONS) {
                transactions[transactionCount].transactionId = transactionCount + 1;
                transactions[transactionCount].accountId = accounts[accountIndex].id;
                strcpy(transactions[transactionCount].type, "transfer");
                transactions[transactionCount].amount = amount;
                time_t t = time(NULL);
                strftime(transactions[transactionCount].date, sizeof(transactions[transactionCount].date), "%Y-%m-%d %H:%M:%S", localtime(&t));
                transactionCount++;
                logAction("Customer made a transfer.");
            }
        } else {
            printf("Transfer failed. Check account ID or balance.\n");
        }
    } else {
        printf("Invalid choice.\n");
    }
}

void viewTransactionHistory(int userId) {
    int accountId = findAccountByUserId(userId);
    if (accountId == -1) {
        printf("Account not found.\n");
        return;
    }
    printf("\n--- Transaction History ---\n");
    printf("%-5s %-15s %-10s %-25s\n", "ID", "Type", "Amount", "Date");
    printf("--------------------------------------------------\n");
    int found = 0;
    for (int i = 0; i < transactionCount; i++) {
        if (transactions[i].accountId == accounts[findAccountByUserId(userId)].id) {
            printf("%-5d %-15s $%-9.2f %-25s\n", transactions[i].transactionId, transactions[i].type, transactions[i].amount, transactions[i].date);
            found = 1;
        }
    }
    if (!found) {
        printf("No transactions found for this account.\n");
    }
}

// --- Utility Functions ---
int findUser(char* username, char* password, char* role) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 &&
            strcmp(users[i].password, password) == 0 &&
            strcmp(users[i].role, role) == 0) {
            return users[i].id;
        }
    }
    return -1;
}

int findAccount(int accountId) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].id == accountId) {
            return i;
        }
    }
    return -1;
}

int findAccountByUserId(int userId) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].userId == userId) {
            return i;
        }
    }
    return -1;
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void logAction(char* action) {
    if (auditLogCount < MAX_RECORDS) {
        auditLogs[auditLogCount].logId = auditLogCount + 1;
        time_t t = time(NULL);
        strftime(auditLogs[auditLogCount].timestamp, sizeof(auditLogs[auditLogCount].timestamp), "%Y-%m-%d %H:%M:%S", localtime(&t));
        strcpy(auditLogs[auditLogCount].action, action);
        auditLogCount++;
    }
}

void createInitialData() {
    // Initial Admin user
    users[userCount++] = (User){1, "admin", "admin123", "admin"};
    
    // Initial customer and user
    users[userCount++] = (User){2, "user", "user123", "customer"};

    printf("\nUsers:\n");
    for (int i = 0; i < userCount; i++) {
        printf("ID: %d | Username: %s | password: %s | Role: %s\n", 
               users[i].id,
               users[i].username,
               users[i].password, 
               users[i].role);
    }
    
    accounts[accountCount++] = (Account){1, 2, "savings", 5000.0};
    
    // Initial loan
    // loans[loanCount++] = (Loan){1, 1, 10000.0, 0.05, 10000.0}; 
    
    printf("Initial data created. Please restart the program.\n");
}
