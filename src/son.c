#include "raylib.h"

int main(void)
{
    Sound sound;
    Music music;
    int width = 800;
    int height = 450;

    InitWindow(width, height, "title");
    InitAudioDevice();

    sound = LoadSound("../assets/test.wav");
    music = LoadMusicStream("../assets/test.wav");
    PlayMusicStream(music);

    while (!WindowShouldClose())
    {
        UpdateMusicStream(music);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            PlaySound(sound);
        }

        BeginDrawing();
        ClearBackground(GRAY);
        DrawText("MUSIC SHOULD BE PLAYING!", width / 2, height / 2, 20, LIGHTGRAY);
        EndDrawing();
    }
    StopMusicStream(music);

    CloseAudioDevice();

    CloseWindow();

    return 0;
}