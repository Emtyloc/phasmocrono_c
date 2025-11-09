#include <stdio.h>
#include "raylib.h"

typedef struct {
    int hours;
    int minutes;
    float seconds;
} Time;

void start_pause(bool *running)
{
    *running = !*running;
}

void reset(Time *time, bool *running)
{
    time->minutes = 0;
    time->seconds = 0.0f;

    *running = false;
}

int main(int argc, char **argv)
{
    SetConfigFlags(FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_TRANSPARENT);

    const int DEFAULT_WINDOW_WIDTH = 200;
    const int DEFAULT_WINDOW_HEIGHT = 75;
    const int MONITOR_ID = 0;

    InitWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "Phasmocrono");
    SetWindowState(FLAG_WINDOW_TOPMOST);
    SetExitKey(KEY_PAGE_UP);

    SetTargetFPS(30);
    SetWindowMonitor(MONITOR_ID);

    int monitor_x = GetMonitorPosition(MONITOR_ID).x;
    int monitor_y = GetMonitorPosition(MONITOR_ID).y;
    int monitor_width = GetMonitorWidth(MONITOR_ID);

    float scale = (float)monitor_width / 2560.0f;

    int window_width = (int)(200 * scale);
    int window_height = (int)(75 * scale);
    int font_size = (int)(40 * scale);

    SetWindowSize(window_width, window_height);
    int x = monitor_x + (monitor_width - window_width) / 2;
    int y = monitor_y;
    SetWindowPosition(x, y);

    
    //setup crono
    Time time = {0};
    bool running = false;

    while (!WindowShouldClose())
    {
        float delta_time = GetFrameTime();
        
        int key_pressed = GetKeyPressed();
        if(key_pressed)
        {
            switch (key_pressed)
            {
                case KEY_END:
                    start_pause(&running);
                    break;
                case KEY_PAGE_DOWN:
                    reset(&time, &running);
                    break;
            }
        }
        if (running)
        {
            time.seconds += delta_time;
            if (time.seconds >= 60)
            {
                const float rest_seconds = time.seconds - 60.0f;
                time.seconds = rest_seconds;
                time.minutes++;
            }
        }
        
        const char *time_m_text = TextFormat("%02d", time.minutes);
        const char *time_s_text = TextFormat("%05.2f", time.seconds);
        
        char time_text[16] = {0};
        int position = 0;
        TextAppend(time_text, time_m_text, &position);
        TextAppend(time_text, ":", &position);
        TextAppend(time_text, time_s_text, &position);        
        
        int textWidth = MeasureText(time_text, font_size);
        
        // START RENDERING
        BeginDrawing();
        ClearBackground(BLANK);

        DrawText(
            time_text,
            (GetScreenWidth() - textWidth) / 2,
            (GetScreenHeight() - font_size) / 2,
            font_size,
            RED
        );

        //DrawFPS(0, 0);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
