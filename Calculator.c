#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_CHARS 16

bool IsButtonClicked(Vector2 mousePos, Rectangle bounds) {
    return CheckCollisionPointRec(mousePos, bounds) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

int main(void) {
    // Initialization
    const int windowWidth = 320;
    const int windowHeight = 480;
    InitWindow(windowWidth, windowHeight, "Calculator");

    char displayBuffer[MAX_INPUT_CHARS + 1] = "0";
    double value1 = 0;
    char currentOp = 0;
    bool readyForNextNumber = false;

    // Layout 
    int padding = 10;
    int btnWidth = 65;
    int btnHeight = 65;
    int startY = 110;

    struct CalcButton {
        const char *text;
        int gridX;
        int gridY;
        Color baseColor;
    } buttons[] = {
        {"C", 0, 0, RED},    {"/", 3, 0, ORANGE},
        {"7", 0, 1, DARKGRAY},{"8", 1, 1, DARKGRAY},{"9", 2, 1, DARKGRAY},{"*", 3, 1, ORANGE},
        {"4", 0, 2, DARKGRAY},{"5", 1, 2, DARKGRAY},{"6", 2, 2, DARKGRAY},{"-", 3, 2, ORANGE},
        {"1", 0, 3, DARKGRAY},{"2", 1, 3, DARKGRAY},{"3", 2, 3, DARKGRAY},{"+", 3, 3, ORANGE},
        {"0", 0, 4, DARKGRAY},{".", 2, 4, DARKGRAY},{"=", 3, 4, GREEN}
    };
    int numButtons = sizeof(buttons) / sizeof(buttons[0]);

    SetTargetFPS(60);

    // Main Loop
    while (!WindowShouldClose()) {
        // Update
        Vector2 mousePos = GetMousePosition();

        for (int i = 0; i < numButtons; i++) {
            // Calculate button layout bounds
            Rectangle btnRec;
            if (strcmp(buttons[i].text, "0") == 0) {
                // Wide zero button spanning 2 columns
                btnRec = (Rectangle){ padding, startY + buttons[i].gridY * (btnHeight + padding), (btnWidth * 2) + padding, btnHeight };
            } else {
                btnRec = (Rectangle){ padding + buttons[i].gridX * (btnWidth + padding), startY + buttons[i].gridY * (btnHeight + padding), btnWidth, btnHeight };
            }

            if (IsButtonClicked(mousePos, btnRec)) {
                const char *val = buttons[i].text;

                if (val[0] >= '0' && val[0] <= '9' || val[0] == '.') {
                    if (readyForNextNumber || strcmp(displayBuffer, "0") == 0) {
                        strcpy(displayBuffer, val);
                        readyForNextNumber = false;
                    } else if (strlen(displayBuffer) < MAX_INPUT_CHARS) {
                        strcat(displayBuffer, val);
                    }
                } else if (strcmp(val, "C") == 0) {
                    strcpy(displayBuffer, "0");
                    value1 = 0;
                    currentOp = 0;
                    readyForNextNumber = false;
                } else if (strcmp(val, "=") == 0) {
                    if (currentOp != 0) {
                        double value2 = atof(displayBuffer);
                        double result = 0;
                        if (currentOp == '+') result = value1 + value2;
                        else if (currentOp == '-') result = value1 - value2;
                        else if (currentOp == '*') result = value1 * value2;
                        else if (currentOp == '/') {
                            if (value2 != 0) result = value1 / value2;
                            else result = 0; // Avoid crash
                        }
                        snprintf(displayBuffer, MAX_INPUT_CHARS, "%g", result);
                        currentOp = 0;
                        readyForNextNumber = true;
                    }
                } else { // Arithmetic operators
                    value1 = atof(displayBuffer);
                    currentOp = val[0];
                    readyForNextNumber = true;
                }
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw Display Box
        DrawRectangle(padding, padding, windowWidth - (padding * 2), 80, BLACK);
        DrawText(displayBuffer, windowWidth - padding - MeasureText(displayBuffer, 40) - 10, 30, 40, GREEN);

        // Draw Buttons
        for (int i = 0; i < numButtons; i++) {
            Rectangle btnRec;
            if (strcmp(buttons[i].text, "0") == 0) {
                btnRec = (Rectangle){ padding, startY + buttons[i].gridY * (btnHeight + padding), (btnWidth * 2) + padding, btnHeight };
            } else {
                btnRec = (Rectangle){ padding + buttons[i].gridX * (btnWidth + padding), startY + buttons[i].gridY * (btnHeight + padding), btnWidth, btnHeight };
            }

            Color renderColor = buttons[i].baseColor;
            if (CheckCollisionPointRec(mousePos, btnRec)) {
                renderColor = MAROON; // highlight color
            }

            DrawRectangleRec(btnRec, renderColor);
            
            int textWidth = MeasureText(buttons[i].text, 20);
            DrawText(buttons[i].text, btnRec.x + (btnRec.width / 2) - (textWidth / 2), btnRec.y + (btnRec.height / 2) - 10, 20, WHITE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}