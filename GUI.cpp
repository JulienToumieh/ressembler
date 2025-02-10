#include "raylib.h"
#include <iostream>
#include <string.h>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include "interpreter.cpp"

using namespace std;


typedef struct Button {
    Texture2D texture; 
    Vector2 position; 
    Rectangle bounds;  
} Button;

Button InitButton(const char* imagePath, Vector2 position) {
    Button btn;
    btn.texture = LoadTexture(imagePath);
    btn.position = position;
    btn.bounds = (Rectangle){position.x, position.y, btn.texture.width, btn.texture.height};
    return btn;
}

void DrawButton(Button btn) {
    DrawTexture(btn.texture, btn.position.x, btn.position.y, WHITE);
}

bool IsButtonPressed(Button btn) {
    return CheckCollisionPointRec(GetMousePosition(), btn.bounds) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
}

void drawRect(int x, int y, int w, int h, Color col) {
    float topLeftX = x - w / 2.0f;
    float topLeftY = y - h / 2.0f;

    DrawRectangleRounded((Rectangle){topLeftX, topLeftY, (float)w, (float)h}, 0.2f, 16, col);
}

void DrawCenteredText(const char* text, int x, int y, int fontSize, Color color) {
    int textWidth = MeasureText(text, fontSize);
    int centerX = x - textWidth / 2;
    // Draw the text at the calculated position
    DrawText(text, centerX, y, fontSize, color);
}

void findSpecificCommand(FILE* file, const char* targetSecondPart) {
    // Reopen the file in read mode to start from the beginning
    fseek(file, 0, SEEK_SET); 

    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = 0;

        char* firstPart = strtok(line, " ");
        char* secondPart = strtok(NULL, " ");
        
        if (firstPart == NULL || secondPart == NULL) {
            continue;
        }

        // Check if the first part matches the target
        if (!strcmp(firstPart, "LBL") && !strcmp(secondPart, targetSecondPart)) {
            break;
        }
    }
}

char* ReadFileContents(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Could not open file %s for reading.\n", filename);
        return NULL; // Return NULL if the file cannot be opened
    }

    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    fseek(file, 0, SEEK_SET); 

    char* buffer = (char*)malloc(filesize + 1);
    if (buffer == NULL) {
        printf("Memory allocation failed.\n");
        fclose(file);
        return NULL; 
    }

    // Read the contents of the file into the buffer
    fread(buffer, 1, filesize, file);
    buffer[filesize] = '\0'; 

    fclose(file); 
    return buffer; // Return the contents of the file
}

void appendChar(char text[], char c) {
    size_t len = strlen(text); 
    if (len < 20000) { 
        text[len] = c; 
        text[len + 1] = '\0'; 
    } else {
        printf("Cannot append character, array is full!\n");
    }
}

int main() {
    
    RessemblyInterpreter interpreter;
    
    // Initialize the window
    const int screenWidth = 1200;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Ressembly GUI");
    
    char currentCommand[64] = "";
    
    char binaryText[20000] = "";
    char consoleText[20000] = "";
    
    char outputText[20000] = "";
    char compiledText[20000] = "";
    
    bool outputView = true;
    bool knobSlide = false;
    Vector2 mousePos;
    
    FILE *file = fopen("compiled.bin", "r");
    
    float timer = 0.0f; // Timer to track elapsed time
    float interval = 0.5f; // Interval in seconds
    float initInterval = 0.5f; // Interval in seconds
    
    bool playing = false;
    
    Vector2 knobPosition = { 534, 180 + 90 * 6 }; // Knob position
    float knobRadius = 30; // Radius of the knob
    
    
    
    Rectangle inputField = { 344, 180, 50, 50 }; // x, y, width, height
    char inputChar = '\0'; // To store the input character
    bool isHovered = false;
    
    bool consoleHovered = false;
    int consoleTextHeight = 0;
    bool binaryHovered = false;
    int binaryTextHeight = 0;
    

    SetTargetFPS(60);
    
    
    Button nextStepButton = InitButton("images/next_step.png",     (Vector2){414, 188});
    
    Button compileCppButton = InitButton("images/compile_cpp.png", (Vector2){510, 140 + 90 * 1});
    Button compileBinButton = InitButton("images/compile_bin.png", (Vector2){510, 140 + 90 * 2});
    Button reloadButton = InitButton("images/reload.png",          (Vector2){510, 140 + 90 * 3});
    Button startButton = InitButton("images/start.png",            (Vector2){510, 140 + 90 * 4});
    Button pauseButton = InitButton("images/pause.png",            (Vector2){510, 140 + 90 * 5});

    Button switchConsoleButton = InitButton("images/switch_console.png", (Vector2){1076, 170});

    while (!WindowShouldClose()) {
        
        Vector2 mousePosition = GetMousePosition();


        if (playing){

        timer += GetFrameTime();
        
            if (timer >= interval) { 

                char line[64];
                fgets(line, sizeof(line), file);
                
                line[strcspn(line, "\n")] = 0;  // Remove the newline character
                
                strcpy(currentCommand, line);

                // Tokenize the line by space
                char *firstPart = strtok(line, " ");
                char *secondPart = strtok(NULL, " ");
                
                if (firstPart == NULL) {
                    firstPart = "";
                }
                
                if (!strcmp(firstPart, "OPR")){
                    interpreter.increment(atoi(secondPart));
                    
                } else if (!strcmp(firstPart, "MOV")){
                    interpreter.move(atoi(secondPart));
                    
                } else if (!strcmp(firstPart, "VIN")){
                    interpreter.input(inputChar);
                    
                } else if (!strcmp(firstPart, "VOUT")){
                    appendChar(outputText, static_cast<char>(interpreter.memCells[interpreter.currCell]));
                    
                } else if (!strcmp(firstPart, "JNZ")){
                    if (interpreter.memCells[interpreter.currCell] != 0)
                        findSpecificCommand(file, secondPart);
                    
                } else if (!strcmp(firstPart, "JMP")){
                    findSpecificCommand(file, secondPart);
                    
                } else if (!strcmp(firstPart, "END")){
                    printf("END");
                } 
                timer -= interval; 
            }
        }

        // Begin drawing
        BeginDrawing();
        ClearBackground((Color){20, 20, 20, 255});
        
       

        // Draw cells
        int memCellHeight = 70;
        int cellSpacing = 70;
        

        if (CheckCollisionPointRec(mousePosition, (Rectangle){50, 260, 420, 500})){
            binaryHovered = true;
        } else {
            binaryHovered = false;
        }
        if (CheckCollisionPointRec(mousePosition, (Rectangle){600, 180, 550, 585})){
            consoleHovered = true;
        } else {
            consoleHovered = false;
        }
        
        float wheelMove = GetMouseWheelMove();
        if (consoleHovered) {
            consoleTextHeight += wheelMove*40;
            if (consoleTextHeight > 0)
                consoleTextHeight = 0;
        }
        if (binaryHovered) {
            binaryTextHeight += wheelMove*40;
            if (binaryTextHeight > 0)
                binaryTextHeight = 0;
        }
        

        if (!isHovered){
        if (IsKeyPressed(KEY_A)) { // Move left
            interpreter.move(-1);
        }
        if (IsKeyPressed(KEY_D)) { // Move right
            interpreter.move(1);
        }
        if (IsKeyPressed(KEY_W)) { // Increment
            interpreter.increment(1);
        }
        if (IsKeyPressed(KEY_S)) { // Decrement
            interpreter.increment(-1);
        }
        }
        
        if (IsButtonPressed(compileCppButton)) {
            system("CPPConverter.exe");
            system("g++ compiled.cpp -o compiledCPP.exe");
            strcpy(compiledText, ReadFileContents("compiled.cpp"));
        }
        if (IsButtonPressed(compileBinButton)) {
            system("BinCompiler.exe");
            strcpy(binaryText, ReadFileContents("compiled.bin"));
        }
        if (IsButtonPressed(switchConsoleButton)) {
            outputView = !outputView;
        }
        if (IsButtonPressed(reloadButton)) {
            file = fopen("compiled.bin", "r");

            if (file == NULL) {
                printf("Could not open the file\n");
                return 1;
            }
            
            playing = false;
            interpreter.reset();
            outputText[0] = '\0';

        }
        if (IsButtonPressed(startButton)) {
            playing = true;
        }
        if (IsButtonPressed(pauseButton)) {
            playing = false;
        }
        if (IsButtonPressed(nextStepButton)) {
            char line[64];
            fgets(line, sizeof(line), file);
            
            line[strcspn(line, "\n")] = 0;  // Remove the newline character
            
            strcpy(currentCommand, line);

            // Separate by space
            char *firstPart = strtok(line, " ");
            char *secondPart = strtok(NULL, " ");
            
            if (firstPart == NULL) {
                firstPart = "";
            }
            
            if (!strcmp(firstPart, "OPR")){
                interpreter.increment(atoi(secondPart));
                
            } else if (!strcmp(firstPart, "MOV")){
                interpreter.move(atoi(secondPart));
                
            } else if (!strcmp(firstPart, "VIN")){
                interpreter.input(inputChar);
                
            } else if (!strcmp(firstPart, "VOUT")){
                appendChar(outputText, static_cast<char>(interpreter.memCells[interpreter.currCell]));
                
            } else if (!strcmp(firstPart, "JNZ")){
                if (interpreter.memCells[interpreter.currCell] != 0)
                    findSpecificCommand(file, secondPart);
                
            } else if (!strcmp(firstPart, "JMP")){
                findSpecificCommand(file, secondPart);
                
            } else if (!strcmp(firstPart, "END")){
                printf("END");
            } 
        }
        
        if (outputView) {
            strcpy(consoleText, outputText);
        } else {
            strcpy(consoleText, compiledText);
        }
        
        
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            if (!knobSlide){
                mousePos = GetMousePosition();
                initInterval = interval;
            }
            
            if (CheckCollisionCircles(mousePos, 5, knobPosition, knobRadius)) {
                knobSlide = true;
            }
        }
        
        if (knobSlide) {
            // Adjust interval based on mouse position (x-axis)
            Vector2 newMousePos = GetMousePosition();
            interval = initInterval + (newMousePos.x - mousePos.x) * 0.001; 
            if (interval < 0.01f) interval = 0.01f; // Minimum interval limit
            if (interval > 1.0f) interval = 1.0f; // Maximum interval limit
        }
        
        if (IsMouseButtonUp(MOUSE_LEFT_BUTTON)) {
            knobSlide = false;
        }
        
        DrawCircleV(knobPosition, knobRadius, DARKPURPLE);
        
        char text[20];
        snprintf(text, sizeof(text), "%.2f", interval); 
        Vector2 textSize = MeasureTextEx(GetFontDefault(), text, 20, 1); 
        
        DrawText(text, 
                  knobPosition.x - textSize.x / 2, 
                  knobPosition.y - textSize.y / 2, 
                  20, 
                  WHITE);
        
        DrawRectangleRounded((Rectangle){50, 260, 420, 500}, 0.05f, 8, (Color){11, 11, 11, 255});
        DrawRectangleRounded((Rectangle){600, 180, 550, 585}, 0.05f, 8, (Color){11, 11, 11, 255});
        
        
        DrawText(binaryText, 66, binaryTextHeight + 274, 22, LIGHTGRAY);
        DrawText(consoleText, 614, consoleTextHeight + 188, 20, LIGHTGRAY);
        
        DrawRectangle(0, 0, 1200, 180, (Color){20, 20, 20, 255});
        DrawRectangle(0, 0, 480, 260, (Color){20, 20, 20, 255});
        DrawRectangle(0, 760, 1200, 100, (Color){20, 20, 20, 255});
        
        DrawRectangleRounded((Rectangle){50, 180, 280, 50}, 0.4f, 8, (Color){11, 11, 11, 255});
        DrawText(currentCommand, 70, 196, 22, WHITE);
        
        drawRect(72 + interpreter.currCell * cellSpacing, memCellHeight + 12, 60, 85, (Color){82, 255, 139, 255});
        for (int i = 0; i < 16; i++) {
            drawRect(72 + i*cellSpacing, memCellHeight, 50, 50, (Color){47, 47, 47, 255});
            DrawCenteredText(TextFormat("%i", interpreter.memCells[i]), 71 + i * cellSpacing, memCellHeight - 10, 20, LIGHTGRAY);
            DrawCenteredText(TextFormat("%i", i), 71 + i * cellSpacing, memCellHeight + 30, 20, GRAY);
        }
        
        isHovered = CheckCollisionPointRec(mousePosition, inputField);

        // Input handling
        if (isHovered) {
            int keyPressed = GetKeyPressed();
            if (keyPressed >= 32 && keyPressed <= 126) { // ASCII printable characters
                inputChar = (char)keyPressed; // Store the character
            }
        }
        
        drawRect(inputField.x + 25, inputField.y + 25, 50, 50, isHovered ? DARKGRAY : (Color){11, 11, 11, 255});
        DrawCenteredText(TextFormat("%c", inputChar), inputField.x + 25, inputField.y + 16, 20, WHITE);

        
        
        DrawButton(compileCppButton);
        DrawButton(compileBinButton);
        DrawButton(nextStepButton);
        DrawButton(startButton);
        DrawButton(pauseButton);
        DrawButton(reloadButton);
        DrawButton(switchConsoleButton);
        
        // End drawing
        EndDrawing();
    }

    // De-Initialization
    CloseWindow(); 
    return 0;
}
