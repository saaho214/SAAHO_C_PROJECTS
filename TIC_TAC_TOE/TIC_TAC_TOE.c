#include <stdio.h>
#include <stdlib.h> 

// Global variables
char board[10] = {'o', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
char player1_name[20], player2_name[20];
int winner_flag = 0; // 0 = draw/no winner, 1 = Player 1 wins, 2 = Player 2 wins

// Function prototypes
void displayBoard();
int checkWin();
void displayResult();
void clearScreen();

int main() {
    int player, choice;
    char mark;

    printf("Enter Player 1's name (X): ");
    scanf("%s", player1_name);
    printf("Enter Player 2's name (O): ");
    scanf("%s", player2_name);

    clearScreen();

    // Main game loop
    for (int i = 1; i <= 9; i++) {
        displayBoard();
        
        // This is the corrected logic to switch players
        player = (i % 2 != 0) ? 1 : 2; 

        if (player == 1) {
            printf("%s's turn (X), enter a number: ", player1_name);
            mark = 'X';
        } else {
            printf("%s's turn (O), enter a number: ", player2_name);
            mark = 'O';
        }

        scanf("%d", &choice);

        // Input validation and board update
        if (choice >= 1 && choice <= 9 && board[choice] == (char)(choice + '0')) {
            board[choice] = mark;
        } else {
            printf("Invalid move. Try again.\n");
            i--; // Decrement loop counter to allow the same player another turn
            clearScreen();
            continue; // Skip the rest of the loop and start the next iteration
        }

        clearScreen();
        int game_status = checkWin();

        if (game_status == 1) { // A player has won
            winner_flag = player; // Set the winner_flag to the current player's number (1 or 2)
            break;
        } else if (game_status == 0) { // A draw
            winner_flag = 0; // Set winner_flag to 0 for a draw
            break;
        }
    }

    displayBoard();
    displayResult();
    
    // --- The Fix for the Window Closing ---
    // This code pauses the program and waits for the user to press a key.
    printf("\n\nPress any key to exit...");
    while ((getchar()) != '\n'); // Clear the input buffer from the last scanf
    getchar(); // Wait for a new key press from the user

    return 0;
}

// Function to display the game board in a neat tabular format
void displayBoard() {
    printf("\n\n\tTic-Tac-Toe\n\n");
    printf("+---+---+---+\n");
    printf("| %c | %c | %c |\n", board[1], board[2], board[3]);
    printf("+---+---+---+\n");
    printf("| %c | %c | %c |\n", board[4], board[5], board[6]);
    printf("+---+---+---+\n");
    printf("| %c | %c | %c |\n", board[7], board[8], board[9]);
    printf("+---+---+---+\n");
}

// Function to check for a win or a draw
int checkWin() {
    // Check horizontal wins
    if (board[1] == board[2] && board[2] == board[3]) return 1;
    if (board[4] == board[5] && board[5] == board[6]) return 1;
    if (board[7] == board[8] && board[8] == board[9]) return 1;

    // Check vertical wins
    if (board[1] == board[4] && board[4] == board[7]) return 1;
    if (board[2] == board[5] && board[5] == board[8]) return 1;
    if (board[3] == board[6] && board[6] == board[9]) return 1;

    // Check diagonal wins
    if (board[1] == board[5] && board[5] == board[9]) return 1;
    if (board[3] == board[5] && board[5] == board[7]) return 1;

    // Check for a draw (all squares filled)
    for(int i = 1; i <= 9; i++) {
        if (board[i] == (char)(i + '0')) {
            return -1; // Game is still in progress
        }
    }

    return 0; // It's a draw
}

// Function to display the final result in a receipt-like table
void displayResult() {
    printf("\n\n+---------------------------------+\n");
    printf("|          Game Result            |\n");
    printf("+---------------------------------+\n");

    if (winner_flag == 1) {
        printf("| Winner:    %-20s|\n", player1_name);
    } else if (winner_flag == 2) {
        printf("| Winner:    %-20s|\n", player2_name);
    } else {
        printf("| Result:    %-20s|\n", "It's a Draw!");
    }
    printf("+---------------------------------+\n");
}

// Function to clear the console screen
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}