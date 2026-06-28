#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 100

// A global array and its current size
int *array = NULL;
int size = 0;
int max_capacity = 0;

// --- Utility Functions ---

// Function to display the array in a neat tabular format
void displayArray(const char *title) {
    printf("\n+---------------------------------------+\n");
    printf("| %-37s |\n", title);
    printf("+---------------------------------------+\n");
    if (size == 0) {
        printf("| %-37s |\n", "Array is empty.");
    } else {
        printf("| %-10s | %-24s |\n", "Index", "Value");
        printf("+---------------------------------------+\n");
        for (int i = 0; i < size; i++) {
            printf("| %-10d | %-24d |\n", i, array[i]);
        }
    }
    printf("+---------------------------------------+\n\n");
}

// Function to initialize or resize the array
void initializeArray() {
    int new_capacity;
    printf("Enter the initial capacity of the array: ");
    scanf("%d", &new_capacity);
    if (new_capacity <= 0) {
        printf("Invalid capacity. Using default capacity of 10.\n");
        new_capacity = 10;
    }
    array = (int *)malloc(new_capacity * sizeof(int));
    if (array == NULL) {
        printf("Memory allocation failed. Exiting.\n");
        exit(1);
    }
    max_capacity = new_capacity;
    size = 0;
    printf("Array initialized with capacity %d.\n", max_capacity);
}

// --- Array Operations ---

void createArray() {
    // If an array already exists, free its memory to prevent leaks
    if (array != NULL) {
        free(array);
        array = NULL;
    }

    printf("Enter the number of elements for the new array: ");
    scanf("%d", &size);

    if (size <= 0) {
        printf("Invalid size. Array creation failed.\n");
        size = 0;
        return;
    }

    // Allocate memory for the new array
    array = (int *)malloc(size * sizeof(int));
    if (array == NULL) {
        printf("Memory allocation failed. Exiting.\n");
        exit(1);
    }

    // Prompt the user for each element
    printf("Enter the initial values for the %d elements:\n", size);
    for (int i = 0; i < size; i++) {
        printf("Element at index %d: ", i);
        scanf("%d", &array[i]);
    }
    
    printf("Array created successfully with %d elements.\n", size);
    max_capacity = size; // Set max capacity to the initial size
}


// 1. Access an element by index
void accessElement() {
    int index;
    printf("Enter the index of the element to access: ");
    scanf("%d", &index);

    if (index >= 0 && index < size) {
        printf("Element at index %d is: %d\n", index, array[index]);
    } else {
        printf("Error: Index is out of bounds.\n");
    }
}

// 2. Search for an element by value
void searchElement() {
    int value;
    int found = 0;
    printf("Enter the value to search for: ");
    scanf("%d", &value);

    printf("Searching for value %d...\n", value);
    for (int i = 0; i < size; i++) {
        if (array[i] == value) {
            printf("Value %d found at index %d.\n", value, i);
            found = 1;
        }
    }
    if (!found) {
        printf("Value %d not found in the array.\n", value);
    }
}

// 3. Insertion at the end
void insertAtEnd() {
    if (size >= max_capacity) {
        printf("Array is full. Please resize first.\n");
        return;
    }
    int value;
    printf("Enter the value to insert at the end: ");
    scanf("%d", &value);
    array[size++] = value;
    printf("Element %d inserted at the end.\n", value);
}

// 4. Insertion at a specific index
void insertAtIndex() {
    if (size >= max_capacity) {
        printf("Array is full. Please resize first.\n");
        return;
    }
    int index, value;
    printf("Enter the index for insertion: ");
    scanf("%d", &index);
    if (index < 0 || index > size) {
        printf("Error: Invalid index for insertion.\n");
        return;
    }
    printf("Enter the value to insert: ");
    scanf("%d", &value);

    for (int i = size; i > index; i--) {
        array[i] = array[i - 1];
    }
    array[index] = value;
    size++;
    printf("Element %d inserted at index %d.\n", value, index);
}

// 5. Deletion from the end
void deleteFromEnd() {
    if (size == 0) {
        printf("Array is empty. Nothing to delete.\n");
        return;
    }
    printf("Element %d deleted from the end.\n", array[size - 1]);
    size--;
}

// 6. Deletion from a specific index
void deleteFromIndex() {
    if (size == 0) {
        printf("Array is empty. Nothing to delete.\n");
        return;
    }
    int index;
    printf("Enter the index of the element to delete: ");
    scanf("%d", &index);

    if (index < 0 || index >= size) {
        printf("Error: Index is out of bounds.\n");
        return;
    }
    printf("Element %d at index %d deleted.\n", array[index], index);
    for (int i = index; i < size - 1; i++) {
        array[i] = array[i + 1];
    }
    size--;
}

// 7. Update an element
void updateElement() {
    int index, newValue;
    printf("Enter the index of the element to update: ");
    scanf("%d", &index);
    if (index < 0 || index >= size) {
        printf("Error: Index is out of bounds.\n");
        return;
    }
    printf("Enter the new value: ");
    scanf("%d", &newValue);
    array[index] = newValue;
    printf("Element at index %d updated to %d.\n", index, newValue);
}

// 8. Resizing the array
void resizeArray() {
    int new_capacity;
    printf("Enter the new capacity of the array: ");
    scanf("%d", &new_capacity);

    if (new_capacity <= 0) {
        printf("Invalid capacity. Resizing cancelled.\n");
        return;
    }
    int *temp = realloc(array, new_capacity * sizeof(int));
    if (temp == NULL) {
        printf("Memory reallocation failed. Array remains unchanged.\n");
        return;
    }
    array = temp;
    max_capacity = new_capacity;
    if (size > max_capacity) {
        size = max_capacity;
    }
    printf("Array resized to a new capacity of %d.\n", max_capacity);
}

// 9. Merging two arrays
void mergeArrays() {
    int other_size;
    printf("Enter the number of elements for the second array: ");
    scanf("%d", &other_size);

    int *other_array = (int *)malloc(other_size * sizeof(int));
    if (other_array == NULL) {
        printf("Memory allocation failed for second array.\n");
        return;
    }

    printf("Enter %d elements for the second array:\n", other_size);
    for (int i = 0; i < other_size; i++) {
        scanf("%d", &other_array[i]);
    }

    int new_size = size + other_size;
    int *merged_array = (int *)malloc(new_size * sizeof(int));
    if (merged_array == NULL) {
        printf("Memory allocation failed for merged array.\n");
        free(other_array);
        return;
    }

    memcpy(merged_array, array, size * sizeof(int));
    memcpy(merged_array + size, other_array, other_size * sizeof(int));

    free(array);
    free(other_array);

    array = merged_array;
    size = new_size;
    max_capacity = new_size;

    printf("Arrays merged successfully.\n");
    displayArray("Merged Array");
}

// --- Sorting Algorithms ---

// Helper function for swapping two elements
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Insertion Sort
void insertionSort() {
    int i, key, j;
    for (i = 1; i < size; i++) {
        key = array[i];
        j = i - 1;
        while (j >= 0 && array[j] > key) {
            array[j + 1] = array[j];
            j = j - 1;
        }
        array[j + 1] = key;
    }
    printf("Array sorted using Insertion Sort.\n");
}

// Selection Sort
void selectionSort() {
    int i, j, min_idx;
    for (i = 0; i < size - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < size; j++) {
            if (array[j] < array[min_idx]) {
                min_idx = j;
            }
        }
        swap(&array[min_idx], &array[i]);
    }
    printf("Array sorted using Selection Sort.\n");
}

// Bubble Sort
void bubbleSort() {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                swap(&array[j], &array[j + 1]);
            }
        }
    }
    printf("Array sorted using Bubble Sort.\n");
}

// Quick Sort
int partition(int low, int high) {
    int pivot = array[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (array[j] < pivot) {
            i++;
            swap(&array[i], &array[j]);
        }
    }
    swap(&array[i + 1], &array[high]);
    return (i + 1);
}

void quickSort(int low, int high) {
    if (low < high) {
        int pi = partition(low, high);
        quickSort(low, pi - 1);
        quickSort(pi + 1, high);
    }
}

// Merge Sort
void merge(int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    int L[n1], R[n2];
    for (i = 0; i < n1; i++) L[i] = array[l + i];
    for (j = 0; j < n2; j++) R[j] = array[m + 1 + j];
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            array[k] = L[i];
            i++;
        } else {
            array[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        array[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        array[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(l, m);
        mergeSort(m + 1, r);
        merge(l, m, r);
    }
}


// --- Main Function and Menu ---

int main() {
    int choice;
    initializeArray();
    srand(time(NULL));

    do {
        printf("\n\n+---------------------------------------+\n");
        printf("|         Array Operations Menu         |\n");
        printf("+---------------------------------------+\n");
        printf("| 0. Create/Re-create Array               |\n"); 
        printf("| 1. Display Array                      |\n");
        printf("| 2. Access Element                     |\n");
        printf("| 3. Search Element                     |\n");
        printf("| 4. Insert Element                     |\n");
        printf("| 5. Delete Element                     |\n");
        printf("| 6. Update Element                     |\n");
        printf("| 7. Resizing Array                     |\n");
        printf("| 8. Sort Array                         |\n");
        printf("| 9. Merge Arrays                       |\n");
        printf("| 10. Exit                              |\n");
        printf("+---------------------------------------+\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 0:createArray();break;
            case 1: displayArray("Current Array Elements"); break;
            case 2: accessElement(); break;
            case 3: searchElement(); break;
            case 4: {
                int insert_choice;
                printf("  4.1 Insert at the End\n");
                printf("  4.2 Insert at specific Index\n");
                printf("  Enter choice: ");
                scanf("%d", &insert_choice);
                if (insert_choice == 1) insertAtEnd();
                else if (insert_choice == 2) insertAtIndex();
                else printf("Invalid sub-choice.\n");
                break;
            }
            case 5: {
                int delete_choice;
                printf("  5.1 Delete from the End\n");
                printf("  5.2 Delete from specific Index\n");
                printf("  Enter choice: ");
                scanf("%d", &delete_choice);
                if (delete_choice == 1) deleteFromEnd();
                else if (delete_choice == 2) deleteFromIndex();
                else printf("Invalid sub-choice.\n");
                break;
            }
            case 6: updateElement(); break;
            case 7: resizeArray(); break;
            case 8: {
                int sort_choice;
                printf("  8.1 Bubble Sort\n");
                printf("  8.2 Insertion Sort\n");
                printf("  8.3 Selection Sort\n");
                printf("  8.4 Quick Sort\n");
                printf("  8.5 Merge Sort\n");
                printf("  Enter choice: ");
                scanf("%d", &sort_choice);
                if (sort_choice == 1) bubbleSort();
                else if (sort_choice == 2) insertionSort();
                else if (sort_choice == 3) selectionSort();
                else if (sort_choice == 4) quickSort(0, size - 1);
                else if (sort_choice == 5) mergeSort(0, size - 1);
                else printf("Invalid sub-choice.\n");
                break;
            }
            case 9: mergeArrays(); break;
            case 10:
                if (array != NULL) free(array);
                printf("Exiting program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 10.\n");
        }
    } while (choice != 10);

    return 0;
}