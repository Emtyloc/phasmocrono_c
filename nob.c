#define NOB_IMPLEMENTATION
#include "nob.h"

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    Nob_Cmd cmd = {0};
    nob_cc(&cmd);
    nob_cc_inputs(&cmd, "main.c");

#if defined(_WIN32)
    nob_cc_output(&cmd, "phasmocrono.exe");
    nob_cmd_append(&cmd, "-lraylib", "-lopengl32", "-lgdi32", "-lwinmm");
#else
    nob_cc_output(&cmd, "phasmocrono");
    nob_cmd_append(&cmd, "-lraylib", "-lGL", "-lm", "-lpthread", "-ldl", "-lrt", "-lX11");
#endif

    if (!nob_cmd_run(&cmd)) return 1;
    nob_cmd_free(cmd);
    return 0;
}
