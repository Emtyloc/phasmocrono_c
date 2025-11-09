#define NOB_IMPLEMENTATION
#include "nob.h"

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    Nob_Cmd cmd = {0};
    bool build_for_windows = false;

    while (argc > 1) {
        char *arg = nob_shift_args(&argc, &argv);
        if (strcmp(arg, "--win") == 0) {
            build_for_windows = true;
        }
    }

    if (build_for_windows) {
        nob_cmd_append(&cmd,
            "x86_64-w64-mingw32-gcc",
            "main.c",
            "-O2", "-s",
            "-o", "phasmocrono.exe",
            "-Iraylib_win/include",
            "-Lraylib_win/lib",
            "-lraylib",
            "-lopengl32", "-lgdi32", "-lwinmm"
        );
        nob_log(NOB_INFO, "Compiling for Windows (cross-build using mingw-w64)");
    } else {
#if defined(_WIN32)
        nob_cmd_append(&cmd, "gcc", "main.c",
            "-O2", "-s",
            "-o", "phasmocrono.exe",
            "-lraylib", "-lopengl32", "-lgdi32", "-lwinmm"
        );
        nob_log(NOB_INFO, "Compiling for Windows (native)");
#else
        nob_cc(&cmd);
        nob_cc_output(&cmd, "phasmocrono");
        nob_cc_inputs(&cmd, "main.c");
        nob_cmd_append(&cmd,
            "-O2", "-s",
            "-lraylib", "-lGL", "-lm", "-lpthread", "-ldl", "-lrt", "-lX11"
        );
        nob_log(NOB_INFO, "Compiling for Linux (native)");
#endif
    }

    if (!nob_cmd_run(&cmd)) {
        nob_cmd_free(cmd);
        return 1;
    }

    nob_cmd_free(cmd);
    return 0;
}
