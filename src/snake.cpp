#include <raylib.h>
#include <iostream>
#include <deque>
#include <raymath.h>
#include "menu.hpp"

using namespace std;

int Score = 0;

double lastUpdateTime = 0;

bool eventTriggered(double interval) {
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

bool ElementInDeque(Vector2 element, deque<Vector2> deque) {
    for (unsigned int i = 0; i < deque.size(); i++) {
        if (Vector2Equals(deque[i], element)) {
            return true;
        }
    }
    return false;
}

class Snake {
public:
    deque<Vector2> body = {Vector2{4, 9}, Vector2{5, 9}, Vector2{6, 9}};
    Vector2 direction = {1, 0};
    bool addSegment = false;

    void Draw() {
        for (unsigned int i = 0; i < body.size(); i++) {
            float x = body[i].x;
            float y = body[i].y;
            Rectangle segment = Rectangle{offset + x * cellSize, offset + y * cellSize, (float)cellSize, (float)cellSize};
            DrawRectangleRounded(segment, 0.5, 6, Retro2);
        }
    }

    void Update() {
        body.push_front(Vector2Add(body[0], direction));
        if (addSegment) {
            addSegment = false;
        } else {
            body.pop_back();
        }
    }

    void Reset() {
        body = {Vector2{4, 9}, Vector2{5, 9}, Vector2{6, 9}};
        direction = {1, 0};
    }
};

class Food {
public:
    Vector2 position;
    Texture2D texture;

    Food(deque<Vector2> snakeBody) {
        Image image = LoadImage("food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandompos(snakeBody);
    }

    ~Food() {
        UnloadTexture(texture);
    }

    void Draw() {
        DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, WHITE);
    }

    Vector2 GenerateRandomCell() {
        float x = GetRandomValue(0, cellCount - 1);
        float y = GetRandomValue(0, cellCount - 1);
        return Vector2{x, y};
    }

    Vector2 GenerateRandompos(deque<Vector2> snakeBody) {
        Vector2 position = GenerateRandomCell();
        while (ElementInDeque(position, snakeBody)) {
            position = GenerateRandomCell();
        }
        return position;
    }
};

class Game {
public:
    Snake snake = Snake();
    Food food = Food(snake.body);
    Sound eatSound;
    Sound wallSound;
    bool running = true;

    Game() {
        InitAudioDevice();
        eatSound = LoadSound("chipsi.mp3");
        wallSound = LoadSound("Game_over.mp3");
    }

    ~Game() {
        UnloadSound(eatSound);
        UnloadSound(wallSound);
        CloseAudioDevice();
    }

    void Update() {
        if (running) {
            snake.Update();
            CheckCollisions();
            CheckCollisionsEdges();
            CheckCollisionWithTail();
        }
    }

    void Draw() {
        if (running) {
            food.Draw();
            snake.Draw();
        } else {
            DrawText("Game Over", (cellCount * cellSize) / 2 - 70, (cellCount * cellSize) / 2, 64, Retro2);
        }
    }

    void CheckCollisions() {
        if (Vector2Equals(snake.body[0], food.position)) {
            food.position = food.GenerateRandompos(snake.body);
            snake.addSegment = true;
            Score++;
            PlaySound(eatSound);
        }
    }

    void CheckCollisionsEdges() {
        if (snake.body[0].x == cellCount || snake.body[0].x == -1 ||
            snake.body[0].y == cellCount || snake.body[0].y == -1) {
            GameOver();
        }
    }

    void GameOver() {
        snake.Reset();
        food.position = food.GenerateRandompos(snake.body);
        running = false;
        Score = 0;
        PlaySound(wallSound);
    }

    void CheckCollisionWithTail() {
        deque<Vector2> headlessBody = snake.body;
        headlessBody.pop_front();
        if (ElementInDeque(snake.body[0], headlessBody)) {
            GameOver();
        }
    }
};

int main() {
    InitWindow(2 * offset + cellCount * cellSize, 2 * offset + cellCount * cellSize, "Snake Game");
    SetTargetFPS(60);

    Game game = Game();
    Sprite name = Sprite("game_name.png");
    Sprite wall = Sprite("wall.png");
    Sprite button_start = Sprite("button_start.png");
    Sprite button_exit = Sprite("button_exit.png");

    while (!WindowShouldClose()) {
        if(start==true)
        {    BeginDrawing();
        
            if (eventTriggered(0.1)) {
                game.Update();
            }

            if (IsKeyPressed(KEY_W) && game.snake.direction.y != 1) {
                game.snake.direction = {0, -1};
                game.running = true;
            } else if (IsKeyPressed(KEY_S) && game.snake.direction.y != -1) {
                game.snake.direction = {0, 1};
                game.running = true;
            } else if (IsKeyPressed(KEY_A) && game.snake.direction.x != 1) {
                game.snake.direction = {-1, 0};
                game.running = true; 
            } else if (IsKeyPressed(KEY_D) && game.snake.direction.x != -1) {
                game.snake.direction = {1, 0};
                game.running = true;
            }

            ClearBackground(Retro1);
            DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, (float)cellSize * cellCount + 10, (float)cellSize * cellCount + 10}, 5, Retro2);
            DrawText("Goyda Snake", 75, 10, 40, Retro2);
            DrawText("Score:", 75, 80 + cellSize * cellCount, 40, Retro2);
            DrawText(TextFormat("%i", Score), 210, 80 + cellSize * cellCount, 40, Retro2);
            game.Draw();

            EndDrawing();
            }
        else if(start == false){
            BeginDrawing();
            ClearBackground(Retro1);

            name.Draw(225,65);
            wall.Draw(75,0);
            wall.Draw(750+45,0);
            button_start.Draw(260,400);
            button_exit.Draw(370,530);

            EndDrawing();
            
        }

    }
   
    CloseWindow();
    return 0;
}
