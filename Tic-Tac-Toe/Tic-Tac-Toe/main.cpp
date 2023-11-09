#include <raylib.h>
#include <ctime>
#include <cstdlib>

const int boardSize = 3;
const int cellSize = 100;
const int offsetMessage = 100;

char board[boardSize][boardSize] = {
    {' ', ' ', ' '},
    {' ', ' ', ' '},
    {' ', ' ', ' '}
};

int currentPlayer = 1;  // Player 1 starts
bool gameWon = false;
bool vsAI = false;  // Play against another player by default
bool inMenu = true;
Rectangle playAIButton = { 50, GetScreenHeight() + 200  , 200, 50 };
Rectangle playPlayerButton = { 50, GetScreenHeight() + 100, 200, 50 };

void DrawMainMenu() {
    ClearBackground(RAYWHITE);

    DrawText("Tic Tac Toe", 50, 50, 30, BLACK);

    DrawRectangleRec(playAIButton, vsAI ? DARKGRAY : GRAY);
    DrawRectangleLines(playAIButton.x, playAIButton.y, playAIButton.width, playAIButton.height, BLACK);
    DrawText("Play vs AI", playAIButton.x + 25, playAIButton.y + 15, 20, BLACK);

    DrawRectangleRec(playPlayerButton, !vsAI ? DARKGRAY : GRAY);
    DrawRectangleLines(playPlayerButton.x, playPlayerButton.y, playPlayerButton.width, playPlayerButton.height, BLACK);
    DrawText("Play vs Player", playPlayerButton.x + 25, playPlayerButton.y + 15, 20, BLACK);
}

bool CheckWinner(char mark) {
    // Check rows, columns, and diagonals for a win
    for (int i = 0; i < boardSize; i++) {
        if ((board[i][0] == mark && board[i][1] == mark && board[i][2] == mark) ||  // Rows
            (board[0][i] == mark && board[1][i] == mark && board[2][i] == mark))    // Columns
            return true;
    }

    // Diagonals
    if ((board[0][0] == mark && board[1][1] == mark && board[2][2] == mark) ||     // Diagonal 1
        (board[0][2] == mark && board[1][1] == mark && board[2][0] == mark))        // Diagonal 2
        return true;

    return false;
}


bool IsMoveValid(int cellX, int cellY) {
    return cellX >= 0 && cellX < boardSize && cellY >= 0 && cellY < boardSize && board[cellY][cellX] == ' ';
}

void AIMove() {
    for (int cellX = 0; cellX < boardSize; cellX++) {
        for (int cellY = 0; cellY < boardSize; cellY++) {
            if (board[cellY][cellX] == ' ') {
                board[cellY][cellX] = 'O';
                if (CheckWinner('O')) {
                    return;
                }
                board[cellY][cellX] = ' ';  // Reset the test move
            }
        }
    }

    for (int cellX = 0; cellX < boardSize; cellX++) {
        for (int cellY = 0; cellY < boardSize; cellY++) {
            if (board[cellY][cellX] == ' ') {
                board[cellY][cellX] = 'X';
                if (CheckWinner('X')) {
                    board[cellY][cellX] = 'O';  // Block the player's winning move
                    return;
                }
                board[cellY][cellX] = ' ';  // Reset the test move
            }
        }
    }

    // Random move if no winning move for AI or player to block
    do {
        int cellX = rand() % boardSize;
        int cellY = rand() % boardSize;
        if (board[cellY][cellX] == ' ') {
            board[cellY][cellX] = 'O';
            return;
        }
    } while (true);
}

void PlayerMove(int mouseX, int mouseY) {
    int cellX = mouseX / cellSize;
    int cellY = mouseY / cellSize;

    if (IsMoveValid(cellX, cellY)) {
        if (currentPlayer == 1) {
            board[cellY][cellX] = 'X';
            currentPlayer = 2;
        } else {
            board[cellY][cellX] = 'O';
            currentPlayer = 1;
        }
    }
}

void DrawBoard() {
    ClearBackground(RAYWHITE);

    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            DrawRectangle(j * cellSize, i * cellSize, cellSize, cellSize, LIGHTGRAY);
            DrawRectangleLines(j * cellSize, i * cellSize, cellSize, cellSize, GRAY);

            if (board[i][j] == 'X') {
                DrawText("X", j * cellSize + cellSize / 3 -10, i * cellSize + cellSize / 3 -20, cellSize, BLACK);
            } else if (board[i][j] == 'O') {
                DrawText("O", j * cellSize + cellSize / 3 -10, i * cellSize + cellSize / 3 -20, cellSize, BLACK);
            }
        }
    }
}

int main() {
    InitWindow(boardSize * cellSize, boardSize * cellSize + offsetMessage, "Tic Tac Toe");

    SetTargetFPS(60);
    srand(time(0));

    while (!WindowShouldClose()) {

        int emptyCells = 0;
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                if (board[i][j] == ' ')
                    emptyCells++;
            }
        }    

        BeginDrawing();
        ClearBackground(RAYWHITE);
        if (inMenu) {
            DrawMainMenu();
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(GetMousePosition(), playAIButton)) {
                    vsAI = true;
                    inMenu = false;
                } else if (CheckCollisionPointRec(GetMousePosition(), playPlayerButton)) {
                    vsAI = false;
                    inMenu = false;
                }
            }
        } else {
            if (!gameWon) {
                if (!emptyCells == 0 && !CheckWinner('X') && !CheckWinner('O')) {
                    if (vsAI && currentPlayer == 2) {
                        AIMove();  // AI's move
                        currentPlayer = 1;
                    } else if (!vsAI && currentPlayer == 1) {
                        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                            int mouseX = GetMouseX();
                            int mouseY = GetMouseY();
                            PlayerMove(mouseX, mouseY);
                        }
                    }
                    else if (!vsAI && currentPlayer == 2) {
                        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                            int mouseX = GetMouseX();
                            int mouseY = GetMouseY();
                            PlayerMove(mouseX, mouseY);
                        }
                    }
                    else if (vsAI && currentPlayer == 1) {
                        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                            int mouseX = GetMouseX();
                            int mouseY = GetMouseY();
                            PlayerMove(mouseX, mouseY);
                        }
                    }
                } else if(emptyCells == 0) {
                    gameWon = true;
                } else {
                    gameWon = true;
                }
            }

            DrawBoard();
        
            if (gameWon) {
                if (CheckWinner('X')) {
                    DrawText("Player X wins!", 40, GetScreenHeight() -70, 30, RED);
                } else if (CheckWinner('O')) {
                    DrawText("Player O wins!", 40, GetScreenHeight() -70, 30, RED);
                } else{
                    DrawText("It's a Draw", 40, GetScreenHeight() -70, 30, RED);
                    vsAI =false;
                }
            }
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}