#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Function prototypes
int factorial(int n);
void fibonacci(int n);
int sumOfDigits(int n);
int gcd(int a, int b);
void towerOfHanoi(int n, char from_rod, char to_rod, char aux_rod);
int binarySearch(int arr[], int l, int r, int x);
void swap(int* a, int* b);
int partition(int arr[], int low, int high);
void quickSort(int arr[], int low, int high);
void merge(int arr[], int l, int m, int r);
void mergeSort(int arr[], int l, int r);
int searchElement(int arr[], int n, int x);
struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};
struct Node* createNode(int data);
int findDeepestNodeUtil(struct Node* root, int level, int* maxLevel, int* res);
int findDeepestNode(struct Node* root);
int isSorted(int arr[], int n);
void reverseStackUtil(int *stack, int *top);
void insertAtBottom(int stack[], int* top, int item);
void reverseStack(int stack[], int* top);
int sumOfN(int n);
int findPeak(int arr[], int low, int high, int n);
void permute(char* a, int l, int r);
void findSubsetsUtil(int arr[], int subset[], int n, int index, int sub_index);
void findSubsets(int arr[], int n);
int isPalindrome(char str[], int l, int r);
void dfs(int adj[][10], int V, int start, int visited[]);
int countNodes(struct Node* root);
int treeHeight(struct Node* root);

int main() {
    int choice;
    int arr[100], n, x, a, b, i;
    char str[100];
    int stack[100], top = -1;
    struct Node* root = NULL;
    int adj[10][10] = {0};
    int V = 4;

    do {
        printf("\n--- Menu Driven Program ---\n");
        printf("1. Factorial of a number\n");
        printf("2. Fibonacci sequence\n");
        printf("3. Sum of digits of a number\n");
        printf("4. GCD of two numbers\n");
        printf("5. Tower of Hanoi\n");
        printf("6. Binary search\n");
        printf("7. Quicksort\n");
        printf("8. Merge sort\n");
        printf("9. Search for an element in an array\n");
        printf("10. Find the deepest node in a tree\n");
        printf("11. Check for a sorted array\n");
        printf("12. Reverse a stack\n");
        printf("13. Sum of n consecutive numbers\n");
        printf("14. Find peak element\n");
        printf("15. Print all permutations of a string\n");
        printf("16. Find all subsets of a set\n");
        printf("17. Check if a string is a palindrome\n");
        printf("18. Depth-first search (DFS) traversal of a tree or graph\n");
        printf("19. Counting the number of nodes in a tree\n");
        printf("20. Finding the height of a tree\n");
        printf("21. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter a non-negative number: ");
                scanf("%d", &n);
                printf("Factorial of %d is %d\n", n, factorial(n));
                break;
            case 2:
                printf("Enter the number of terms: ");
                scanf("%d", &n);
                fibonacci(n);
                break;
            case 3:
                printf("Enter a number: ");
                scanf("%d", &n);
                printf("Sum of digits of %d is %d\n", n, sumOfDigits(n));
                break;
            case 4:
                printf("Enter two numbers: ");
                scanf("%d %d", &a, &b);
                printf("GCD of %d and %d is %d\n", a, b, gcd(a, b));
                break;
            case 5:
                printf("Enter the number of disks: ");
                scanf("%d", &n);
                towerOfHanoi(n, 'A', 'C', 'B');
                break;
            case 6:
                printf("Enter the number of elements: ");
                scanf("%d", &n);
                printf("Enter %d sorted elements: ", n);
                for (i = 0; i < n; i++)
                    scanf("%d", &arr[i]);
                printf("Enter the element to search: ");
                scanf("%d", &x);
                int result = binarySearch(arr, 0, n - 1, x);
                (result != -1) ? printf("Element is present at index %d\n", result) : printf("Element is not present in array\n");
                break;
            case 7:
                printf("Enter the number of elements: ");
                scanf("%d", &n);
                printf("Enter %d elements: ", n);
                for (i = 0; i < n; i++)
                    scanf("%d", &arr[i]);
                quickSort(arr, 0, n - 1);
                printf("Sorted array: ");
                for (i = 0; i < n; i++)
                    printf("%d ", arr[i]);
                printf("\n");
                break;
            case 8:
                printf("Enter the number of elements: ");
                scanf("%d", &n);
                printf("Enter %d elements: ", n);
                for (i = 0; i < n; i++)
                    scanf("%d", &arr[i]);
                mergeSort(arr, 0, n - 1);
                printf("Sorted array: ");
                for (i = 0; i < n; i++)
                    printf("%d ", arr[i]);
                printf("\n");
                break;
            case 9:
                printf("Enter the number of elements: ");
                scanf("%d", &n);
                printf("Enter %d elements: ", n);
                for (i = 0; i < n; i++)
                    scanf("%d", &arr[i]);
                printf("Enter the element to search: ");
                scanf("%d", &x);
                int index = searchElement(arr, n, x);
                if (index != -1)
                    printf("Element found at index %d\n", index);
                else
                    printf("Element not found\n");
                break;
            case 10:
                // Example Tree:       10
                //                    /  \
                //                   8    12
                //                  / \
                //                 7   9
                root = createNode(10);
                root->left = createNode(8);
                root->right = createNode(12);
                root->left->left = createNode(7);
                root->left->right = createNode(9);
                printf("Deepest node value: %d\n", findDeepestNode(root));
                break;
            case 11:
                printf("Enter the number of elements: ");
                scanf("%d", &n);
                printf("Enter %d elements: ", n);
                for (i = 0; i < n; i++)
                    scanf("%d", &arr[i]);
                if (isSorted(arr, n))
                    printf("The array is sorted.\n");
                else
                    printf("The array is not sorted.\n");
                break;
            case 12:
                top = -1; // Reset stack
                printf("Enter the number of elements to push onto the stack: ");
                scanf("%d", &n);
                for(i=0; i<n; i++) {
                    printf("Enter element %d: ", i+1);
                    scanf("%d", &x);
                    stack[++top] = x;
                }
                printf("Original Stack: ");
                for(i=0; i<=top; i++) printf("%d ", stack[i]);
                printf("\n");
                reverseStack(stack, &top);
                printf("Reversed Stack: ");
                for(i=0; i<=top; i++) printf("%d ", stack[i]);
                printf("\n");
                break;
            case 13:
                printf("Enter a number: ");
                scanf("%d", &n);
                printf("Sum of first %d numbers is: %d\n", n, sumOfN(n));
                break;
            case 14:
                printf("Enter the number of elements: ");
                scanf("%d", &n);
                printf("Enter %d elements: ", n);
                for (i = 0; i < n; i++)
                    scanf("%d", &arr[i]);
                int peak = findPeak(arr, 0, n - 1, n);
                printf("Peak element is %d\n", arr[peak]);
                break;
            case 15:
                printf("Enter a string: ");
                scanf("%s", str);
                printf("Permutations of the string are:\n");
                permute(str, 0, strlen(str) - 1);
                break;
            case 16:
                printf("Enter the number of elements in the set: ");
                scanf("%d", &n);
                printf("Enter %d elements: ", n);
                for (i = 0; i < n; i++)
                    scanf("%d", &arr[i]);
                printf("Subsets of the set are:\n");
                findSubsets(arr, n);
                break;
            case 17:
                printf("Enter a string: ");
                scanf("%s", str);
                if (isPalindrome(str, 0, strlen(str) - 1))
                    printf("%s is a palindrome.\n", str);
                else
                    printf("%s is not a palindrome.\n", str);
                break;
            case 18:
                // Example Graph: 0--1, 0--2, 1--2, 2--3
                adj[0][1] = adj[1][0] = 1;
                adj[0][2] = adj[2][0] = 1;
                adj[1][2] = adj[2][1] = 1;
                adj[2][3] = adj[3][2] = 1;
                int visited[10] = {0};
                printf("DFS traversal starting from vertex 0:\n");
                dfs(adj, V, 0, visited);
                printf("\n");
                break;
            case 19:
                // Same tree as case 10
                root = createNode(10);
                root->left = createNode(8);
                root->right = createNode(12);
                root->left->left = createNode(7);
                root->left->right = createNode(9);
                printf("Number of nodes in the tree: %d\n", countNodes(root));
                break;
            case 20:
                // Same tree as case 10
                root = createNode(10);
                root->left = createNode(8);
                root->right = createNode(12);
                root->left->left = createNode(7);
                root->left->right = createNode(9);
                printf("Height of the tree: %d\n", treeHeight(root));
                break;
            case 21:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 21);

    return 0;
}

// Function implementations
int factorial(int n) {
    if (n == 0)
        return 1;
    return n * factorial(n - 1);
}

void fibonacci(int n) {
    static int a = 0, b = 1, next;
    if (n > 0) {
        next = a + b;
        a = b;
        b = next;
        printf("%d ", a);
        fibonacci(n - 1);
    }
}

int sumOfDigits(int n) {
    if (n == 0)
        return 0;
    return (n % 10) + sumOfDigits(n / 10);
}

int gcd(int a, int b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

void towerOfHanoi(int n, char from_rod, char to_rod, char aux_rod) {
    if (n == 1) {
        printf("Move disk 1 from rod %c to rod %c\n", from_rod, to_rod);
        return;
    }
    towerOfHanoi(n - 1, from_rod, aux_rod, to_rod);
    printf("Move disk %d from rod %c to rod %c\n", n, from_rod, to_rod);
    towerOfHanoi(n - 1, aux_rod, to_rod, from_rod);
}

int binarySearch(int arr[], int l, int r, int x) {
    if (r >= l) {
        int mid = l + (r - l) / 2;
        if (arr[mid] == x)
            return mid;
        if (arr[mid] > x)
            return binarySearch(arr, l, mid - 1, x);
        return binarySearch(arr, mid + 1, r, x);
    }
    return -1;
}

void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    int L[n1], R[n2];
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

int searchElement(int arr[], int n, int x) {
    static int i = 0;
    if (i == n)
        return -1;
    if (arr[i] == x)
        return i;
    i++;
    return searchElement(arr, n, x);
}

struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

int findDeepestNodeUtil(struct Node* root, int level, int* maxLevel, int* res) {
    if (root == NULL)
        return 0;
    if (level > *maxLevel) {
        *res = root->data;
        *maxLevel = level;
    }
    findDeepestNodeUtil(root->left, level + 1, maxLevel, res);
    findDeepestNodeUtil(root->right, level + 1, maxLevel, res);
    return *res;
}

int findDeepestNode(struct Node* root) {
    int res = -1;
    int maxLevel = -1;
    findDeepestNodeUtil(root, 0, &maxLevel, &res);
    return res;
}

int isSorted(int arr[], int n) {
    if (n == 0 || n == 1)
        return 1;
    if (arr[0] > arr[1])
        return 0;
    return isSorted(arr + 1, n - 1);
}

void insertAtBottom(int stack[], int* top, int item) {
    if (*top == -1) {
        stack[++(*top)] = item;
        return;
    }
    int temp = stack[(*top)--];
    insertAtBottom(stack, top, item);
    stack[++(*top)] = temp;
}

void reverseStackUtil(int *stack, int *top) {
    if (*top != -1) {
        int item = stack[(*top)--];
        reverseStackUtil(stack, top);
        insertAtBottom(stack, top, item);
    }
    return;
}

void reverseStack(int stack[], int* top) {
    reverseStackUtil(stack, top);
}

int sumOfN(int n) {
    if (n == 0)
        return 0;
    return n + sumOfN(n - 1);
}

int findPeak(int arr[], int low, int high, int n) {
    int mid = low + (high - low) / 2;
    if ((mid == 0 || arr[mid - 1] <= arr[mid]) && (mid == n - 1 || arr[mid + 1] <= arr[mid]))
        return mid;
    else if (mid > 0 && arr[mid - 1] > arr[mid])
        return findPeak(arr, low, mid - 1, n);
    else
        return findPeak(arr, mid + 1, high, n);
}

void permute(char* a, int l, int r) {
    if (l == r)
        printf("%s\n", a);
    else {
        for (int i = l; i <= r; i++) {
            swap((int*)(a + l), (int*)(a + i));
            permute(a, l + 1, r);
            swap((int*)(a + l), (int*)(a + i));
        }
    }
}

void findSubsetsUtil(int arr[], int subset[], int n, int index, int sub_index) {
    if (index == n) {
        printf("{ ");
        for (int i = 0; i < sub_index; i++) {
            printf("%d ", subset[i]);
        }
        printf("}\n");
        return;
    }
    subset[sub_index] = arr[index];
    findSubsetsUtil(arr, subset, n, index + 1, sub_index + 1);
    findSubsetsUtil(arr, subset, n, index + 1, sub_index);
}

void findSubsets(int arr[], int n) {
    int subset[n];
    findSubsetsUtil(arr, subset, n, 0, 0);
}

int isPalindrome(char str[], int l, int r) {
    if (l >= r)
        return 1;
    if (str[l] != str[r])
        return 0;
    return isPalindrome(str, l + 1, r - 1);
}

void dfs(int adj[][10], int V, int start, int visited[]) {
    printf("%d ", start);
    visited[start] = 1;
    for (int i = 0; i < V; i++) {
        if (adj[start][i] == 1 && visited[i] == 0) {
            dfs(adj, V, i, visited);
        }
    }
}

int countNodes(struct Node* root) {
    if (root == NULL)
        return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}

int treeHeight(struct Node* root) {
    if (root == NULL)
        return 0;
    int leftHeight = treeHeight(root->left);
    int rightHeight = treeHeight(root->right);
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}
