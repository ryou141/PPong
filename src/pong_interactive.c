#include <stdio.h>
#include <termios.h>
#include <unistd.h>

int movement1(char n, int move, int flag);
int movement2(char n, int move, int flag);
int ball_move_x(int x, int flag);
int ball_move_y(int y, int flag);
int flag_check(int flag, int y);
int clearTerminal();
int point_check_(int x);
int pointerr(int x, int pointsr);
int pointerl(int x, int pointsl);
void winner(int pointsr, int pointsl);
int ball_bounce_l(int flag, int ball_x, int ball_y, int r1y1, int r1y2, int r1y3);
int ball_bounce_r(int flag, int ball_x, int ball_y, int r2y1, int r2y2, int r2y3);

void reset_terminal_mode() { tcsetattr(0, TCSANOW, &(struct termios){0}); }

int kbhit() {
    struct timeval tv = {0, 0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

int main() {
    int r1x = 3, r1y1 = 12, r1y2 = 13, r1y3 = 14, x_len = 82, y_len = 27, move_count = 1, pointsr = 0;
    int r2x = 78, r2y1 = 12, r2y2 = 13, r2y3 = 14, ball_x = 40, ball_y = 13, flag = 1, pointsl = 0;
    char move = ' ';
    struct termios new_tio;
    tcgetattr(0, &new_tio), new_tio.c_lflag = ~ICANON, tcsetattr(0, TCSAFLUSH, &new_tio);
    while (pointsl < 21 && pointsr < 21) {
        printf("\e[1;1H\e[2J");
        if (move_count == 1) {
            flag = flag_check(flag, ball_y), flag = ball_bounce_l(flag, ball_x, ball_y, r1y1, r1y2, r1y3);
            flag = ball_bounce_r(flag, ball_x, ball_y, r2y1, r2y2, r2y3), ball_x = ball_move_x(ball_x, flag),
            ball_y = ball_move_y(ball_y, flag);
            printf("LEFT: %d                                                                  RIGHT: %d\n",
                   pointsl, pointsr);
            for (int i = 0; i < y_len; i++) {
                for (int j = 0; j < x_len; j++) {
                    if (i == 0 || i == y_len - 1) {
                        printf("=");
                    } else if (((i == r1y1 || i == r1y2 || i == r1y3) && j == r1x) ||
                               ((i == r2y1 || i == r2y2 || i == r2y3) && j == r2x)) {
                        printf("█");
                    } else if ((i == ball_y) && (j == ball_x)) {
                        printf("•");
                    } else if (j == 40) {
                        printf("|");
                    } else {
                        printf(" ");
                    }
                }
                printf("\n");
            }
            if (point_check_(ball_x) == 1) {
                pointsr = pointerr(ball_x, pointsr), pointsl = pointerl(ball_x, pointsl), ball_x = 40;
            }
            move_count = 0, move = ' ';
        }
        if (kbhit()) {
            scanf("%c", &move);
        }
        if (r1y1 >= 1 && r1y3 <= 25) {
            r1y1 = movement1(move, r1y1, 1), r1y2 = movement1(move, r1y2, 2), r1y3 = movement1(move, r1y3, 3);
        }
        if (r2y1 >= 1 && r2y3 <= 25) {
            r2y1 = movement2(move, r2y1, 1), r2y2 = movement2(move, r2y2, 2), r2y3 = movement2(move, r2y3, 3);
        }
        move_count++, usleep(50000);
    }
    reset_terminal_mode(), winner(pointsr, pointsl);
    return 0;
}

int movement1(char n, int move, int flag) {
    if (flag == 1 && ((move == 1 && (n == 'A' || n == 'a')) || (move == 23 && (n == 'Z' || n == 'z')))) {
        return move;
    }
    if (flag == 2 && ((move == 2 && (n == 'A' || n == 'a')) || (move == 24 && (n == 'Z' || n == 'z')))) {
        return move;
    }
    if (flag == 3 && ((move == 3 && (n == 'A' || n == 'a')) || (move == 25 && (n == 'Z' || n == 'z')))) {
        return move;
    }
    switch (n) {
        case 'A':
        case 'a':
            move--;
            break;
        case 'Z':
        case 'z':
            move++;
            break;
    }
    return move;
}

int movement2(char n, int move, int flag) {
    if (flag == 1 && ((move == 1 && (n == 'K' || n == 'k')) || (move == 23 && (n == 'M' || n == 'm')))) {
        return move;
    }
    if (flag == 2 && ((move == 2 && (n == 'K' || n == 'k')) || (move == 24 && (n == 'M' || n == 'm')))) {
        return move;
    }
    if (flag == 3 && ((move == 3 && (n == 'K' || n == 'k')) || (move == 25 && (n == 'M' || n == 'm')))) {
        return move;
    }
    switch (n) {
        case 'K':
        case 'k':
            move--;
            break;
        case 'M':
        case 'm':
            move++;
            break;
    }
    return move;
}

int ball_move_x(int x, int flag) {
    if (flag == 1 || flag == 4) {
        x--;
    } else if (flag == 2 || flag == 3) {
        x++;
    }
    return x;
}

int ball_move_y(int y, int flag) {
    if (flag == 1 || flag == 2) {
        y--;
    } else if (flag == 3 || flag == 4) {
        y++;
    }
    return y;
}

int flag_check(int flag, int y) {
    if (y == 1) {
        if (flag == 1) {
            flag = 4;
        } else if (flag == 2) {
            flag = 3;
        }
    }
    if (y == 25) {
        if (flag == 4) {
            flag = 1;
        } else if (flag == 3) {
            flag = 2;
        }
    }
    return flag;
}

int point_check_(int x) {
    if (x <= 1 || x >= 80) {
        return 1;
    }

    return 0;
}

int pointerr(int x, int pointsr) {
    if (x <= 1) {
        pointsr += 1;
    }
    return pointsr;
}

int pointerl(int x, int pointsl) {
    if (x >= 80) {
        pointsl++;
    }

    return pointsl;
}

void winner(int pointsr, int pointsl) {
    if (pointsr == 21) {
        printf("\e[1;1H\e[2J");
        printf("Right player win!!!");
    }
    if (pointsl == 21) {
        printf("\e[1;1H\e[2J");
        printf("Left player win!!!");
    }
}

int ball_bounce_l(int flag, int ball_x, int ball_y, int r1y1, int r1y2, int r1y3) {
    if ((flag == 1) && ball_x == 4) {
        if (ball_y == r1y1 || ball_y == r1y2 || ball_y == r1y3) {
            flag = 2;
        }
    }
    if ((flag == 4) && ball_x == 4) {
        if (ball_y == r1y1 || ball_y == r1y2 || ball_y == r1y3) {
            flag = 3;
        }
    }
    return flag;
}
int ball_bounce_r(int flag, int ball_x, int ball_y, int r2y1, int r2y2, int r2y3) {
    if ((flag == 2) && ball_x == 77) {
        if (ball_y == r2y1 || ball_y == r2y2 || ball_y == r2y3) {
            flag = 1;
        }
    }
    if ((flag == 3) && ball_x == 77) {
        if (ball_y == r2y1 || ball_y == r2y2 || ball_y == r2y3) {
            flag = 4;
        }
    }
    return flag;
}