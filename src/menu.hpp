#include<raylib.h>
#include<iostream>
using namespace std;

Color Retro1 = Color{173, 204, 96, 255};
Color Retro2 = Color{43, 51, 24, 255};

bool start = false;
int cellSize = 30;
int cellCount = 25;
int offset = 75;

class Sprite{
public:
    Texture2D sprite;

    Sprite(const char* image){
        sprite = LoadTextureFromImage(LoadImage(image));
    }

    ~Sprite(){
        UnloadTexture(sprite);
    }

    void Draw(int x, int y){
        DrawTexture(sprite,x,y,WHITE);
    }
};
