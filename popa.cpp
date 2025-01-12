#include <raylib.h>
#include <iostream>

using namespace std;

Color Retro1 = Color{173, 204, 96, 255};
Color Retro2 = Color{43, 51, 24, 255};

int cellSize = 30;
int cellCount = 25;

class Food {
public:
    Vector2 position = {5, 6};  // Змінена позиція
    Texture2D texture;
    
    Food() {
        Image image = LoadImage("food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
    
    }

    ~Food() {
        UnloadTexture(texture);
    }

    void Draw() {
        DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE);
    }
};

int main() {
    InitWindow(cellCount * cellSize, cellCount * cellSize, "Tetris");
    SetTargetFPS(60);

    Food food = Food();  // Об'єкт створюється після ініціалізації вікна

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(Retro1);

        food.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
