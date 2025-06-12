#include "pty.h"
#include <pty.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int spawn_shell(pid_t *child_pid) {
    int master_fd;
    struct winsize ws = {
        .ws_row = 24, // Default number of rows
        .ws_col = 80, // Default number of columns
        .ws_xpixel = 0,
        .ws_ypixel = 0,
    };

    *child_pid = forkpty(&master_fd, NULL, NULL, &ws);
    if (*child_pid < 0 ) {
        perror("forkpty");
        return -1;
    } else if (*child_pid == 0) {
        const char *shell = getenv("SHELL");
        if (!shell) shell = "/bin/sh";
        execlp(shell, shell, NULL);
        perror("execlp");
        exit(1);
    }

    return master_fd;
}
