#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game.h"
#include "menu.h"

void inicializacija(int a, int q, int p, int val[q][p]);
void grafika(int a, int q, int p, int val[q][p], WINDOW *win, const char* nickname, int score, int best_score);
void gamestate_update(int *a, int q, int p, int val[q][p], WINDOW *win, int komanda, int *y, int *x, int *status, int *score);
void obuolys(int q, int p, int val[q][p]);
void end_game(int q, int p, WINDOW *win);

int start_game(const char* nickname) {
    int aukstis = 25, plotis = 75, ch = KEY_RIGHT, y, x;
    int val[aukstis][plotis];
    int status = 1;
    int komanda = 1;
    int gyvates_ilgis = 3;
    int score = 0;
    int best_score = read_highscore(nickname);

    WINDOW *win = initscr();
    curs_set(0);
    box(win, 0, 0);
    cbreak();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    x = plotis / 2;
    y = aukstis / 2;

    inicializacija(gyvates_ilgis, aukstis, plotis, val);

    while (status != 0) {
        ch = getch();
        if (ch == KEY_RIGHT && val[y][x + 1] != (gyvates_ilgis - 1)) komanda = 1;
        if (ch == KEY_UP && val[y - 1][x] != (gyvates_ilgis - 1)) komanda = 2;
        if (ch == KEY_LEFT && val[y][x - 1] != (gyvates_ilgis - 1)) komanda = 3;
        if (ch == KEY_DOWN && val[y + 1][x] != (gyvates_ilgis - 1)) komanda = 4;

        grafika(gyvates_ilgis, aukstis, plotis, val, win, nickname, score, best_score);
        gamestate_update(&gyvates_ilgis, aukstis, plotis, val, win, komanda, &y, &x, &status, &score);

        if (komanda == 1 || komanda == 3) napms(100);
        if (komanda == 2 || komanda == 4) napms(150);
    }

    end_game(aukstis, plotis, win);

    getch();
    endwin();

    return score;
}

void inicializacija(int a, int q, int p, int val[q][p]) {
    int i, j;

    for (i = 0; i < q; i++) {
        for (j = 0; j < p; j++) {
            val[i][j] = 0;
            if (i == 0 || i == (q - 1) || j == 0 || j == (p - 1)) val[i][j] = -2;
        }
    }

    for (i = 0; i < a; i++) {
        val[(q / 2)][(p / 2) - i] = a - i;
    }
    obuolys(q, p, val);
}

void grafika(int a, int q, int p, int val[q][p], WINDOW *win, const char* nickname, int score, int best_score) {
    char simb;
    int i, j;

    for (i = 0; i < q; i++) {
        for (j = 0; j < p; j++) {
            if (val[i][j] == a) simb = '@';
            if (val[i][j] > 0 && val[i][j] < a) simb = 'O';
            if (val[i][j] == 0) simb = ' ';
            if (val[i][j] == -1) simb = 'o';
            if (val[i][j] == -2) simb = '#';
            mvwprintw(win, i + 1, j + 1, "%c", simb);
        }
    }

    mvwprintw(win, 1, p + 2, "Nickname: %s", nickname);
    mvwprintw(win, 2, p + 2, "Current Score: %d", score);
    mvwprintw(win, 3, p + 2, "Your Highscore: %d", best_score);

    refresh();
}

void gamestate_update(int *a, int q, int p, int val[q][p], WINDOW *win, int komanda, int *y, int *x, int *status, int *score) {
    int i, j;

    if (komanda == 1) *x = *x + 1;
    if (komanda == 2) *y = *y - 1;
    if (komanda == 3) *x = *x - 1;
    if (komanda == 4) *y = *y + 1;

    if (val[*y][*x] == -2) *status = 0;
    if (val[*y][*x] > 0 && val[*y][*x] != (*a - 1)) *status = 0;
    if (val[*y][*x] == 0) val[*y][*x] = *a + 1;

    if (val[*y][*x] == -1) {
        *a = *a + 1;
        *score += 100; // +100 points score
        val[*y][*x] = *a;
        for (i = 0; i < q; i++) {
            for (j = 0; j < p; j++) {
                if (val[i][j] > 0) val[i][j] = val[i][j] + 1;
            }
        }
        obuolys(q, p, val);
    }

    for (i = 0; i < q; i++) {
        for (j = 0; j < p; j++) {
            if (val[i][j] > 0) val[i][j] = val[i][j] - 1;
        }
    }
}

void obuolys(int q, int p, int val[q][p]) {
    int ran1, ran2, status = 1;
    while (status != 0) {
        srand(time(0));
        ran1 = rand() % (q - 1) + 1; //[1;24]
        ran2 = rand() % (p - 1) + 1; //[1;74]
        if (val[ran1][ran2] == 0) {
            val[ran1][ran2] = -1;
            status = 0;
        }
    }
}

void end_game(int q, int p, WINDOW *win) {
    nodelay(stdscr, FALSE);
    keypad(stdscr,FALSE);

    int i, j, z, vid_aukst, vid_plot;
    char arr[q][p];

    for (i = 0; i < q; i++) {
        for (j = 0; j < p; j++) {
            arr[i][j] = ' ';
        }
    }

    vid_aukst = q / 2;
    vid_plot = (p - 11) / 2;

    char *game_over = " GAME OVER ";

    for (i = 0; i < 11; i++) {
        arr[vid_aukst][vid_plot + i] = game_over[i];
    }

    for (z = 0; z < (q / 2 - 1); z++) {
        for (i = 0; i < q; i++) {
            for (j = 0; j < p; j++) {
                if (i == z || i == (q - (z + 1)) || j == z || j == (p - (z + 1))) arr[i][j] = '#';
                mvwprintw(win, i + 1, j + 1, "%c", arr[i][j]);
            }
        }
        refresh();
        napms(150);
    }

    mvprintw(q+2, p -50, "Press any key to return to menu...");
    refresh();

    keypad(stdscr,TRUE);
    getch();
}
