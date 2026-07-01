#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

/* ---------- PRODUCT STRUCT ---------- */
struct Product {
    int id;
    char name[30];
    float price;
    int qty;
};

/* ---------- LINKED LIST ---------- */
struct Node {
    struct Product p;
    struct Node *next;
};

struct Node *head = NULL;

/* ---------- STACK (CART) ---------- */
struct Product cart[MAX];
int top = -1;

/* ---------- QUEUE (ORDERS) ---------- */
struct Product orderQueue[MAX];
int front = 0, rear = -1;

/* ---------- ARRAY STORAGE ---------- */
struct Product productArray[MAX];
int productCount = 0;

/* ---------- FUNCTION DECLARATIONS ---------- */
void loadFromFile();
void saveToFile();
void addProduct();
void viewProducts();
void searchProduct();
void updateProduct();
void deleteProduct();

/* ---------- STACK FUNCTIONS ---------- */
void pushCart(struct Product p);
void popCart();
void viewCart();

/* ---------- QUEUE FUNCTIONS ---------- */
void enqueueOrder(struct Product p);
void viewOrders();

/* ---------- FILE FUNCTIONS ---------- */
void loadFromFile() {
    FILE *fp = fopen("products.txt", "r");
    struct Product p;
    productCount = 0;

    if (!fp) return;

    while (fscanf(fp,
              "%d|%29[^|]|%f|%d",
              &p.id,
              p.name,
              &p.price,
              &p.qty) == 4){
       productArray[productCount++] = p;
    }
    fclose(fp);
}

void saveToFile() {
    FILE *fp = fopen("products.txt", "w");
    struct Product p;
    if (fp == NULL) {
        printf("Cannot open file.\n");
        return;
    }
    for (int i = 0; i < productCount; i++) {
        fprintf(fp,"%d|%s|%.2f|%d\n",
            p.id,
            p.name,
            p.price,
            p.qty);
    }

    fclose(fp);
}

/* ---------- ADMIN FUNCTIONS ---------- */
void addProduct() {
    struct Product p;

    FILE *fp = fopen("products.txt", "a");
    if (fp == NULL) {
        printf("File cannot be opened!\n");
        return;
    }

    printf("Enter Product ID: ");
    scanf("%d", &p.id);
    getchar();

    printf("Enter Product Name: ");
    fgets(p.name, sizeof(p.name), stdin);
    p.name[strcspn(p.name, "\n")] = '\0';

    printf("Enter Product Price: ");
    scanf("%f", &p.price);

    printf("Enter Product Quantity: ");
    scanf("%d", &p.qty);

    fprintf(fp, "%d|%s|%.2f|%d\n",
            p.id, p.name, p.price, p.qty);

    fclose(fp);

    printf("Product Added Successfully.\n");
}

void viewProducts() {
    struct Product p;

    FILE *fp = fopen("products.txt", "r");

    if (fp == NULL) {
        printf("No Products Found.\n");
        return;
    }

    printf("\n%-5s %-25s %-15s %-10s\n",
           "ID", "Name", "Price", "Qty");
    printf("----------------------------------------------------\n");

    while (fscanf(fp,
              "%d|%29[^|]|%f|%d",
              &p.id,
              p.name,
              &p.price,
              &p.qty) == 4) {

        printf("%-5d %-25s %-15.2f %-10d\n",
               p.id,
               p.name,
               p.price,
               p.qty);
    }

    fclose(fp);
}

void searchProduct() {
    struct Product p;
    int id, found = 0;

    FILE *fp = fopen("products.txt", "r");

    if (fp == NULL) {
        printf("File Not Found.\n");
        return;
    }

    printf("Enter Product ID: ");
    scanf("%d", &id);

    while (fscanf(fp,
              "%d|%29[^|]|%f|%d",
              &p.id,
              p.name,
              &p.price,
              &p.qty) == 4){

        if (p.id == id) {
            printf("\nProduct Found\n");
            printf("ID : %d\n", p.id);
            printf("Name : %s\n", p.name);
            printf("Price : %.2f\n", p.price);
            printf("Quantity : %d\n", p.qty);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Product Not Found.\n");

    fclose(fp);
}

void updateProduct() {
    struct Product p;
    int id, found = 0;

    FILE *fp = fopen("products.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (fp == NULL) {
        printf("File Not Found.\n");
        return;
    }

    printf("Enter Product ID to Update: ");
    scanf("%d", &id);

    while (fscanf(fp,
        "%d|%29[^|]|%f|%d",
        &p.id,
        p.name,
        &p.price,
        &p.qty) == 4){

        if (p.id == id) {
            found = 1;

            printf("Enter New Price: ");
            scanf("%f", &p.price);

            printf("Enter New Quantity: ");
            scanf("%d", &p.qty);
        }

        fprintf(temp, "%d|%s|%.2f|%d\n",
                p.id, p.name, p.price, p.qty);
    }

    fclose(fp);
    fclose(temp);

    remove("products.txt");
    rename("temp.txt", "products.txt");

    if (found)
        printf("Product Updated Successfully.\n");
    else
        printf("Product Not Found.\n");
}

void deleteProduct() {
    struct Product p;
    int id, found = 0;

    FILE *fp = fopen("products.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (fp == NULL) {
        printf("File Not Found.\n");
        return;
    }

    printf("Enter Product ID to Delete: ");
    scanf("%d", &id);

    while (fscanf(fp,
              "%d|%29[^|]|%f|%d",
              &p.id,
              p.name,
              &p.price,
              &p.qty) == 4){

        if (p.id == id) {
            found = 1;
            continue;
        }

        fprintf(temp, "%d|%s|%.2f|%d\n",
                p.id, p.name, p.price, p.qty);
    }

    fclose(fp);
    fclose(temp);

    remove("products.txt");
    rename("temp.txt", "products.txt");

    if (found)
        printf("Product Deleted Successfully.\n");
    else
        printf("Product Not Found.\n");
}

/* ---------- STACK (CART) ---------- */
void pushCart(struct Product p) {
    if (top == MAX - 1) {
        printf("Cart is Full!\n");
        return;
    }

    cart[++top] = p;
    printf("%s added to cart.\n", p.name);
}

void popCart() {
    if (top == -1) {
        printf("Cart is Empty!\n");
        return;
    }

    printf("%s removed from cart.\n", cart[top].name);
    top--;
}

void viewCart() {
    if (top == -1) {
        printf("Cart is Empty!\n");
        return;
    }

    float total = 0;

    printf("\n----- CART -----\n");
    printf("ID\tName\t\tPrice\tQty\n");

    for (int i = top; i >= 0; i--) {
        printf("%-5d %-10s %12.2f %4d\n",
               cart[i].id,
               cart[i].name,
               cart[i].price,
               cart[i].qty);

        total += cart[i].price * cart[i].qty;
    }

    printf("------------------------------\n");
    printf("Total Amount = %.2f\n", total);
}

/* ---------- QUEUE (ORDER) ---------- */
void enqueueOrder(struct Product p) {
    if (rear == MAX - 1) {
        printf("Order Queue Full!\n");
        return;
    }

    orderQueue[++rear] = p;
}

void viewOrders() {
    if (front > rear) {
        printf("No Orders Placed.\n");
        return;
    }

    printf("\n----- ORDERS -----\n");
    printf("ID\tName\t\tPrice\tQty\n");

    for (int i = front; i <= rear; i++) {
        printf("%-5d %-10s %12.2f %4d\n",
               orderQueue[i].id,
               orderQueue[i].name,
               orderQueue[i].price,
               orderQueue[i].qty);
    }
}

/* ---------- ADMIN MENU ---------- */
void adminMenu() {
    int ch;
    do {
        printf("\n--- ADMIN MENU ---\n");
        printf("1.Add Product\n2.View Products\n3.Search Product\n");
        printf("4.Update Product\n5.Delete Product\n6.Stock Report\n");
        printf("7.Load File\n8.Save File\n9.View Sales\n10.Logout\n11.choose option :");
        scanf("%d", &ch);

        switch (ch) {
            case 1: addProduct(); break;
            case 2: viewProducts(); break;
            case 3: searchProduct(); break;
            case 4: updateProduct(); break;
            case 5: deleteProduct(); break;
            case 6: viewProducts(); break;
            case 7: printf("Product loaded successfully!\n");
                    loadFromFile(); break;
            case 8: printf("Product saved successfully!\n");
                    saveToFile(); break;
            case 9: viewOrders(); break;
        }
    } while (ch != 10);
}

/* ---------- USER MENU ---------- */
void userMenu() {
    int ch, id , found=0;
    FILE *fp = fopen("products.txt", "r");
        struct Product p;


    do {
        printf("\n--- USER MENU ---\n");
        printf("1.View Products\n2.Search Product\n3.Add to Cart\n");
        printf("4.View Cart\n5.Remove Cart\n6.Generate Bill\n");
        printf("7.Place Order\n8.View Orders\n9.Clear Cart\n10.Logout\n11.choose option :");
        scanf("%d", &ch);

         switch (ch) {

        case 1:viewProducts();
            break;

        case 2:searchProduct();break;

        case 3:printf("Enter Product ID: ");
                scanf("%d", &id);
            if (fp == NULL) {
                            printf("File Not Found.\n");
                            break;
                        }
            while (fscanf(fp,
                          "%d|%29[^|]|%f|%d",
                          &p.id,
                          p.name,
                          &p.price,
                          &p.qty) == 4) {

                if (p.id == id) {
                    pushCart(p);
                    found = 1;
                    break;
                }
            }
            fclose(fp);
            if (!found)printf("Product Not Found.\n");
            break;

        case 4:viewCart();break;

        case 5:popCart();break;

        case 6:printf("Total Bill:\n");viewCart();break;

        case 7:while (top != -1)
                    enqueueOrder(cart[top--]);
                printf("Order Placed Successfully.\n");
                break;

        case 8:viewOrders();break;

        case 9: top = -1;
                printf("Cart Cleared.\n")
                ;break;

        case 10:printf("Logging Out...\n");
                break;

        default:printf("Invalid Choice!\n");
        }
    } while (ch != 10);
}

/* ---------- MAIN ---------- */
int main() {
    int choice;
    loadFromFile();

    do {
        printf("\n==== DEPARTMENT STORE SYSTEM ====\n");
        printf("1.Admin\n2.User\n3.Exit\n");
        scanf("%d", &choice);

        if (choice == 1) adminMenu();
        else if (choice == 2) userMenu();
    } while (choice != 3);

    return 0;
}
