// Donation Management System (arrays, admin/user, authentication, tabular display, file persistence)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DONORS 500
#define MAX_DONATIONS 2000
#define STR_SMALL 32
#define STR_MED 64
#define STR_LARGE 256

// Files
const char *DONOR_FILE = "donors.txt";
const char *DONATION_FILE = "donations.txt";

// Admin credentials (simple hardcoded admin)
const char *ADMIN_USER = "admin";
const char *ADMIN_PASS = "admin123";

// Donation categories
typedef enum
{
    CAT_MONEY = 1,
    CAT_FOOD,
    CAT_CLOTHES,
    CAT_MEDICINE,
    CAT_OTHER
} DonationCategory;
const char *CATEGORY_STR[] = {"Unknown", "Money", "Food", "Clothes", "Medicine", "Other"};

// Donor structure
typedef struct
{
    int id; // unique numeric id
    char name[STR_MED];
    int age;
    char gender[STR_SMALL];
    char contact[STR_SMALL];
    char password[STR_SMALL];
    int active; // 1 active, 0 deleted
} Donor;

// Donation structure
typedef struct
{
    int donationId; // unique auto-increment id
    int donorId;    // foreign key
    DonationCategory category;
    double amount;               // for money donations; 0 if not applicable
    char description[STR_LARGE]; // for items or notes
    char date[STR_SMALL];        // DD-MM-YYYY
    char status[STR_SMALL];      // Pending / Approved / Rejected
    int active;                  // 1 active, 0 deleted (soft)
} Donation;

// Storage arrays and counters
static Donor donors[MAX_DONORS];
static int donorCount = 0;

static Donation donations[MAX_DONATIONS];
static int donationCount = 0;
static int NEXT_DONATION_ID = 1;

// ----------------- Utility helpers -----------------
void pause_enter()
{
    printf("\nPress ENTER to continue...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

int findDonorIndexById(int id)
{
    for (int i = 0; i < donorCount; ++i)
        if (donors[i].active && donors[i].id == id)
            return i;
    return -1;
}

int findDonationIndexById(int donationId)
{
    for (int i = 0; i < donationCount; ++i)
        if (donations[i].active && donations[i].donationId == donationId)
            return i;
    return -1;
}

int idExistsDonor(int id)
{
    return findDonorIndexById(id) >= 0;
}

void printLine(int n)
{
    for (int i = 0; i < n; ++i)
        putchar('-');
    putchar('\n');
}

// ----------------- File I/O -----------------
void saveDonorsToFile()
{
    FILE *f = fopen(DONOR_FILE, "w");
    if (!f)
    {
        perror("Cannot open donors.txt");
        return;
    }

    fprintf(f, "%d\n", donorCount);

    for (int i = 0; i < donorCount; i++)
    {
        fprintf(f, "%d|%s|%d|%s|%s|%s|%d\n",
                donors[i].id,
                donors[i].name,
                donors[i].age,
                donors[i].gender,
                donors[i].contact,
                donors[i].password,
                donors[i].active);
    }

    fclose(f);
}

void loadDonorsFromFile()
{
    FILE *f = fopen(DONOR_FILE, "r");
    if (!f)
        return;

    fscanf(f, "%d\n", &donorCount);

    if (donorCount > MAX_DONORS)
        donorCount = 0;

    for (int i = 0; i < donorCount; i++)
    {
        fscanf(f, "%d|%[^|]|%d|%[^|]|%[^|]|%[^|]|%d\n",
               &donors[i].id,
               donors[i].name,
               &donors[i].age,
               donors[i].gender,
               donors[i].contact,
               donors[i].password,
               &donors[i].active);
    }

    fclose(f);
}

void saveDonationsToFile()
{
    FILE *f = fopen(DONATION_FILE, "w");
    if (!f)
    {
        perror("Cannot open donations.txt");
        return;
    }

    fprintf(f, "%d\n", donationCount);

    for (int i = 0; i < donationCount; i++)
    {

        fprintf(f, "%d|%d|%d|%.2lf|%s|%s|%s|%d\n",
                donations[i].donationId,
                donations[i].donorId,
                donations[i].category,
                donations[i].amount,
                donations[i].description,
                donations[i].date,
                donations[i].status,
                donations[i].active);
    }

    fprintf(f, "NEXT_ID|%d\n", NEXT_DONATION_ID);

    fclose(f);
}

void loadDonationsFromFile()
{
    FILE *f = fopen(DONATION_FILE, "r");
    if (!f)
        return;

    fscanf(f, "%d\n", &donationCount);

    if (donationCount > MAX_DONATIONS)
        donationCount = 0;

    for (int i = 0; i < donationCount; i++)
    {

        fscanf(f, "%d|%d|%d|%lf|%[^|]|%[^|]|%[^|]|%d\n",
               &donations[i].donationId,
               &donations[i].donorId,
               (int *)&donations[i].category,
               &donations[i].amount,
               donations[i].description,
               donations[i].date,
               donations[i].status,
               &donations[i].active);
    }

    char buffer[20];

    if (fscanf(f, "%[^|]|%d", buffer, &NEXT_DONATION_ID) != 2)
    {
        NEXT_DONATION_ID = donationCount + 1;
    }

    fclose(f);
}

// ----------------- Tabular displays -----------------
void listAllDonors()
{
    FILE *f = fopen(DONOR_FILE, "r");
    if (!f)
    {
        printf("No donor file found.\n");
        return;
    }

    int count;
    fscanf(f, "%d\n", &count);

    printLine(80);
    printf("%-6s %-22s %-4s %-8s %-16s %-8s\n",
           "ID", "Name", "Age", "Gender", "Contact", "Active");
    printLine(80);

    Donor d;

    for (int i = 0; i < count; i++)
    {

        fscanf(f, "%d|%[^|]|%d|%[^|]|%[^|]|%[^|]|%d\n",
               &d.id,
               d.name,
               &d.age,
               d.gender,
               d.contact,
               d.password,
               &d.active);

        if (d.active)
        {
            printf("%-6d %-22s %-4d %-8s %-16s %-8s\n",
                   d.id,
                   d.name,
                   d.age,
                   d.gender,
                   d.contact,
                   d.active ? "Yes" : "No");
        }
    }

    fclose(f);
}

void listAllDonations()
{

    FILE *f = fopen(DONATION_FILE, "r");

    if (!f)
    {
        printf("No donation file.\n");
        return;
    }

    int count;
    fscanf(f, "%d\n", &count);

    Donation d;

    printLine(120);

    printf("%-6s %-7s %-12s %-10s %-30s %-12s %-10s\n",
           "Did", "Donor", "Category", "Amount",
           "Description", "Date", "Status");

    printLine(120);

    for (int i = 0; i < count; i++)
    {

        fscanf(f, "%d|%d|%d|%lf|%[^|]|%[^|]|%[^|]|%d\n",
               &d.donationId,
               &d.donorId,
               (int *)&d.category,
               &d.amount,
               d.description,
               d.date,
               d.status,
               &d.active);

        if (d.active)
        {

            printf("%-6d %-7d %-12s %-10.2f %-30s %-12s %-10s\n",
                   d.donationId,
                   d.donorId,
                   CATEGORY_STR[d.category],
                   d.amount,
                   d.description,
                   d.date,
                   d.status);
        }
    }

    fclose(f);
}

// ----------------- Admin Features -----------------
int adminLogin()
{
    char user[STR_SMALL], pass[STR_SMALL];
    printf("Admin username: ");
    scanf("%31s", user);
    printf("Admin password: ");
    scanf("%31s", pass);
    if (strcmp(user, ADMIN_USER) == 0 && strcmp(pass, ADMIN_PASS) == 0)
    {
        printf("Admin login successful.\n");
        return 1;
    }
    printf("Invalid admin credentials.\n");
    return 0;
}

void adminViewAllUsers()
{
    if (donorCount == 0)
    {
        printf("No donors registered.\n");
        return;
    }
    listAllDonors();
}

void adminViewAllDonations()
{
    if (donationCount == 0)
    {
        printf("No donations recorded.\n");
        return;
    }
    listAllDonations(-1);
}

void adminApproveRejectDonation()
{
    int id;
    printf("Enter Donation ID: ");
    scanf("%d", &id);
    FILE *f = fopen(DONATION_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!f || !temp)
    {
        printf("File error\n");
        return;
    }
    int count;
    fscanf(f, "%d\n", &count);
    fprintf(temp, "%d\n", count);
    Donation d;
    for (int i = 0; i < count; i++)
    {

        fscanf(f, "%d|%d|%d|%lf|%[^|]|%[^|]|%[^|]|%d\n",
               &d.donationId,
               &d.donorId,
               (int *)&d.category,
               &d.amount,
               d.description,
               d.date,
               d.status,
               &d.active);
        if (d.donationId == id &&
            strcmp(d.status, "Pending") == 0)
        {

            int ch;

            printf("1.Approve\n2.Reject\n");
            scanf("%d", &ch);

            if (ch == 1)
                strcpy(d.status, "Approved");

            else if (ch == 2)
                strcpy(d.status, "Rejected");
        }
        fprintf(temp, "%d|%d|%d|%.2lf|%s|%s|%s|%d\n",
                d.donationId,
                d.donorId,
                d.category,
                d.amount,
                d.description,
                d.date,
                d.status,
                d.active);
    }

    fclose(f);
    fclose(temp);

    remove(DONATION_FILE);
    rename("temp.txt", DONATION_FILE);

    printf("Updated successfully\n");
}

// Reports
void reportTotals()
{

    FILE *f = fopen(DONATION_FILE, "r");
    if (!f)
    {
        printf("No donation file found.\n");
        return;
    }

    int count;
    fscanf(f, "%d\n", &count);

    Donation d;
    double totalMoney = 0.0;
    int totalDonations = 0;

    for (int i = 0; i < count; i++)
    {

        fscanf(f, "%d|%d|%d|%lf|%[^|]|%[^|]|%[^|]|%d\n",
               &d.donationId,
               &d.donorId,
               (int *)&d.category,
               &d.amount,
               d.description,
               d.date,
               d.status,
               &d.active);

        if (!d.active)
            continue;

        totalDonations++;

        if (d.category == CAT_MONEY &&
            strcmp(d.status, "Approved") == 0)
        {
            totalMoney += d.amount;
        }
    }

    fclose(f);

    printf("Total donations recorded: %d\n", totalDonations);
    printf("Total approved monetary donations: %.2f\n", totalMoney);
}

void reportCategorySummary()
{
    int counts[6] = {0};
    double moneyTotal = 0.0;
    for (int i = 0; i < donationCount; ++i)
    {
        Donation *d = &donations[i];
        if (!d->active)
            continue;
        counts[(int)d->category]++;
        if (d->category == CAT_MONEY && strcmp(d->status, "Approved") == 0)
            moneyTotal += d->amount;
    }
    printLine(60);
    printf("%-12s %-10s\n", "Category", "Count");
    printLine(60);
    for (int c = CAT_MONEY; c <= CAT_OTHER; ++c)
    {
        printf("%-12s %-10d\n", CATEGORY_STR[c], counts[c]);
    }
    printLine(60);
    printf("Approved money total: %.2f\n", moneyTotal);
}

void reportDonorWise()
{
    printLine(80);
    printf("%-6s %-20s %-12s\n", "Donor", "Name", "TotalAmountApproved");
    printLine(80);
    for (int i = 0; i < donorCount; ++i)
    {
        Donor *dn = &donors[i];
        if (!dn->active)
            continue;
        double tot = 0.0;
        for (int j = 0; j < donationCount; ++j)
        {
            Donation *d = &donations[j];
            if (!d->active)
                continue;
            if (d->donorId == dn->id && d->category == CAT_MONEY && strcmp(d->status, "Approved") == 0)
                tot += d->amount;
        }
        printf("%-6d %-20s %-12.2f\n", dn->id, dn->name, tot);
    }
    printLine(80);
}

void reportDailySummary()
{
    char date[STR_SMALL];
    printf("Enter date (DD-MM-YYYY): ");
    scanf("%s", date);
    int total = 0, approved = 0, pending = 0, rejected = 0;
    for (int i = 0; i < donationCount; ++i)
    {
        Donation *d = &donations[i];
        if (!d->active)
            continue;
        if (strcmp(d->date, date) == 0)
        {
            total++;
            if (strcmp(d->status, "Approved") == 0)
                approved++;
            else if (strcmp(d->status, "Pending") == 0)
                pending++;
            else if (strcmp(d->status, "Rejected") == 0)
                rejected++;
        }
    }
    printf("Date %s -> Total:%d Approved:%d Pending:%d Rejected:%d\n", date, total, approved, pending, rejected);
}

// Admin menu
void adminMenu()
{
    while (1)
    {
        printf("\n===== ADMIN MENU =====\n");
        printf("1. See all users (donors)\n");
        printf("2. See all donations\n");
        printf("3. Approve/Reject donation\n");
        printf("4. Reports: totals\n");
        printf("5. Reports: category summary\n");
        printf("6. Reports: donor-wise totals\n");
        printf("7. Reports: daily summary\n");
        printf("8. Exit to main\n");
        printf("Choice: ");
        int ch;
        if (scanf("%d", &ch) != 1)
        {
            while (getchar() != '\n')
                ;
            continue;
        }
        if (ch == 1)
            adminViewAllUsers();
        else if (ch == 2)
            adminViewAllDonations();
        else if (ch == 3)
            adminApproveRejectDonation();
        else if (ch == 4)
            reportTotals();
        else if (ch == 5)
            reportCategorySummary();
        else if (ch == 6)
            reportDonorWise();
        else if (ch == 7)
            reportDailySummary();
        else if (ch == 8)
            break;
        else
            printf("Invalid choice.\n");
        pause_enter();
    }
}

// ----------------- Donor (User) Features -----------------
void donorRegister()
{
    if (donorCount >= MAX_DONORS)
    {
        printf("Donor storage full.\n");
        return;
    }
    Donor d;
    memset(&d, 0, sizeof(d));
    printf("Choose numeric ID: ");
    scanf("%d", &d.id);
    if (idExistsDonor(d.id))
    {
        printf("ID exists, choose different.\n");
        return;
    }
    printf("Name: ");
    getchar();
    fgets(d.name, STR_MED, stdin);
    d.name[strcspn(d.name, "\n")] = 0;
    printf("Age: ");
    scanf("%d", &d.age);
    printf("Gender: ");
    scanf("%31s", d.gender);
    printf("Contact: ");
    scanf("%31s", d.contact);
    printf("Password: ");
    scanf("%31s", d.password);
    d.active = 1;
    donors[donorCount++] = d;
    saveDonorsToFile();
    printf("Registration successful. You can login with your ID and password.\n");
}

int donorLogin()
{
    int id;
    char pw[STR_SMALL];
    printf("Donor ID: ");
    scanf("%d", &id);
    int idx = findDonorIndexById(id);
    if (idx < 0)
    {
        printf("No such donor.\n");
        return -1;
    }
    printf("Password: ");
    scanf("%31s", pw);
    if (strcmp(donors[idx].password, pw) == 0)
    {
        printf("Login successful. Welcome %s!\n", donors[idx].name);
        return id;
    }
    printf("Wrong password.\n");
    return -1;
}

void donorUpdateProfile(int donorId)
{
    FILE *f = fopen(DONOR_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!f || !temp)
    {
        printf("File error.\n");
        return;
    }

    int count;
    fscanf(f, "%d\n", &count);

    fprintf(temp, "%d\n", count);

    Donor d;
    char buffer[STR_MED];

    for (int i = 0; i < count; i++)
    {

        fscanf(f, "%d|%[^|]|%d|%[^|]|%[^|]|%[^|]|%d\n",
               &d.id,
               d.name,
               &d.age,
               d.gender,
               d.contact,
               d.password,
               &d.active);

        if (d.id == donorId)
        {

            getchar();

            printf("Name (%s): ", d.name);
            fgets(buffer, STR_MED, stdin);

            if (buffer[0] != '\n')
            {
                buffer[strcspn(buffer, "\n")] = 0;
                strcpy(d.name, buffer);
            }

            printf("Age (%d): ", d.age);
            fgets(buffer, STR_MED, stdin);

            if (buffer[0] != '\n')
                d.age = atoi(buffer);

            printf("Gender (%s): ", d.gender);
            fgets(buffer, STR_MED, stdin);

            if (buffer[0] != '\n')
            {
                buffer[strcspn(buffer, "\n")] = 0;
                strcpy(d.gender, buffer);
            }

            printf("Contact (%s): ", d.contact);
            fgets(buffer, STR_MED, stdin);

            if (buffer[0] != '\n')
            {
                buffer[strcspn(buffer, "\n")] = 0;
                strcpy(d.contact, buffer);
            }
        }

        fprintf(temp, "%d|%s|%d|%s|%s|%s|%d\n",
                d.id,
                d.name,
                d.age,
                d.gender,
                d.contact,
                d.password,
                d.active);
    }

    fclose(f);
    fclose(temp);

    remove(DONOR_FILE);
    rename("temp.txt", DONOR_FILE);

    printf("Profile updated.\n");
}

void donorChangePassword(int donorId)
{

    FILE *f = fopen(DONOR_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!f || !temp)
    {
        printf("File error\n");
        return;
    }

    int count;
    fscanf(f, "%d\n", &count);

    fprintf(temp, "%d\n", count);

    Donor d;

    char cur[STR_SMALL];
    char nw[STR_SMALL];

    printf("Current password: ");
    scanf("%31s", cur);

    printf("New password: ");
    scanf("%31s", nw);

    int changed = 0;

    for (int i = 0; i < count; i++)
    {

        fscanf(f, "%d|%[^|]|%d|%[^|]|%[^|]|%[^|]|%d\n",
               &d.id,
               d.name,
               &d.age,
               d.gender,
               d.contact,
               d.password,
               &d.active);

        if (d.id == donorId)
        {

            if (strcmp(cur, d.password) != 0)
            {
                printf("Incorrect password.\n");
                fclose(f);
                fclose(temp);
                remove("temp.txt");
                return;
            }

            strcpy(d.password, nw);
            changed = 1;
        }

        fprintf(temp, "%d|%s|%d|%s|%s|%s|%d\n",
                d.id, d.name, d.age,
                d.gender, d.contact,
                d.password, d.active);
    }

    fclose(f);
    fclose(temp);

    remove(DONOR_FILE);
    rename("temp.txt", DONOR_FILE);

    if (changed)
        printf("Password updated.\n");
}

void donorMakeDonation(int donorId){
    FILE *f = fopen(DONATION_FILE, "r");

    if (!f)
    {
        f = fopen(DONATION_FILE, "w");

        if (!f)
        {
            printf("Cannot create donation file\n");
            return;
        }

        fprintf(f, "0\nNEXT_ID|1\n");
        fclose(f);

        f = fopen(DONATION_FILE, "r");
    }

    int count;
    fscanf(f, "%d\n", &count);

    Donation *arr = malloc((count + 1) * sizeof(Donation));

    for (int i = 0; i < count; i++)
    {
        fscanf(f, "%d|%d|%d|%lf|%[^|]|%[^|]|%[^|]|%d\n",
               &arr[i].donationId,
               &arr[i].donorId,
               (int *)&arr[i].category,
               &arr[i].amount,
               arr[i].description,
               arr[i].date,
               arr[i].status,
               &arr[i].active);
    }

    char temp[20];
    fscanf(f, "%[^|]|%d", temp, &NEXT_DONATION_ID);

    fclose(f);

    Donation d;
    memset(&d, 0, sizeof(d));

    d.donationId = NEXT_DONATION_ID++;
    d.donorId = donorId;

    printf("Select category:\n");
    printf("1 Money\n2 Food\n3 Clothes\n4 Medicine\n5 Other\n");

    int c;
    scanf("%d", &c);

    if (c < 1 || c > 5)
    {
        free(arr);
        return;
    }

    d.category = (DonationCategory)c;

    if (d.category == CAT_MONEY)
    {
        printf("Amount: ");
        scanf("%lf", &d.amount);

        if (d.amount <= 0)
        {
            free(arr);
            return;
        }

        strcpy(d.description, "Monetary donation");
    }
    else
    {
        getchar();

        printf("Description: ");
        fgets(d.description, STR_LARGE, stdin);

        d.description[strcspn(d.description, "\n")] = 0;

        d.amount = 0;
    }

    printf("Date DD-MM-YYYY: ");
    scanf("%31s", d.date);

    strcpy(d.status, "Pending");

    d.active = 1;

    arr[count] = d;

    count++;

    // rewrite whole file

    f = fopen(DONATION_FILE, "w");

    fprintf(f, "%d\n", count);

    for (int i = 0; i < count; i++)
    {
        fprintf(f, "%d|%d|%d|%.2lf|%s|%s|%s|%d\n",
                arr[i].donationId,
                arr[i].donorId,
                arr[i].category,
                arr[i].amount,
                arr[i].description,
                arr[i].date,
                arr[i].status,
                arr[i].active);
    }

    fprintf(f, "NEXT_ID|%d\n", NEXT_DONATION_ID);

    fclose(f);

    free(arr);

    printf("Donation submitted. ID=%d\n",
           d.donationId);
}

void donorViewMyDonations(int donorId)
{
    listAllDonations(donorId);
}

void donorMenu(int donorId)
{
    while (1)
    {
        printf("\n===== DONOR MENU =====\n");
        printf("1. Make a donation\n");
        printf("2. View my donations\n");
        printf("3. Update profile\n");
        printf("4. Change password\n");
        printf("5. Logout\n");
        printf("Choice: ");
        int ch;
        if (scanf("%d", &ch) != 1)
        {
            while (getchar() != '\n')
                ;
            continue;
        }
        if (ch == 1)
            donorMakeDonation(donorId);
        else if (ch == 2)
            donorViewMyDonations(donorId);
        else if (ch == 3)
            donorUpdateProfile(donorId);
        else if (ch == 4)
            donorChangePassword(donorId);
        else if (ch == 5)
            break;
        else
            printf("Invalid choice.\n");
        pause_enter();
    }
}

// ----------------- Main -----------------
void seedIfEmpty()
{
    // only seed if no donors exist
    if (donorCount == 0)
    {
        Donor d1 = {1001, "Alice", 30, "F", "9999999990", "alicepw", 1};
        Donor d2 = {1002, "Bob", 45, "M", "9999999991", "bobpw", 1};
        donors[donorCount++] = d1;
        donors[donorCount++] = d2;
        saveDonorsToFile();
    }
    // no seed for donations
}

int main()
{
    // load data
    loadDonorsFromFile();
    loadDonationsFromFile();
    seedIfEmpty();

    while (1)
    {
        printf("\n===== DONATION MANAGEMENT SYSTEM =====\n");
        printf("1. Admin Login\n");
        printf("2. Donor Login\n");
        printf("3. Donor Register\n");
        printf("4. Exit\n");
        printf("Choice: ");
        int choice;
        if (scanf("%d", &choice) != 1)
        {
            printf("Input error.\n");
            break;
        }

        if (choice == 1)
        {
            if (adminLogin())
                adminMenu();
        }
        else if (choice == 2)
        {
            int did = donorLogin();
            if (did > 0)
                donorMenu(did);
        }
        else if (choice == 3)
        {
            donorRegister();
        }
        else if (choice == 4)
        {
            // save and exit
            saveDonorsToFile();
            saveDonationsToFile();
            printf("Goodbye!\n");
            break;
        }
        else
        {
            printf("Invalid choice.\n");
        }
    }

    return 0;
}