#include "raylib.h"

int main()
{
    Texture2D car;
    Sound sound;
    Music music;

    InitWindow(800, 450, "First");
    InitAudioDevice();
    sound = LoadSound("clank-car-crash-collision-6206.mp3");
    music = LoadMusicStream("highoctane.mp3");

    SetTargetFPS(80);

    car = LoadTexture("Audi.png");
    //float posY = -car.height;
    float posX = car.width - 50;

    // SetTargetFPS(120);
    while (!WindowShouldClose())
    {

        UpdateMusicStream(music);
        // posY += GetFrameTime() * 350;
        posX += GetFrameTime() * 350;
        if (posX > 1100)
            posX = car.width - 150;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Rectangle spriteRect = {
                posX,
                0,
                car.height,
                car.width
            };

            if (CheckCollisionPointRec(GetMousePosition(), spriteRect))
                PlaySound(sound);
        }
        if (IsKeyPressed(KEY_W))
            PlayMusicStream(music);
        if (IsKeyPressed(KEY_SPACE))
            StopMusicStream(music);
        BeginDrawing();
        ClearBackground(RAYWHITE);
       // Rectangle car = (Rectangle){ 0,0,256,128 };
        //DrawTexture(car, 100, -posY, WHITE);
        DrawTextureEx(car, (Vector2){posX,0}, 90 , 1.0, WHITE);
        // DrawRectangle(posX,100,100,100,BLUE);
        EndDrawing();
    }
    CloseAudioDevice();
    StopMusicStream(music);
    CloseWindow();

    return 0;
}