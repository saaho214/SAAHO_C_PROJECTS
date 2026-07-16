#include <stdio.h>
#include <stdlib.h> // For dynamic memory allocation

// Function prototypes
void displayMatrix(int rows, int cols, int **matrix);
void inputMatrix(int rows, int cols, int **matrix);
int** createMatrix(int rows, int cols);
void freeMatrix(int rows, int **matrix);
void addMatrices();
void subtractMatrices();
void multiplyMatrices();
void transposeMatrix();
void scalarMultiplication();
int determinant(int **matrix, int n);
void getCofactor(int **matrix, int **temp, int p, int q, int n);
void adjoint(int **A, int **adj, int n);
void inverseMatrix();

int main() {
    int choice;

    do {
        printf("\n--- Matrix Operations Menu ---\n");
        printf("1. Addition of Matrices\n");
        printf("2. Subtraction of Matrices\n");
        printf("3. Multiplication of Matrices\n");
        printf("4. Transpose of a Matrix\n");
        printf("5. Scalar Multiplication\n");
        printf("6. Finding the Determinant\n");
        printf("7. Inverse of a Matrix\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addMatrices();
                break;
            case 2:
                subtractMatrices();
                break;
            case 3:
                multiplyMatrices();
                break;
            case 4:
                transposeMatrix();
                break;
            case 5:
                scalarMultiplication();
                break;
            case 6: {
                int n;
                printf("Enter the size of the square matrix: ");
                scanf("%d", &n);
                int **matrix = createMatrix(n, n);
                inputMatrix(n, n, matrix);
                printf("\n--- Original Matrix ---\n");
                displayMatrix(n, n, matrix);
                printf("\nDeterminant of the matrix is: %d\n", determinant(matrix, n));
                freeMatrix(n, matrix);
                break;
            }
            case 7:
                inverseMatrix();
                break;
            case 8:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice! Please enter a number between 1 and 8.\n");
        }
    } while (choice != 8);

    return 0;
}

// Dynamically allocate memory for a matrix
int** createMatrix(int rows, int cols) {
    int** matrix = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int*)malloc(cols * sizeof(int));
    }
    return matrix;
}

// Free dynamically allocated matrix
void freeMatrix(int rows, int **matrix) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Function to display a matrix
void displayMatrix(int rows, int cols, int **matrix) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
}

// Function to input matrix elements
void inputMatrix(int rows, int cols, int **matrix) {
    printf("Enter elements of the matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter element at [%d][%d]: ", i, j);
            scanf("%d", &matrix[i][j]);
        }
    }
}

// Function for matrix addition
void addMatrices() {
    int rows1, cols1, rows2, cols2;

    printf("Enter rows and columns for first matrix: ");
    scanf("%d %d", &rows1, &cols1);

    printf("Enter rows and columns for second matrix: ");
    scanf("%d %d", &rows2, &cols2);

    if (rows1 != rows2 || cols1 != cols2) {
        printf("Matrices must have the same dimensions for addition.\n");
        return;
    }

    int **matrix1 = createMatrix(rows1, cols1);
    int **matrix2 = createMatrix(rows2, cols2);
    int **result = createMatrix(rows1, cols1);

    printf("\n--- First Matrix ---\n");
    inputMatrix(rows1, cols1, matrix1);

    printf("\n--- Second Matrix ---\n");
    inputMatrix(rows2, cols2, matrix2);

    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            result[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }

    printf("\n--- Resultant Matrix (Addition) ---\n");
    displayMatrix(rows1, cols1, result);

    freeMatrix(rows1, matrix1);
    freeMatrix(rows2, matrix2);
    freeMatrix(rows1, result);
}

// Function for matrix subtraction
void subtractMatrices() {
    int rows1, cols1, rows2, cols2;

    printf("Enter rows and columns for first matrix: ");
    scanf("%d %d", &rows1, &cols1);

    printf("Enter rows and columns for second matrix: ");
    scanf("%d %d", &rows2, &cols2);

    if (rows1 != rows2 || cols1 != cols2) {
        printf("Matrices must have the same dimensions for subtraction.\n");
        return;
    }

    int **matrix1 = createMatrix(rows1, cols1);
    int **matrix2 = createMatrix(rows2, cols2);
    int **result = createMatrix(rows1, cols1);

    printf("\n--- First Matrix ---\n");
    inputMatrix(rows1, cols1, matrix1);

    printf("\n--- Second Matrix ---\n");
    inputMatrix(rows2, cols2, matrix2);

    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            result[i][j] = matrix1[i][j] - matrix2[i][j];
        }
    }

    printf("\n--- Resultant Matrix (Subtraction) ---\n");
    displayMatrix(rows1, cols1, result);

    freeMatrix(rows1, matrix1);
    freeMatrix(rows2, matrix2);
    freeMatrix(rows1, result);
}

// Function for matrix multiplication
void multiplyMatrices() {
    int rows1, cols1, rows2, cols2;

    printf("Enter rows and columns for first matrix: ");
    scanf("%d %d", &rows1, &cols1);

    printf("Enter rows and columns for second matrix: ");
    scanf("%d %d", &rows2, &cols2);

    if (cols1 != rows2) {
        printf("The number of columns in the first matrix must be equal to the number of rows in the second matrix for multiplication.\n");
        return;
    }

    int **matrix1 = createMatrix(rows1, cols1);
    int **matrix2 = createMatrix(rows2, cols2);
    int **result = createMatrix(rows1, cols2);

    printf("\n--- First Matrix ---\n");
    inputMatrix(rows1, cols1, matrix1);

    printf("\n--- Second Matrix ---\n");
    inputMatrix(rows2, cols2, matrix2);

    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols1; k++) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    printf("\n--- Resultant Matrix (Multiplication) ---\n");
    displayMatrix(rows1, cols2, result);

    freeMatrix(rows1, matrix1);
    freeMatrix(rows2, matrix2);
    freeMatrix(rows1, result);
}

// Function for matrix transpose
void transposeMatrix() {
    int rows, cols;

    printf("Enter rows and columns for the matrix: ");
    scanf("%d %d", &rows, &cols);

    int **matrix = createMatrix(rows, cols);
    int **transpose = createMatrix(cols, rows);

    inputMatrix(rows, cols, matrix);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            transpose[j][i] = matrix[i][j];
        }
    }

    printf("\n--- Original Matrix ---\n");
    displayMatrix(rows, cols, matrix);

    printf("\n--- Transpose Matrix ---\n");
    displayMatrix(cols, rows, transpose);

    freeMatrix(rows, matrix);
    freeMatrix(cols, transpose);
}

// Function for scalar multiplication
void scalarMultiplication() {
    int rows, cols, scalar;

    printf("Enter rows and columns for the matrix: ");
    scanf("%d %d", &rows, &cols);

    int **matrix = createMatrix(rows, cols);
    int **result = createMatrix(rows, cols);

    inputMatrix(rows, cols, matrix);

    printf("Enter the scalar value: ");
    scanf("%d", &scalar);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = scalar * matrix[i][j];
        }
    }

    printf("\n--- Original Matrix ---\n");
    displayMatrix(rows, cols, matrix);

    printf("\n--- Resultant Matrix (Scalar Multiplication) ---\n");
    displayMatrix(rows, cols, result);

    freeMatrix(rows, matrix);
    freeMatrix(rows, result);
}

// Function to get a cofactor of a matrix
void getCofactor(int **matrix, int **temp, int p, int q, int n) {
    int i = 0, j = 0;
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            if (row != p && col != q) {
                temp[i][j++] = matrix[row][col];
                if (j == n - 1) {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

// Function to find the determinant of a matrix
int determinant(int **matrix, int n) {
    int D = 0;
    if (n == 1) {
        return matrix[0][0];
    }
    int **temp = createMatrix(n, n);
    int sign = 1;
    for (int f = 0; f < n; f++) {
        getCofactor(matrix, temp, 0, f, n);
        D += sign * matrix[0][f] * determinant(temp, n - 1);
        sign = -sign;
    }
    freeMatrix(n, temp);
    return D;
}

// Function to find the adjoint of a matrix
void adjoint(int **A, int **adj, int n) {
    if (n == 1) {
        adj[0][0] = 1;
        return;
    }
    int **temp = createMatrix(n, n);
    int sign = 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            getCofactor(A, temp, i, j, n);
            sign = ((i + j) % 2 == 0) ? 1 : -1;
            adj[j][i] = sign * determinant(temp, n - 1);
        }
    }
    freeMatrix(n, temp);
}

// Function for inverse of a matrix
void inverseMatrix() {
    int n;
    printf("Enter the size of the square matrix: ");
    scanf("%d", &n);
    int **matrix = createMatrix(n, n);
    inputMatrix(n, n, matrix);
    int det = determinant(matrix, n);
    if (det == 0) {
        printf("\nInverse does not exist for this matrix (Determinant is 0).\n");
        freeMatrix(n, matrix);
        return;
    }
    int **adj = createMatrix(n, n);
    adjoint(matrix, adj, n);
    printf("\n--- Original Matrix ---\n");
    displayMatrix(n, n, matrix);
    printf("\n--- Inverse Matrix ---\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d/%d\t", adj[i][j], det);
        }
        printf("\n");
    }
    freeMatrix(n, matrix);
    freeMatrix(n, adj);
}