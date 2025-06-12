#include "screen.h"
#include <c++/15.1.1/bits/ostream.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum ParserState {
    STATE_NORMAL,
    STATE_ESC,
    STATE_CSI
};

static enum ParserState state = STATE_NORMAL;
static char csi_buf[32];
static int csi_len = 0;

void parser_feed(char byte) {
    switch (state) {
        case STATE_NORMAL:
            if (byte == 0x1b) {
                state = STATE_ESC;
            } else {
                put_char_at_cursor(byte);
            }
            break;
        
        case STATE_ESC:
            if (byte == '[') {
                state = STATE_CSI;
                csi_len = 0;
            } else {
                state = STATE_NORMAL;
            }
            break;

        case STATE_CSI:
            if ((byte >= '0' && byte <= '9' || byte == ';')) {
                if (csi_len < (int)sizeof(csi_buf) - 1) {
                    csi_buf[csi_len++] = byte;
                }
            } else {
                csi_buf[csi_len] = '\0';
                handle_csi(csi_buf, byte);
                state = STATE_NORMAL;
            }
            break;
    }
}
