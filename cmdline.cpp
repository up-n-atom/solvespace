#include "solvespace.h"
#include <stdarg.h>

void TextWindow::Init(void) {
    int i, j;
    for(i = 0; i < MAX_ROWS; i++) {
        for(j = 0; j < MAX_COLS; j++) {
            text[i][j] = ' ';
            meta[i][j].color = COLOR_NORMAL;
            meta[i][j].link = NOT_A_LINK;
        }
    }
    ClearCommand();
}

void TextWindow::Printf(char *fmt, ...) {
    va_list vl;
    va_start(vl, fmt);

    int r, c;
    if(rows < MAX_ROWS) {
        r = rows;
        rows++;
    } else {
        r = row0;
        row0++;
    }
    for(c = 0; c < MAX_COLS; c++) {
        text[r][c] = ' ';
        meta[r][c].link = NOT_A_LINK;
    }

    int color = COLOR_NORMAL;

    c = 0;
    while(*fmt) {
        if(*fmt == '%') {
        } else {
            if(c >= MAX_COLS) goto done;
            text[r][c++] = *fmt;
        }
        fmt++;
    }

done:
    va_end(vl);
}

void TextWindow::ClearCommand(void) {
    int j;
    for(j = 0; j < MAX_COLS; j++) {
        cmd[j] = ' ';
    }
    memcpy(cmd, "+> ", 3);
    cmdLen = 0;
    cmdInsert = 3;
    row0 = 0;
    rows = 0;
}

void TextWindow::KeyPressed(int c) {
    if(cmdLen >= MAX_COLS - 10) {
        ClearCommand();
        return;
    }

    if(c == '\n' || c == '\r') {
        // process the command, and then
        ClearCommand();
        return;
    } else if(c == 27) {
        ClearCommand();
    } else if(c == '\b') {
        // backspace, delete from insertion point
        if(cmdInsert <= 3) return;
        memmove(cmd+cmdInsert-1, cmd+cmdInsert, MAX_COLS-cmdInsert);
        cmdLen--;
        cmdInsert--;
    } else {
        cmd[cmdInsert] = c;
        cmdInsert++;
        cmdLen++;
    }
}

