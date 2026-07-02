#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ================= Data Structures ================= //
typedef struct Employee
{
    int id;
    char name[50];
    char dept[50];
    char password[20];
    int leaveBalance;
    struct Employee *next;
} Employee;

typedef struct Leave
{
    int empId;
    char type[20];
    char startDate[15];
    char endDate[15];
    char status[20]; // Pending, Approved, Rejected
    struct Leave *next;
} Leave;

typedef struct Admin
{
    char username[50];
    char password[20];
    struct Admin *next;
} Admin;

// ================= Global Linked Lists ================= //
Employee *empHead = NULL;
Leave *leaveHead = NULL;
Admin *adminHead = NULL;

// ================= Function Prototypes ================= //
void registerEmployee();
void registerAdmin();
Employee *loginEmployee();
Admin *loginAdmin();

void addEmployee();
void updateEmployee();
void deleteEmployee();
void viewEmployees();

void applyLeave(Employee *emp);
void approveLeave();
void viewLeaveHistory();
void searchEmployee();
void deleteEmployee();
void updateEmployee();

void changePassword(Employee *emp);
void cancelLeave(Employee *emp);
void myLeaveHistory(Employee *emp);
void editLeave(Employee *emp);

void leaveStatistics();

int generateID();
int idExists(int id);

void resetLeaveBalance();
void transferDepartment();
void addLeaveBalance();

void deleteAdmin();
void renameEmployee();

void reports();

void saveData();
void loadData();

// ================= Utility ================= //
void saveData()
{
    FILE *f1 = fopen("employees.txt", "w");

    Employee *e = empHead;
    while (e)
    {
        fprintf(f1, "%d %s %s %s %d\n",
                e->id,
                e->name,
                e->dept,
                e->password,
                e->leaveBalance);
        e = e->next;
    }
    fclose(f1);

    FILE *f2 = fopen("leaves.txt", "w");

    Leave *l = leaveHead;
    while (l)
    {
        fprintf(f2, "%d %s %s %s %s\n",
                l->empId,
                l->type,
                l->startDate,
                l->endDate,
                l->status);
        l = l->next;
    }
    fclose(f2);

    FILE *f3 = fopen("admins.txt", "w");

    Admin *a = adminHead;
    while (a)
    {
        fprintf(f3, "%s %s\n",
                a->username,
                a->password);
        a = a->next;
    }
    fclose(f3);
}

void loadData()
{

    FILE *f1 = fopen("employees.txt", "r");

    if (f1)
    {
        Employee *e;

        while (1)
        {
            e = (Employee *)malloc(sizeof(Employee));

            if (fscanf(f1, "%d %49s %49s %19s %d",
                       &e->id,
                       e->name,
                       e->dept,
                       e->password,
                       &e->leaveBalance) != 5)
            {

                free(e);
                break;
            }

            e->next = empHead;
            empHead = e;
        }

        fclose(f1);
    }

    FILE *f2 = fopen("leaves.txt", "r");

    if (f2)
    {

        Leave *l;

        while (1)
        {

            l = (Leave *)malloc(sizeof(Leave));

            if (fscanf(f2, "%d %19s %14s %14s %19s",
                       &l->empId,
                       l->type,
                       l->startDate,
                       l->endDate,
                       l->status) != 5)
            {

                free(l);
                break;
            }

            l->next = leaveHead;
            leaveHead = l;
        }

        fclose(f2);
    }

    FILE *f3 = fopen("admins.txt", "r");

    if (f3)
    {

        Admin *a;

        while (1)
        {

            a = (Admin *)malloc(sizeof(Admin));

            if (fscanf(f3, "%49s %19s",
                       a->username,
                       a->password) != 2)
            {

                free(a);
                break;
            }

            a->next = adminHead;
            adminHead = a;
        }

        fclose(f3);
    }
}

// ================= Authentication ================= //
void registerEmployee()
{
    Employee *newEmp = (Employee *)malloc(sizeof(Employee));
    newEmp->id = generateID();
    printf("Generated Employee ID : %d\n", newEmp->id);
    printf("Enter Name: ");
    scanf("%s", newEmp->name);
    printf("Enter Department: ");
    scanf("%s", newEmp->dept);
    printf("Set Password: ");
    scanf("%s", newEmp->password);
    newEmp->leaveBalance = 20;
    newEmp->next = empHead;
    empHead = newEmp;
    saveData();
    printf("Employee registered successfully!\n");
}

void registerAdmin()
{
    Admin *newAdmin = (Admin *)malloc(sizeof(Admin));
    printf("Enter Admin Username: ");
    scanf("%s", newAdmin->username);
    printf("Set Password: ");
    scanf("%s", newAdmin->password);
    newAdmin->next = adminHead;
    adminHead = newAdmin;
    saveData();
    printf("Admin registered successfully!\n");
}

Employee *loginEmployee()
{
    int id;
    char pass[20];
    printf("Enter Employee ID: ");
    scanf("%d", &id);
    printf("Enter Password: ");
    scanf("%s", pass);

    Employee *e = empHead;
    while (e)
    {
        if (e->id == id && strcmp(e->password, pass) == 0)
        {
            printf("Login successful. Welcome %s!\n", e->name);
            return e;
        }
        e = e->next;
    }
    printf("Invalid login.\n");
    return NULL;
}

Admin *loginAdmin()
{
    char user[50], pass[20];
    printf("Enter Username: ");
    scanf("%s", user);
    printf("Enter Password: ");
    scanf("%s", pass);

    Admin *a = adminHead;
    while (a)
    {
        if (strcmp(a->username, user) == 0 && strcmp(a->password, pass) == 0)
        {
            printf("Admin login successful. Welcome %s!\n", a->username);
            return a;
        }
        a = a->next;
    }
    printf("Invalid admin login.\n");
    return NULL;
}

// ================= Employee Management ================= //
void viewEmployees()
{
    Employee *temp = empHead;
    if (!temp)
    {
        printf("No employees found!\n");
        return;
    }
    printf("\nEmployee List:\n");
    while (temp)
    {
        printf("ID: %d | Name: %s | Dept: %s | Balance: %d\n", temp->id, temp->name, temp->dept, temp->leaveBalance);
        temp = temp->next;
    }
}

// ================= Leave Management ================= //
void applyLeave(Employee *emp)
{
    if (emp->leaveBalance <= 0)
    {
        printf("No leave balance left!\n");
        return;
    }
    Leave *newLeave = (Leave *)malloc(sizeof(Leave));
    newLeave->empId = emp->id;
    printf("Enter Leave Type: ");
    scanf("%s", newLeave->type);
    printf("Enter Start Date: ");
    scanf("%s", newLeave->startDate);
    printf("Enter End Date: ");
    scanf("%s", newLeave->endDate);
    strcpy(newLeave->status, "Pending");
    newLeave->next = leaveHead;
    leaveHead = newLeave;
    saveData();
    printf("Leave applied successfully.\n");
}

void approveLeave()
{
    int empId;
    printf("Enter Employee ID to review: ");
    scanf("%d", &empId);
    Leave *l = leaveHead;
    while (l && l->empId != empId)
        l = l->next;
    if (!l)
    {
        printf("No leave requests for this ID.\n");
        return;
    }
    printf("Leave Request: %s to %s [%s]\n", l->startDate, l->endDate, l->status);
    printf("1=Approve 2=Reject: ");
    int ch;
    scanf("%d", &ch);
    Employee *e = empHead;
    while (e && e->id != empId)
        e = e->next;
    if (ch == 1)
    {
        strcpy(l->status, "Approved");
        if (e)
            e->leaveBalance--;
    }
    else
        strcpy(l->status, "Rejected");
    saveData();
}

void viewLeaveHistory()
{
    Leave *l = leaveHead;
    if (!l)
    {
        printf("No leave records.\n");
        return;
    }
    while (l)
    {
        printf("EmpID: %d | %s: %s to %s | %s\n", l->empId, l->type, l->startDate, l->endDate, l->status);
        l = l->next;
    }
}

// ================= Reports ================= //
void reports()
{
    Employee *e = empHead;
    printf("\n=== Reports ===\n");
    while (e)
    {
        printf("ID: %d | %s | Dept: %s | Balance: %d\n", e->id, e->name, e->dept, e->leaveBalance);
        e = e->next;
    }
}

// ================= Menus ================= //
void employeeMenu(Employee *emp)
{
    int ch;

    do
    {

        printf("\n====== Employee Menu ======\n");
        printf("1. Apply Leave\n");
        printf("2. My Leave History\n");
        printf("3. Edit Pending Leave\n");
        printf("4. Cancel Pending Leave\n");
        printf("5. Change Password\n");
        printf("6. Logout\n");

        printf("Choice : ");
        scanf("%d", &ch);

        switch (ch)
        {

        case 1:
            applyLeave(emp);
            break;

        case 2:
            myLeaveHistory(emp);
            break;

        case 3:
            editLeave(emp);
            break;

        case 4:
            cancelLeave(emp);
            break;

        case 5:
            changePassword(emp);
            break;

        case 6:
            printf("Logged Out.\n");
            break;

        default:
            printf("Invalid Choice.\n");
        }

    } while (ch != 6);
}

void adminMenu(Admin *a)
{
    int ch;

    do
    {

        printf("\n========== ADMIN MENU ==========\n");

        printf("1. View Employees\n");
        printf("2. Search Employee\n");
        printf("3. Update Employee\n");
        printf("4. Delete Employee\n");
        printf("5. Rename Employee\n");
        printf("6. Transfer Department\n");
        printf("7. Add Leave Balance\n");
        printf("8. Reset Leave Balance\n");
        printf("9. Approve Leave\n");
        printf("10. View Leave History\n");
        printf("11. Leave Statistics\n");
        printf("12. Reports\n");
        printf("13. Delete Admin\n");
        printf("14. Logout\n");

        printf("Choice : ");
        scanf("%d", &ch);

        switch (ch)
        {

        case 1:
            viewEmployees();
            break;

        case 2:
            searchEmployee();
            break;

        case 3:
            updateEmployee();
            break;

        case 4:
            deleteEmployee();
            break;

        case 5:
            renameEmployee();
            break;

        case 6:
            transferDepartment();
            break;

        case 7:
            addLeaveBalance();
            break;

        case 8:
            resetLeaveBalance();
            break;

        case 9:
            approveLeave();
            break;

        case 10:
            viewLeaveHistory();
            break;

        case 11:
            leaveStatistics();
            break;

        case 12:
            reports();
            break;

        case 13:
            deleteAdmin();
            break;

        case 14:
            printf("Logged Out.\n");
            break;

        default:
            printf("Invalid Choice.\n");
        }

    } while (ch != 14);
}

// ================= Main ================= //
int main()
{
    loadData();
    int choice;
    while (1)
    {
        printf("\n===== Leave Management System =====\n");
        printf("1.Register Employee\n2.Register Admin\n3.Login Employee\n4.Login Admin\n5.Exit\nChoice: ");
        scanf("%d", &choice);
        if (choice == 1)
            registerEmployee();
        else if (choice == 2)
            registerAdmin();
        else if (choice == 3)
        {
            Employee *e = loginEmployee();
            if (e)
                employeeMenu(e);
        }
        else if (choice == 4)
        {
            Admin *a = loginAdmin();
            if (a)
                adminMenu(a);
        }
        else if (choice == 5)
        {
            saveData();
            exit(0);
        }
        else
            printf("Invalid choice.\n");
    }
}

void searchEmployee()
{
    int id;
    printf("Enter Employee ID: ");
    scanf("%d", &id);

    Employee *temp = empHead;

    while (temp != NULL)
    {
        if (temp->id == id)
        {
            printf("\nEmployee Found\n");
            printf("ID : %d\n", temp->id);
            printf("Name : %s\n", temp->name);
            printf("Department : %s\n", temp->dept);
            printf("Leave Balance : %d\n", temp->leaveBalance);
            return;
        }
        temp = temp->next;
    }

    printf("Employee not found.\n");
}

void deleteEmployee()
{

    int id;
    printf("Enter Employee ID: ");
    scanf("%d", &id);

    Employee *temp = empHead, *prev = NULL;

    while (temp && temp->id != id)
    {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL)
    {
        printf("Employee not found.\n");
        return;
    }

    if (prev == NULL)
        empHead = temp->next;
    else
        prev->next = temp->next;

    free(temp);

    saveData();

    printf("Employee deleted successfully.\n");
}

void updateEmployee()
{

    int id;

    printf("Enter Employee ID: ");
    scanf("%d", &id);

    Employee *temp = empHead;

    while (temp)
    {

        if (temp->id == id)
        {

            printf("Enter New Name: ");
            scanf("%s", temp->name);

            printf("Enter New Department: ");
            scanf("%s", temp->dept);

            printf("Enter New Password: ");
            scanf("%s", temp->password);

            saveData();

            printf("Updated Successfully.\n");
            return;
        }

        temp = temp->next;
    }

    printf("Employee not found.\n");
}

void changePassword(Employee *emp)
{

    char oldPass[20], newPass[20];

    printf("Old Password : ");
    scanf("%s", oldPass);

    if (strcmp(emp->password, oldPass) != 0)
    {
        printf("Wrong Password.\n");
        return;
    }

    printf("New Password : ");
    scanf("%s", newPass);

    strcpy(emp->password, newPass);

    saveData();

    printf("Password Changed.\n");
}

void cancelLeave(Employee *emp)
{

    Leave *l = leaveHead;

    while (l)
    {

        if (l->empId == emp->id && strcmp(l->status, "Pending") == 0)
        {

            strcpy(l->status, "Cancelled");

            saveData();

            printf("Leave Cancelled.\n");
            return;
        }

        l = l->next;
    }

    printf("No Pending Leave.\n");
}

void myLeaveHistory(Employee *emp)
{

    Leave *l = leaveHead;

    while (l)
    {

        if (l->empId == emp->id)
        {

            printf("%s %s %s %s\n",
                   l->type,
                   l->startDate,
                   l->endDate,
                   l->status);
        }

        l = l->next;
    }
}

void leaveStatistics()
{

    int total = 0, approved = 0, rejected = 0, pending = 0;

    Leave *l = leaveHead;

    while (l)
    {

        total++;

        if (strcmp(l->status, "Approved") == 0)
            approved++;

        else if (strcmp(l->status, "Rejected") == 0)
            rejected++;

        else
            pending++;

        l = l->next;
    }

    printf("Total : %d\n", total);
    printf("Approved : %d\n", approved);
    printf("Rejected : %d\n", rejected);
    printf("Pending : %d\n", pending);
}

int generateID()
{

    int max = 1000;

    Employee *e = empHead;

    while (e)
    {

        if (e->id > max)
            max = e->id;

        e = e->next;
    }

    return max + 1;
}

int idExists(int id)
{

    Employee *e = empHead;

    while (e)
    {

        if (e->id == id)
            return 1;

        e = e->next;
    }

    return 0;
}

void resetLeaveBalance()
{

    int id;

    printf("Enter Employee ID: ");
    scanf("%d", &id);

    Employee *e = empHead;

    while (e)
    {

        if (e->id == id)
        {

            e->leaveBalance = 20;

            saveData();

            printf("Leave Balance Reset Successfully.\n");
            return;
        }

        e = e->next;
    }

    printf("Employee Not Found.\n");
}

void transferDepartment()
{

    int id;
    char dept[50];

    printf("Employee ID : ");
    scanf("%d", &id);

    Employee *e = empHead;

    while (e)
    {

        if (e->id == id)
        {

            printf("New Department : ");
            scanf("%s", dept);

            strcpy(e->dept, dept);

            saveData();

            printf("Department Updated.\n");
            return;
        }

        e = e->next;
    }

    printf("Employee Not Found.\n");
}

void addLeaveBalance()
{

    int id, days;

    printf("Employee ID : ");
    scanf("%d", &id);

    printf("Extra Leave Days : ");
    scanf("%d", &days);

    Employee *e = empHead;

    while (e)
    {

        if (e->id == id)
        {

            e->leaveBalance += days;

            saveData();

            printf("Leave Added.\n");
            return;
        }

        e = e->next;
    }
}

void editLeave(Employee *emp)
{

    Leave *l = leaveHead;

    while (l)
    {

        if (l->empId == emp->id &&
            strcmp(l->status, "Pending") == 0)
        {

            printf("New Start Date : ");
            scanf("%s", l->startDate);

            printf("New End Date : ");
            scanf("%s", l->endDate);

            saveData();

            printf("Leave Updated.\n");
            return;
        }

        l = l->next;
    }

    printf("Pending Leave Not Found.\n");
}

void deleteAdmin()
{

    char user[50];

    printf("Username : ");
    scanf("%s", user);

    Admin *temp = adminHead;
    Admin *prev = NULL;

    while (temp)
    {

        if (strcmp(temp->username, user) == 0)
        {

            if (prev == NULL)
                adminHead = temp->next;
            else
                prev->next = temp->next;

            free(temp);

            saveData();

            printf("Admin Deleted.\n");
            return;
        }

        prev = temp;
        temp = temp->next;
    }

    printf("Admin Not Found.\n");
}

void renameEmployee()
{

    int id;
    char name[50];

    printf("Employee ID : ");
    scanf("%d", &id);

    Employee *e = empHead;

    while (e)
    {

        if (e->id == id)
        {

            printf("New Name : ");
            scanf("%s", name);

            strcpy(e->name, name);

            saveData();

            printf("Name Updated.\n");
            return;
        }

        e = e->next;
    }
}