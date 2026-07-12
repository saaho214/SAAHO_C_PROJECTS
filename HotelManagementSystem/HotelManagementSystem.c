#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
#include <time.h>

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

#define ROOMS_FILE "rooms.txt"
#define CUSTOMERS_FILE "customers.txt"
#define BOOKINGS_FILE "bookings.txt"
#define USERS_FILE "users.txt"
#define SERVICES_FILE "services.txt"
#define REVENUE_FILE "revenue.txt"

// -----------------------------
// Models
// -----------------------------

typedef enum
{
    ROOM_STANDARD = 1,
    ROOM_DELUXE = 2,
    ROOM_SUITE = 3
} RoomType;
typedef enum
{
    ROOM_FREE = 0,
    ROOM_OCCUPIED = 1,
    ROOM_RESERVED = 2
} RoomStatus;

typedef struct
{
    int roomNo;           // unique
    RoomType type;        // 1/2/3
    double pricePerNight; // per night
    RoomStatus status;    // free/occ/res
} Room;

// Basic customer record
typedef struct
{
    int id; // auto-increment ID
    char name[64];
    char phone[24];
    char govId[32]; // national ID/passport
    char address[128];
} Customer;

typedef struct
{
    int id; // auto-increment booking id
    int roomNo;
    int customerId;
    char checkin[11];   // YYYY-MM-DD
    char checkout[11];  // YYYY-MM-DD (empty until checkout)
    double extrasTotal; // food/services
    double roomTotal;   // computed at checkout
    double grandTotal;  // roomTotal + extrasTotal
    int isActive;       // 1 if currently checked in
} Booking;

typedef enum
{
    ROLE_ADMIN = 1,
    ROLE_RECEPTION = 2
} UserRole;

typedef struct
{
    int id; // auto-increment
    char username[32];
    char password[64]; // plaintext for demo; use hashing in real apps
    UserRole role;
} User;

// Food/Service items
typedef struct
{
    int id; // auto-increment
    char name[64];
    double price;
} ServiceItem;

// Revenue entries (used for reporting)
typedef struct
{
    int bookingId;
    char date[11]; // YYYY-MM-DD (checkout date)
    double amount;
} Revenue;

// -----------------------------
// Utility helpers
// -----------------------------

static int parseDate(const char *s, struct tm *out)
{
    // Expect YYYY-MM-DD
    if (!s || strlen(s) != 10)
        return 0;
    int y, m, d;
    if (sscanf(s, "%d-%d-%d", &y, &m, &d) != 3)
        return 0;
    if (y < 1900 || m < 1 || m > 12 || d < 1 || d > 31)
        return 0;
    memset(out, 0, sizeof(struct tm));
    out->tm_year = y - 1900;
    out->tm_mon = m - 1;
    out->tm_mday = d;
    return 1;
}

static int daysBetween(const char *start, const char *end)
{
    struct tm a, b;
    if (!parseDate(start, &a) || !parseDate(end, &b))
        return -1;
    time_t ta = mktime(&a);
    time_t tb = mktime(&b);
    if (tb <= ta)
        return -1; // must be after
    double diff = difftime(tb, ta);
    int days = (int)(diff / (60 * 60 * 24));
    if (days < 1)
        days = 1; // minimum 1 night
    return days;
}

static char *strcasestr_custom(const char *haystack, const char *needle)
{
    if (!*needle)
        return (char *)haystack;
    for (; *haystack; haystack++)
    {
        const char *h = haystack;
        const char *n = needle;
        while (*h && *n && tolower((unsigned char)*h) == tolower((unsigned char)*n))
        {
            h++;
            n++;
        }
        if (!*n)
            return (char *)haystack;
    }
    return NULL;
}

// -----------------------------
// Typed readAll wrappers
// -----------------------------

int readAllRooms(Room **out)
{
    FILE *fp = fopen(ROOMS_FILE, "r");
    if (fp == NULL)
    {
        *out = NULL;
        return 0;
    }
    int count = 0;
    Room temp;
    while (fscanf(fp, "%d %d %lf %d",
                  &temp.roomNo,
                  (int *)&temp.type,
                  &temp.pricePerNight,
                  (int *)&temp.status) == 4)
    {
        count++;
    }
    rewind(fp);
    *out = (Room *)malloc(count * sizeof(Room));
    for (int i = 0; i < count; i++)
    {
        fscanf(fp, "%d %d %lf %d",
               &(*out)[i].roomNo,
               (int *)&(*out)[i].type,
               &(*out)[i].pricePerNight,
               (int *)&(*out)[i].status);
    }
    fclose(fp);
    return count;
}

static int readAllCustomers(Customer **out)
{
    FILE *fp = fopen(CUSTOMERS_FILE, "r");
    if (!fp)
    {
        *out = NULL;
        return 0;
    }
    Customer temp;
    int count = 0;
    while (fscanf(fp, "%d|%63[^|]|%23[^|]|%31[^|]|%127[^\n]\n",
                  &temp.id,
                  temp.name,
                  temp.phone,
                  temp.govId,
                  temp.address) == 5)
    {
        count++;
    }
    rewind(fp);
    *out = (Customer *)malloc(count * sizeof(Customer));
    for (int i = 0; i < count; i++)
    {
        fscanf(fp, "%d|%63[^|]|%23[^|]|%31[^|]|%127[^\n]\n",
               &(*out)[i].id,
               (*out)[i].name,
               (*out)[i].phone,
               (*out)[i].govId,
               (*out)[i].address);
    }
    fclose(fp);
    return count;
}

static int readAllBookings(Booking **out){
    FILE *fp = fopen(BOOKINGS_FILE,"r");

    if(fp==NULL){
        *out=NULL;
        return 0;
    }

    Booking temp;
    int count=0;

    while(fscanf(fp,"%d %d %d %10s %10s %lf %lf %lf %d",
          &temp.id,
          &temp.roomNo,
          &temp.customerId,
          temp.checkin,
          temp.checkout,
          &temp.extrasTotal,
          &temp.roomTotal,
          &temp.grandTotal,
          &temp.isActive)==9)
    {
        count++;
    }

    rewind(fp);

    *out=(Booking*)malloc(count*sizeof(Booking));

    for(int i=0;i<count;i++)
    {
        fscanf(fp,"%d %d %d %10s %10s %lf %lf %lf %d",
              &(*out)[i].id,
              &(*out)[i].roomNo,
              &(*out)[i].customerId,
              (*out)[i].checkin,
              (*out)[i].checkout,
              &(*out)[i].extrasTotal,
              &(*out)[i].roomTotal,
              &(*out)[i].grandTotal,
              &(*out)[i].isActive);
    }

    fclose(fp);
    return count;
}

static int readAllUsers(User **out){
    FILE *fp=fopen(USERS_FILE,"r");

    if(fp==NULL){
        *out=NULL;
        return 0;
    }

    User temp;
    int count=0;

    while(fscanf(fp,"%d %31s %63s %d",
          &temp.id,
          temp.username,
          temp.password,
          (int*)&temp.role)==4)
    {
        count++;
    }

    rewind(fp);

    *out=(User*)malloc(count*sizeof(User));

    for(int i=0;i<count;i++)
    {
        fscanf(fp,"%d %31s %63s %d",
              &(*out)[i].id,
              (*out)[i].username,
              (*out)[i].password,
              (int*)&(*out)[i].role);
    }

    fclose(fp);
    return count;
}

static int readAllServices(ServiceItem **out){
    FILE *fp=fopen(SERVICES_FILE,"r");

    if(fp==NULL){
        *out=NULL;
        return 0;
    }

    ServiceItem temp;
    int count=0;

    while(fscanf(fp,"%d %63s %lf",
          &temp.id,
          temp.name,
          &temp.price)==3)
    {
        count++;
    }

    rewind(fp);

    *out=(ServiceItem*)malloc(count*sizeof(ServiceItem));

    for(int i=0;i<count;i++)
    {
        fscanf(fp,"%d %63s %lf",
              &(*out)[i].id,
              (*out)[i].name,
              &(*out)[i].price);
    }

    fclose(fp);
    return count;
}

static int readAllRevenue(Revenue **out){
    FILE *fp=fopen(REVENUE_FILE,"r");

    if(fp==NULL){
        *out=NULL;
        return 0;
    }

    Revenue temp;
    int count=0;

    while(fscanf(fp,"%d %10s %lf",
          &temp.bookingId,
          temp.date,
          &temp.amount)==3)
    {
        count++;
    }

    rewind(fp);

    *out=(Revenue*)malloc(count*sizeof(Revenue));

    for(int i=0;i<count;i++)
    {
        fscanf(fp,"%d %10s %lf",
              &(*out)[i].bookingId,
              (*out)[i].date,
              &(*out)[i].amount);
    }

    fclose(fp);
    return count;
}

// -----------------------------
// Persistence helpers (append/replace)
// -----------------------------

static int appendCustomer(const Customer *c){
    FILE *fp = fopen(CUSTOMERS_FILE, "a");
    if (!fp)
        return 0;

    fprintf(fp,"%d|%s|%s|%s|%s\n",
            c->id,
            c->name,
            c->phone,
            c->govId,
            c->address);

    fclose(fp);
    return 1;
}

static int overwriteCustomers(Customer *arr,int n){
    FILE *fp = fopen(CUSTOMERS_FILE,"w");
    if(!fp)
        return 0;

    for(int i=0;i<n;i++)
    {
        fprintf(fp,"%d|%s|%s|%s|%s\n",
                arr[i].id,
                arr[i].name,
                arr[i].phone,
                arr[i].govId,
                arr[i].address);
    }

    fclose(fp);
    return 1;
}

static int overwriteBookings(Booking *arr, int n){
    FILE *fp = fopen(BOOKINGS_FILE, "w");
    if (fp == NULL)
        return 0;

    for (int i = 0; i < n; i++)
    {
        fprintf(fp,
                "%d|%d|%d|%s|%s|%.2lf|%.2lf|%.2lf|%d\n",
                arr[i].id,
                arr[i].roomNo,
                arr[i].customerId,
                arr[i].checkin,
                arr[i].checkout,
                arr[i].extrasTotal,
                arr[i].roomTotal,
                arr[i].grandTotal,
                arr[i].isActive);
    }

    fclose(fp);
    return 1;
}

static int overwriteRooms(Room *arr, int n){
    FILE *fp = fopen(ROOMS_FILE, "w");
    if (fp == NULL)
        return 0;

    for (int i = 0; i < n; i++)
    {
        fprintf(fp, "%d %d %.2lf %d\n",
                arr[i].roomNo,
                arr[i].type,
                arr[i].pricePerNight,
                arr[i].status);
    }

    fclose(fp);
    return 1;
}

static int appendBooking(const Booking *b)
{
    FILE *fp=fopen(BOOKINGS_FILE,"a");
    if(!fp)
        return 0;

    fprintf(fp,"%d %d %d %s %s %.2lf %.2lf %.2lf %d\n",
            b->id,
            b->roomNo,
            b->customerId,
            b->checkin,
            b->checkout,
            b->extrasTotal,
            b->roomTotal,
            b->grandTotal,
            b->isActive);

    fclose(fp);
    return 1;
}

static int appendService(const ServiceItem *s){
    FILE *fp=fopen(SERVICES_FILE,"a");
    if(!fp)
        return 0;

    fprintf(fp,"%d %s %.2lf\n",
            s->id,
            s->name,
            s->price);

    fclose(fp);
    return 1;
}

static int appendUser(const User *u){
    FILE *fp=fopen(USERS_FILE,"a");
    if(!fp)
        return 0;

    fprintf(fp,"%d %s %s %d\n",
            u->id,
            u->username,
            u->password,
            u->role);

    fclose(fp);
    return 1;
}

static int appendRevenue(const Revenue *r){
    FILE *fp = fopen(REVENUE_FILE, "a");
    if (fp == NULL)
        return 0;

    fprintf(fp,
            "%d|%s|%.2lf\n",
            r->bookingId,
            r->date,
            r->amount);

    fclose(fp);
    return 1;
}

// -----------------------------
// ID generators (scan files to find max id)
// -----------------------------

static int nextCustomerId()
{
    FILE *fp = fopen(CUSTOMERS_FILE,"r");

    if(fp==NULL)
        return 1;

    Customer c;
    int maxId=0;

    while(fscanf(fp,"%d|%63[^|]|%23[^|]|%31[^|]|%127[^\n]\n",
                 &c.id,
                 c.name,
                 c.phone,
                 c.govId,
                 c.address)==5)
    {
        if(c.id>maxId)
            maxId=c.id;
    }

    fclose(fp);
    return maxId+1;
}

static int nextBookingId()
{
    FILE *fp=fopen(BOOKINGS_FILE,"r");

    if(fp==NULL)
        return 1;

    Booking b;
    int maxId=0;

    while(fscanf(fp,"%d %d %d %10s %10s %lf %lf %lf %d",
                 &b.id,
                 &b.roomNo,
                 &b.customerId,
                 b.checkin,
                 b.checkout,
                 &b.extrasTotal,
                 &b.roomTotal,
                 &b.grandTotal,
                 &b.isActive)==9)
    {
        if(b.id>maxId)
            maxId=b.id;
    }

    fclose(fp);
    return maxId+1;
}

static int nextServiceId()
{
    FILE *fp=fopen(SERVICES_FILE,"r");

    if(fp==NULL)
        return 1;

    ServiceItem s;
    int maxId=0;

    while(fscanf(fp,"%d %63s %lf",
                 &s.id,
                 s.name,
                 &s.price)==3)
    {
        if(s.id>maxId)
            maxId=s.id;
    }

    fclose(fp);
    return maxId+1;
}

static int nextUserId()
{
    FILE *fp=fopen(USERS_FILE,"r");

    if(fp==NULL)
        return 1;

    User u;
    int maxId=0;

    while(fscanf(fp,"%d %31s %63s %d",
                 &u.id,
                 u.username,
                 u.password,
                 (int*)&u.role)==4)
    {
        if(u.id>maxId)
            maxId=u.id;
    }

    fclose(fp);
    return maxId+1;
}

// -----------------------------
// Room management
// -----------------------------

static const char *roomTypeStr(RoomType t)
{
    switch (t)
    {
    case ROOM_STANDARD:
        return "Standard";
    case ROOM_DELUXE:
        return "Deluxe";
    case ROOM_SUITE:
        return "Suite";
    }
    return "?";
}

static const char *roomStatusStr(RoomStatus s)
{
    switch (s)
    {
    case ROOM_FREE:
        return "Free";
    case ROOM_OCCUPIED:
        return "Occupied";
    case ROOM_RESERVED:
        return "Reserved";
    }
    return "?";
}

static Room *room_get(int roomNo)
{
    Room *arr = NULL;
    int n = readAllRooms(&arr);
    int i;
    for (i = 0; i < n; i++)
        if (arr[i].roomNo == roomNo)
        {
            Room *r = (Room *)malloc(sizeof(Room));
            *r = arr[i];
            free(arr);
            return r;
        }
    free(arr);
    return NULL;
}

static int room_updateRecord(const Room *r)
{
    Room *arr = NULL;
    int n = readAllRooms(&arr);

    int k = -1;

    for (int i = 0; i < n; i++)
    {
        if (arr[i].roomNo == r->roomNo)
        {
            k = i;
            break;
        }
    }

    if (k == -1)
    {
        free(arr);
        return 0;
    }

    arr[k] = *r;

    int ok = overwriteRooms(arr, n);

    free(arr);

    return ok;
}

void room_add(){
    Room r;

    printf("Enter room number: ");
    scanf("%d", &r.roomNo);

    Room *arr = NULL;
    int n = readAllRooms(&arr);

    for (int i = 0; i < n; i++)
    {
        if (arr[i].roomNo == r.roomNo)
        {
            printf("Room already exists.\n");
            free(arr);
            return;
        }
    }

    free(arr);

    printf("Room type (1-Standard,2-Deluxe,3-Suite): ");
    scanf("%d", (int *)&r.type);

    printf("Price per night: ");
    scanf("%lf", &r.pricePerNight);

    r.status = ROOM_FREE;

    FILE *fp = fopen(ROOMS_FILE, "a");

    if (fp == NULL)
    {
        printf("Cannot open file.\n");
        return;
    }

    fprintf(fp, "%d %d %.2lf %d\n",
            r.roomNo,
            r.type,
            r.pricePerNight,
            r.status);

    fclose(fp);

    printf("Room added successfully.\n");
}

static void room_list_available()
{
    Room *arr = NULL;
    int n = readAllRooms(&arr);
    printf("\nAvailable Rooms:\n");
    printf("%-8s %-10s %-10s %-10s\n", "RoomNo", "Type", "Price", "Status");
    for (int i = 0; i < n; i++)
        if (arr[i].status == ROOM_FREE)
            printf("%-8d %-10s %-10.2f %-10s\n", arr[i].roomNo, roomTypeStr(arr[i].type), arr[i].pricePerNight, roomStatusStr(arr[i].status));
    free(arr);
}

static void room_update(){
    int rno;
    printf("\nEnter room number to update: ");
    scanf("%d", &rno);
    Room *arr = NULL;
    int n = readAllRooms(&arr);
    int found = -1;
    for (int i = 0; i < n; i++)
        if (arr[i].roomNo == rno)
        {
            found = i;
            break;
        }
    if (found == -1)
    {
        printf("Room not found.\n");
        free(arr);
        return;
    }

    printf("Update (1)Price (2)Type (3)Status : ");
    int ch;
    scanf("%d", &ch);
    if (ch == 1)
    {
        printf("New price: ");
        scanf("%lf", &arr[found].pricePerNight);
    }
    else if (ch == 2)
    {
        int t;
        printf("Type 1/2/3: ");
        scanf("%d", &t);
        if (t >= 1 && t <= 3)
            arr[found].type = (RoomType)t;
    }
    else if (ch == 3)
    {
        int s;
        printf("Status 0-Free,1-Occ,2-Res: ");
        scanf("%d", &s);
        if (s >= 0 && s <= 2)
            arr[found].status = (RoomStatus)s;
    }

    if (overwriteRooms(arr, n))
        printf("Updated.\n");
    else
        printf("Failed.\n");
    free(arr);
}

static void room_delete()
{
    int rno;
    printf("\nEnter room number to delete: ");
    scanf("%d", &rno);
    Room *arr = NULL;
    int n = readAllRooms(&arr);
    if (n == 0)
    {
        printf("No rooms.\n");
        return;
    }
    int keep = 0;
    for (int i = 0; i < n; i++)
        if (arr[i].roomNo != rno)
            arr[keep++] = arr[i];
    if (keep == n)
    {
        printf("Room not found or cannot delete.\n");
        free(arr);
        return;
    }
    if (overwriteRooms(arr, keep))
        printf("Deleted.\n");
    else
        printf("Failed.\n");
    free(arr);
}

// -----------------------------
// Customer management
// -----------------------------

static void customer_add(){
    Customer c;
    memset(&c, 0, sizeof(c));

    c.id = nextCustomerId();

    printf("\nName: ");
    getchar();
    fgets(c.name, sizeof(c.name), stdin);
    c.name[strcspn(c.name, "\n")] = '\0';

    printf("Phone: ");
    fgets(c.phone, sizeof(c.phone), stdin);
    c.phone[strcspn(c.phone, "\n")] = '\0';

    printf("Gov ID: ");
    fgets(c.govId, sizeof(c.govId), stdin);
    c.govId[strcspn(c.govId, "\n")] = '\0';

    printf("Address: ");
    fgets(c.address, sizeof(c.address), stdin);
    c.address[strcspn(c.address, "\n")] = '\0';

    if (appendCustomer(&c))
        printf("Customer added. ID=%d\n", c.id);
    else
        printf("Failed to add customer.\n");
}

static void customer_view()
{
    int id;
    printf("\nEnter customer ID: ");
    scanf("%d", &id);
    Customer *arr = NULL;
    int n = readAllCustomers(&arr);
    int i;
    for (i = 0; i < n; i++)
        if (arr[i].id == id)
        {
            printf("ID:%d\nName:%s\nPhone:%s\nGovID:%s\nAddress:%s\n", arr[i].id, arr[i].name, arr[i].phone, arr[i].govId, arr[i].address);
            free(arr);
            return;
        }
    printf("Not found.\n");
    free(arr);
}

static void customer_update(){
    int id;
    printf("\nCustomer ID to update: ");
    scanf("%d", &id);
    Customer *arr = NULL;
    int n = readAllCustomers(&arr);
    int k = -1;
    for (int i = 0; i < n; i++)
        if (arr[i].id == id)
        {
            k = i;
            break;
        }
    if (k == -1)
    {
        printf("Not found.\n");
        free(arr);
        return;
    }

    printf("Update: 1-Name 2-Phone 3-GovID 4-Address: ");
    int ch;
    scanf("%d", &ch);
    getchar();
    if (ch == 1)
    {
        printf("New name: ");
        fgets(arr[k].name, sizeof(arr[k].name), stdin);
        arr[k].name[strcspn(arr[k].name, "\n")] = '\0';
    }
    else if (ch == 2)
    {
        printf("New phone: ");
        fgets(arr[k].phone, sizeof(arr[k].phone), stdin);
        arr[k].phone[strcspn(arr[k].phone, "\n")] = '\0';
    }
    else if (ch == 3)
    {
        printf("New GovID: ");
        fgets(arr[k].govId, sizeof(arr[k].govId), stdin);
        arr[k].govId[strcspn(arr[k].govId, "\n")] = '\0';
    }
    else if (ch == 4)
    {
        printf("New address: ");
        fgets(arr[k].address, sizeof(arr[k].address), stdin);
        arr[k].address[strcspn(arr[k].address, "\n")] = '\0';
    }

    if (overwriteCustomers(arr, n))
        printf("Updated.\n");
    else
        printf("Failed.\n");
    free(arr);
}

static void customer_delete(){
    int id;
    printf("\nCustomer ID to delete: ");
    scanf("%d", &id);
    Customer *arr = NULL;
    int n = readAllCustomers(&arr);
    if (n == 0)
    {
        printf("No customers.\n");
        return;
    }
    int keep = 0;
    for (int i = 0; i < n; i++)
        if (arr[i].id != id)
            arr[keep++] = arr[i];
    if (keep == n)
    {
        printf("Not found.\n");
        free(arr);
        return;
    }
    if (overwriteCustomers(arr, keep))
        printf("Deleted.\n");
    else
        printf("Failed.\n");
    free(arr);
}

// -----------------------------
// Services (Food/Extras)
// -----------------------------

static void service_add(){
    ServiceItem s;
    memset(&s, 0, sizeof(s));

    s.id = nextServiceId();

    printf("\nService name: ");
    getchar();

    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = '\0';

    printf("Price: ");
    scanf("%lf", &s.price);

    if (appendService(&s))
        printf("Service added. ID=%d\n", s.id);
    else
        printf("Failed.\n");
}

static void service_list(){
    ServiceItem *arr = NULL;
    int n = readAllServices(&arr);
    printf("\nServices:\n%-5s %-20s %-10s\n", "ID", "Name", "Price");
    int i;
    for (i = 0; i < n; i++)
        printf("%-5d %-20s %-10.2f\n", arr[i].id, arr[i].name, arr[i].price);
    free(arr);
}

static ServiceItem *service_findById(int id)
{
    ServiceItem *arr = NULL;
    int n = readAllServices(&arr);
    for (int i = 0; i < n; i++)
        if (arr[i].id == id)
        {
            ServiceItem *s = (ServiceItem *)malloc(sizeof(ServiceItem));
            *s = arr[i];
            free(arr);
            return s;
        }
    free(arr);
    return NULL;
}

// -----------------------------
// Bookings & Billing
// -----------------------------

static int customer_exists(int cid)
{
    Customer *arr = NULL;
    int n = readAllCustomers(&arr);
    for (int i = 0; i < n; i++)
        if (arr[i].id == cid)
        {
            free(arr);
            return 1;
        }
    free(arr);
    return 0;
}

static Booking *booking_getActiveByRoom(int roomNo)
{
    Booking *arr = NULL;
    int n = readAllBookings(&arr);
    for (int i = 0; i < n; i++)
        if (arr[i].roomNo == roomNo && arr[i].isActive)
        {
            Booking *b = (Booking *)malloc(sizeof(Booking));
            *b = arr[i];
            free(arr);
            return b;
        }
    free(arr);
    return NULL;
}

static int booking_updateRecord(const Booking *bk)
{
    Booking *arr = NULL;
    int n = readAllBookings(&arr);
    int k = -1;
    for (int i = 0; i < n; i++)
        if (arr[i].id == bk->id)
        {
            k = i;
            break;
        }
    if (k == -1)
    {
        free(arr);
        return 0;
    }
    arr[k] = *bk;
    int ok = overwriteBookings(arr, n);
    free(arr);
    return ok;
}

static void booking_checkin()
{
    int roomNo, customerId;
    char inDate[11];
    printf("\nRoom No: ");
    scanf("%d", &roomNo);
    printf("Customer ID: ");
    scanf("%d", &customerId);
    printf("Check-in date (YYYY-MM-DD): ");
    scanf("%10s", inDate);

    if (!customer_exists(customerId))
    {
        printf("Customer not found.\n");
        return;
    }
    Room *r = room_get(roomNo);
    if (!r)
    {
        printf("Room not found.\n");
        return;
    }
    if (r->status != ROOM_FREE)
    {
        printf("Room is not free.\n");
        free(r);
        return;
    }

    Booking b;
    memset(&b, 0, sizeof(b));
    b.id = nextBookingId();
    b.roomNo = roomNo;
    b.customerId = customerId;
    strncpy(b.checkin, inDate, 10);
    b.checkin[10] = '\0';
    b.checkout[0] = '\0';
    b.extrasTotal = 0.0;
    b.roomTotal = 0.0;
    b.grandTotal = 0.0;
    b.isActive = 1;

    if (!appendBooking(&b))
    {
        printf("Failed to save booking.\n");
        free(r);
        return;
    }
    r->status = ROOM_OCCUPIED;
    room_updateRecord(r);
    free(r);
    printf("Checked in. Booking ID=%d\n", b.id);
}

static void booking_order_service()
{
    int roomNo;
    printf("\nRoom No (active booking): ");
    scanf("%d", &roomNo);
    Booking *b = booking_getActiveByRoom(roomNo);
    if (!b)
    {
        printf("No active booking for this room.\n");
        return;
    }
    service_list();
    printf("Enter service ID to add (0 to cancel): ");
    int sid;
    scanf("%d", &sid);
    if (sid == 0)
    {
        free(b);
        return;
    }
    ServiceItem *s = service_findById(sid);
    if (!s)
    {
        printf("Service not found.\n");
        free(b);
        return;
    }
    b->extrasTotal += s->price;
    if (booking_updateRecord(b))
        printf("Added '%s' (%.2f). New extras: %.2f\n", s->name, s->price, b->extrasTotal);
    else
        printf("Failed to update booking.\n");
    free(s);
    free(b);
}

static void booking_checkout()
{
    int roomNo;
    char outDate[11];
    printf("\nRoom No: ");
    scanf("%d", &roomNo);
    printf("Checkout date (YYYY-MM-DD): ");
    scanf("%10s", outDate);

    Booking *b = booking_getActiveByRoom(roomNo);
    if (!b)
    {
        printf("No active booking for room.\n");
        return;
    }

    int nights = daysBetween(b->checkin, outDate);
    if (nights < 1)
    {
        printf("Invalid checkout date.\n");
        free(b);
        return;
    }

    Room *r = room_get(roomNo);
    if (!r)
    {
        printf("Room not found.\n");
        free(b);
        return;
    }

    b->roomTotal = nights * r->pricePerNight;
    strncpy(b->checkout, outDate, 10);
    b->checkout[10] = '\0';
    b->grandTotal = b->roomTotal + b->extrasTotal;
    b->isActive = 0;

    if (!booking_updateRecord(b))
    {
        printf("Failed to finalize booking.\n");
        free(b);
        free(r);
        return;
    }

    // free room
    r->status = ROOM_FREE;
    room_updateRecord(r);

    // add to revenue
    Revenue rv;
    rv.bookingId = b->id;
    strncpy(rv.date, outDate, 11);
    rv.amount = b->grandTotal;
    appendRevenue(&rv);

    printf("\n===== BILL RECEIPT =====\n");
    printf("Booking ID: %d\nRoom: %d (%s)\nCheck-in: %s\nCheck-out: %s\nNights: %d\nRoom total: %.2f\nExtras: %.2f\nGrand Total: %.2f\n",
           b->id, b->roomNo, roomTypeStr(r->type), b->checkin, b->checkout, nights, b->roomTotal, b->extrasTotal, b->grandTotal);

    // Save printable receipt
    char fname[64];
    sprintf(fname, "receipt_%d.txt", b->id);
    FILE *rf = fopen(fname, "w");
    if (rf)
    {
        fprintf(rf, "HOTEL BILL RECEIPT\nBookingID: %d\nRoom: %d (%s)\nCheck-in: %s\nCheck-out: %s\nNights: %d\nRoom total: %.2f\nExtras: %.2f\nGrand Total: %.2f\n",
                b->id, b->roomNo, roomTypeStr(r->type), b->checkin, b->checkout, nights, b->roomTotal, b->extrasTotal, b->grandTotal);
        fclose(rf);
        printf("Receipt saved: %s\n", fname);
    }

    free(b);
    free(r);
}

static void bookings_list_all()
{
    Booking *arr = NULL;
    int n = readAllBookings(&arr);
    printf("\nAll Bookings:\n");
    printf("%-6s %-6s %-10s %-10s %-10s %-8s %-8s %-8s %-6s\n", "ID", "Room", "CustID", "CheckIn", "CheckOut", "Extras", "Room$", "Total", "Actv");
    for (int i = 0; i < n; i++)
    {
        printf("%-6d %-6d %-10d %-10s %-10s %-8.2f %-8.2f %-8.2f %-6d\n", arr[i].id, arr[i].roomNo, arr[i].customerId, arr[i].checkin,
               arr[i].checkout[0] ? arr[i].checkout : "-", arr[i].extrasTotal, arr[i].roomTotal, arr[i].grandTotal, arr[i].isActive);
    }
    free(arr);
}

// -----------------------------
// Searches & Reports
// -----------------------------

static void search_customer()
{
    int mode;
    printf("\nSearch by: 1-Name 2-Phone 3-GovID: ");
    scanf("%d", &mode);
    getchar();
    char key[64];
    printf("Enter search text: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0';

    Customer *arr = NULL;
    int n = readAllCustomers(&arr);
    printf("%-5s %-20s %-15s %-15s\n", "ID", "Name", "Phone", "GovID");
    for (int i = 0; i < n; i++)
    {
        int hit = 0;
        if (mode == 1 && strcasestr_custom(arr[i].name, key))
            hit = 1;
        else if (mode == 2 && strcasestr_custom(arr[i].phone, key))
            hit = 1;
        else if (mode == 3 && strcasestr_custom(arr[i].govId, key))
            hit = 1;
        if (hit)
            printf("%-5d %-20s %-15s %-15s\n", arr[i].id, arr[i].name, arr[i].phone, arr[i].govId);
    }
    free(arr);
}

static void search_room()
{
    int mode;
    printf("\nSearch room by: 1-Type 2-Price<= 3-Availability: ");
    scanf("%d", &mode);
    Room *arr = NULL;
    int n = readAllRooms(&arr);
    if (mode == 1)
    {
        int t;
        printf("Type (1/2/3): ");
        scanf("%d", &t);
        for (int i = 0; i < n; i++)
            if (arr[i].type == t)
                printf("Room %d %s Price:%.2f Status:%s\n", arr[i].roomNo, roomTypeStr(arr[i].type), arr[i].pricePerNight, roomStatusStr(arr[i].status));
    }
    else if (mode == 2)
    {
        double p;
        printf("Max price: ");
        scanf("%lf", &p);
        for (int i = 0; i < n; i++)
            if (arr[i].pricePerNight <= p)
                printf("Room %d %s Price:%.2f Status:%s\n", arr[i].roomNo, roomTypeStr(arr[i].type), arr[i].pricePerNight, roomStatusStr(arr[i].status));
    }
    else if (mode == 3)
    {
        int s;
        printf("Status (0-Free,1-Occ,2-Res): ");
        scanf("%d", &s);
        for (int i = 0; i < n; i++)
            if (arr[i].status == s)
                printf("Room %d %s Price:%.2f Status:%s\n", arr[i].roomNo, roomTypeStr(arr[i].type), arr[i].pricePerNight, roomStatusStr(arr[i].status));
    }
    free(arr);
}

static void report_occupancy()
{
    Room *arr = NULL;
    int n = readAllRooms(&arr);
    int freeC = 0, occC = 0, resC = 0;
    for (int i = 0; i < n; i++)
    {
        if (arr[i].status == ROOM_FREE)
            freeC++;
        else if (arr[i].status == ROOM_OCCUPIED)
            occC++;
        else if (arr[i].status == ROOM_RESERVED)
            resC++;
    }
    printf("\nOccupancy Report:\nFree: %d\nOccupied: %d\nReserved: %d\nTotal: %d\n", freeC, occC, resC, n);
    free(arr);
}

static void report_revenue()
{
    Revenue *arr = NULL;
    int n = readAllRevenue(&arr);
    double total = 0;
    for (int i = 0; i < n; i++)
        total += arr[i].amount;
    printf("\nTotal revenue (all time): %.2f\n", total);
    free(arr);
}

// -----------------------------
// Authentication
// -----------------------------

static User *user_findByUsername(const char *u)
{
    User *arr = NULL;
    int n = readAllUsers(&arr);
    for (int i = 0; i < n; i++)
        if (strcmp(arr[i].username, u) == 0)
        {
            User *x = (User *)malloc(sizeof(User));
            *x = arr[i];
            free(arr);
            return x;
        }
    free(arr);
    return NULL;
}

static int authenticate(User *out)
{
    char username[32], pw[64];
    printf("\nUsername: ");
    scanf("%31s", username);
    printf("Password: ");
    scanf("%63s", pw);
    User *u = user_findByUsername(username);
    if (!u)
    {
        printf("No such user.\n");
        return 0;
    }
    if (strcmp(u->password, pw) != 0)
    {
        printf("Wrong password.\n");
        free(u);
        return 0;
    }
    *out = *u;
    free(u);
    return 1;
}

static void register_user(){
    User u;
    memset(&u, 0, sizeof(u));

    u.id = nextUserId();

    printf("\nNew username: ");
    scanf("%31s", u.username);

    User *exist = user_findByUsername(u.username);

    if (exist)
    {
        printf("Username already exists.\n");
        free(exist);
        return;
    }

    printf("Password: ");
    scanf("%63s", u.password);

    printf("Role (1-Admin,2-Receptionist): ");

    int r;
    scanf("%d", &r);

    if (r != 1 && r != 2)
    {
        printf("Invalid role.\n");
        return;
    }

    u.role = (UserRole)r;

    if (appendUser(&u))
        printf("User created. ID=%d\n", u.id);
    else
        printf("Failed.\n");
}

static void ensure_default_admin(){
    User *arr = NULL;

    int n = readAllUsers(&arr);

    if (n == 0)
    {
        User u;

        memset(&u, 0, sizeof(u));

        u.id = 1;

        strcpy(u.username, "admin");
        strcpy(u.password, "admin");

        u.role = ROLE_ADMIN;

        appendUser(&u);

        printf("[Init] Default admin created (admin/admin)\n");
    }

    free(arr);
}

// -----------------------------
// Backup/Restore (simple copies)
// -----------------------------

static int copyFile(const char *src, const char *dst){
    FILE *s = fopen(src, "r");
    if (!s)
        return 0;

    FILE *d = fopen(dst, "w");
    if (!d)
    {
        fclose(s);
        return 0;
    }

    char buf[4096];
    size_t r;

    while ((r = fread(buf, 1, sizeof(buf), s)) > 0)
    {
        fwrite(buf, 1, r, d);
    }

    fclose(s);
    fclose(d);

    return 1;
}

static void backup_data()
{
#ifdef _WIN32
    system("if not exist backup mkdir backup");
#else
    system("mkdir -p backup");
#endif
    copyFile(ROOMS_FILE, "backup/rooms.bak");
    copyFile(CUSTOMERS_FILE, "backup/customers.bak");
    copyFile(BOOKINGS_FILE, "backup/bookings.bak");
    copyFile(USERS_FILE, "backup/users.bak");
    copyFile(SERVICES_FILE, "backup/services.bak");
    copyFile(REVENUE_FILE, "backup/revenue.bak");
    printf("Backup complete (./backup).\n");
}

static void restore_data()
{
    copyFile("backup/rooms.bak", ROOMS_FILE);
    copyFile("backup/customers.bak", CUSTOMERS_FILE);
    copyFile("backup/bookings.bak", BOOKINGS_FILE);
    copyFile("backup/users.bak", USERS_FILE);
    copyFile("backup/services.bak", SERVICES_FILE);
    copyFile("backup/revenue.bak", REVENUE_FILE);
    printf("Restore attempted from ./backup (if files existed).\n");
}

// -----------------------------
// Menus
// -----------------------------

static void menu_room()
{
    int ch;
    do
    {
        printf("\n-- Room Management --\n1. Add Room\n2. View Available Rooms\n3. Update Room\n4. Delete Room\n0. Back\nChoice: ");
        scanf("%d", &ch);
        switch (ch)
        {
        case 1:
            room_add();
            break;
        case 2:
            room_list_available();
            break;
        case 3:
            room_update();
            break;
        case 4:
            room_delete();
            break;
        }
    } while (ch != 0);
}

static void menu_customer()
{
    int ch;
    do
    {
        printf("\n-- Customer Management --\n1. Add Customer\n2. View Customer\n3. Update Customer\n4. Delete Customer\n0. Back\nChoice: ");
        scanf("%d", &ch);
        switch (ch)
        {
        case 1:
            customer_add();
            break;
        case 2:
            customer_view();
            break;
        case 3:
            customer_update();
            break;
        case 4:
            customer_delete();
            break;
        }
    } while (ch != 0);
}

static void menu_services()
{
    int ch;
    do
    {
        printf("\n-- Food/Service --\n1. Add Service Item\n2. List Service Items\n0. Back\nChoice: ");
        scanf("%d", &ch);
        switch (ch)
        {
        case 1:
            service_add();
            break;
        case 2:
            service_list();
            break;
        }
    } while (ch != 0);
}

static void menu_search_reports()
{
    int ch;
    do
    {
        printf("\n-- Search & Reports --\n1. Search Customer\n2. Search Room\n3. Occupancy Report\n4. Revenue Report\n0. Back\nChoice: ");
        scanf("%d", &ch);
        switch (ch)
        {
        case 1:
            search_customer();
            break;
        case 2:
            search_room();
            break;
        case 3:
            report_occupancy();
            break;
        case 4:
            report_revenue();
            break;
        }
    } while (ch != 0);
}

static void menu_booking_reception()
{
    int ch;
    do
    {
        printf("\n-- Booking & Billing --\n1. Check-in\n2. Order Food/Service\n3. Check-out (Bill)\n4. View All Bookings\n0. Back\nChoice: ");
        scanf("%d", &ch);
        switch (ch)
        {
        case 1:
            booking_checkin();
            break;
        case 2:
            booking_order_service();
            break;
        case 3:
            booking_checkout();
            break;
        case 4:
            bookings_list_all();
            break;
        }
    } while (ch != 0);
}

static void admin_menu()
{
    int ch;
    do
    {
        printf("\n==== ADMIN MENU ====\n1. Room Management\n2. Customer Management\n3. Booking & Billing\n4. Food/Service Catalog\n5. Search & Reports\n6. Backup Data\n7. Restore Data\n8. Register User\n0. Logout\nChoice: ");
        scanf("%d", &ch);
        switch (ch)
        {
        case 1:
            menu_room();
            break;
        case 2:
            menu_customer();
            break;
        case 3:
            menu_booking_reception();
            break;
        case 4:
            menu_services();
            break;
        case 5:
            menu_search_reports();
            break;
        case 6:
            backup_data();
            break;
        case 7:
            restore_data();
            break;
        case 8:
            register_user();
            break;
        }
    } while (ch != 0);
}

static void reception_menu()
{
    int ch;
    do
    {
        printf("\n==== RECEPTION MENU ====\n1. View Available Rooms\n2. Booking & Billing\n3. Search & Reports (basic)\n0. Logout\nChoice: ");
        scanf("%d", &ch);
        switch (ch)
        {
        case 1:
            room_list_available();
            break;
        case 2:
            menu_booking_reception();
            break;
        case 3:
            menu_search_reports();
            break;
        }
    } while (ch != 0);
}

int main()
{
    ensure_default_admin();
    int choice;
    while (1)
    {
        printf("\n====== HOTEL MANAGEMENT (FOR ADMIN/RECEPTIONIST) ======\n1. Login\n2. Register User\n0. Exit\nChoice: ");
        if (scanf("%d", &choice) != 1)
            break;
        if (choice == 0)
            break;
        if (choice == 2)
        {
            register_user();
        }
        else if (choice == 1)
        {
            User u;
            if (authenticate(&u))
            {
                system(CLEAR);
                printf("Welcome, %s!\n", u.username);
                if (u.role == ROLE_ADMIN)
                    admin_menu();
                else
                    reception_menu();
            }
        }
    }
    printf("\nGoodbye!\n");
    return 0;
}