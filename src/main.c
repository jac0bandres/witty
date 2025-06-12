#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "pty.h"
#include "term.h"
#include "screen.h"
#include "render.h"
#include "log.h"

int main() {
    log_init("witty.log");
    pid_t child; 
    int pty_fd = spawn_shell(&child);
    if (pty_fd < 0) {
        fprintf(stderr, "Failed to spawn shell\n");
        return 1;
    }
    printf("Spawned shell with PID %d, PTY FD %d\n", child, pty_fd);

    int rowSize = 20;
    int colSize = 84;
    int *rows = &rowSize;
    int *cols = &colSize;
    log_info("Initializing renderer with rows: %d, cols: %d", *rows, *cols);
    init_renderer(rows, cols);
    printf("window size: %d, %d", *rows, *cols);
    Screen *screen = init_screen(*rows, *cols);
    enable_raw_mode();

    char buf[1024];
    ssize_t n;

    fd_set fds;
    while (1) {
        FD_ZERO(&fds); FD_SET(STDIN_FILENO, &fds);
        FD_SET(pty_fd, &fds);

        int max_fd = (STDIN_FILENO > pty_fd ? STDIN_FILENO : pty_fd) + 1;

         if(select(max_fd, &fds, NULL, NULL, NULL) < 0) {
            perror("select");
            break;
        }

        if (FD_ISSET(STDIN_FILENO, &fds)) {
            n = read(STDIN_FILENO, buf, sizeof(buf));
            if (n <= 0) {
                perror("read from stdin");
                break;
            }
            write(pty_fd, buf, n);
        }

        if (FD_ISSET(pty_fd, &fds)) {
            n = read(pty_fd, buf, sizeof(buf));
            if (n <= 0) {
                perror("read from pty");
                break;
            }
            for (ssize_t i = 0; i < n; i++) {
                put_char_at_cursor(buf[i], screen);
            }
            render_screen(screen);
        }
    }

    destroy_screen(screen);
    destroy_renderer();
    restore_terminal();

    int status;
    waitpid(child, &status, 0);
    perror("Child process exited");

    return 0;
}
