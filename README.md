# phasmocrono_c
Timer utility for Phasmophobia ghost hunting developed in C

# build windows using w64devkit console
- gcc -o phasmocrono.exe main.c -mwindows -I include -L lib -lraylib -lgdi32 -lwinmm