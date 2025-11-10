#include <stdio.h>
#include "raylib.h"

#ifdef _WIN32
    #include <windows.h>  //GetAsyncKeyState()
#endif

typedef struct {
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

    const char *icon_path = "./assets/icon.png";
    Image icon = LoadImage(icon_path);
    SetWindowIcon(icon);

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
    int font_size = (int)(60 * scale);
    const float font_spacing = 1.0f * scale;

    const char *font_path = "./assets/CHILLER.TTF";
    Font chiller_font = LoadFontEx(font_path, font_size, NULL, 0);

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

#ifdef _WIN32
        if (GetAsyncKeyState(VK_END) & 0x8000)      start_pause(&running);
        if (GetAsyncKeyState(VK_NEXT) & 0x8000)     reset(&time, &running); // PAGE_DOWN = VK_NEXT
#else
        int key_pressed = GetKeyPressed();
        if (key_pressed)
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
#endif

        if (running)
        {
            time.seconds += delta_time;
            if (time.seconds >= 60)
            {
                time.seconds -= 60.0f;
                time.minutes++;
                if (time.minutes > 99) reset(&time, &running);
            }
        }

        const char *time_m_text = TextFormat("%02d", time.minutes);
        const char *time_s_text = TextFormat("%05.2f", time.seconds);

        char time_text[16] = {0};
        int position = 0;
        TextAppend(time_text, time_m_text, &position);
        TextAppend(time_text, ":", &position);
        TextAppend(time_text, time_s_text, &position);

        Vector2 text_size = MeasureTextEx(chiller_font, time_text, font_size, font_spacing);
        Vector2 text_pos = {
            .x = (GetScreenWidth() - text_size.x) / 2,
            .y = (GetScreenHeight() - text_size.y) / 2,
        };

        // START RENDERING
        BeginDrawing();

        ClearBackground(BLANK);
        DrawTextEx(chiller_font, time_text, text_pos, font_size, font_spacing, RAYWHITE);

        //DrawFPS(0, 0);
        EndDrawing();
    }

    UnloadFont(chiller_font);
    UnloadImage(icon);
    CloseWindow();
    return 0;
}
