#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include "menu.h"
#include "game.h"

#define FILENAME "zaidejai.txt"

void clearScreen() {
    clear();
    refresh();
}

void printCentered(const char* text, int width, int spalvu_pora, int bold) {
    int tarpai = (width - strlen(text)) / 2;
    if (bold) attron(A_BOLD);
    attron(COLOR_PAIR(spalvu_pora));
    for (int i = 0; i < tarpai; i++) printw(" ");
    printw("%s\n", text);
    attroff(COLOR_PAIR(spalvu_pora));
    if (bold) attroff(A_BOLD);
}

void saugotiHighscore(const char* nickname, int score) {
    FILE* file = fopen(FILENAME, "a+");
    if (file == NULL) {
        printw("Error: Could not open file for writing.\n");
        return;
    }

    char name[51];
    int highscore;
    int found = 0;

    FILE* temp = fopen("temp.txt", "w");
    if (!temp) {
        printw("Error: Could not create temporary file.\n");
        fclose(file);
        return;
    }

    while (fscanf(file, "%s %d", name, &highscore) != EOF) {
        if (strcmp(name, nickname) == 0) {
            found = 1;
            if (score > highscore) highscore = score;
        }
        fprintf(temp, "%s %d\n", name, highscore);
    }

    if (!found) {
        fprintf(temp, "%s %d\n", nickname, score);
    }

    fclose(file);
    fclose(temp);
    remove(FILENAME);
    rename("temp.txt", FILENAME);
}


int read_highscore(const char* nickname) {
    FILE* file = fopen(FILENAME, "r");
    if (file == NULL) return 0;

    char name[51];
    int highscore;

    while (fscanf(file, "%s %d", name, &highscore) != EOF) {
        if (strcmp(name, nickname) == 0) {
            fclose(file);
            return highscore;
        }
    }

    fclose(file);
    return 0;
}

void gautiNickname(char* nickname) {
    clearScreen();

    int width = COLS;
    attron(A_BOLD | COLOR_PAIR(2));
    printCentered("=======================================", width, 2, 1);
    printCentered("===       ENTER YOUR NICKNAME       ===", width, 2, 1);
    printCentered("=======================================", width, 2, 1);
    attroff(A_BOLD | COLOR_PAIR(2));

    printw("\n");
    printCentered("Type your nickname and press ENTER:", width, 0, 0);
    move(LINES / 2, (COLS - 20) / 2);
    echo();
    getnstr(nickname, 50);
    noecho();

    clearScreen();
    attron(A_BOLD | COLOR_PAIR(1));
    printCentered("=======================================", width, 1, 1);
    printCentered("WELCOME TO SNAKE GAME", width, 1, 1);
    printCentered("=======================================", width, 1, 1);
    attroff(A_BOLD | COLOR_PAIR(1));
    printw("\n");
    printCentered("Welcome,", width, 0, 1);
    attron(COLOR_PAIR(1));
    printCentered(nickname, width, 1, 0);
    attroff(COLOR_PAIR(1));
    printw("\n\n");
    refresh();
    napms(3500);
}


void ekranoRezultatai() {
    clearScreen();
    FILE* file = fopen(FILENAME, "r");
    if (file == NULL) {
        printw("No results found. File does not exist.\n");
        refresh();

        refresh();
        napms(2000);
        return;
    }

    printw("Results from %s:\n", FILENAME);
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        printw("%s", line);
    }
    fclose(file);

    printw("\nPress any key to return to the menu.\n");
    refresh();
    getch();
}

void menu(const char* nickname) {
    int width = 50;
    char choice;

    while (1) {
        clearScreen();
    printCentered("~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^", width, 0, 0);
    printCentered("SNAKE GAME", width, 1, 1);
    printCentered("~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^", width, 0, 0);
    printw("\n");

    attron(COLOR_PAIR(1));
    printCentered("         /^\\/^\\                ", width, 1, 0);
    printCentered("       _|_O  O|                ", width, 1, 0);
    printCentered("\\/     /~     \\_/ \\                 ", width, 1, 0);
    printCentered(" \\______________/  \\                ", width, 1, 0);
    printCentered("        \\_______      \\                ", width, 1, 0);
    printCentered("                `\\     \\                 \\", width, 1, 0);
    printCentered("                  |     |                  \\", width, 1, 0);
    printCentered("                 /      /                    \\", width, 1, 0);
    printCentered("                /     /                       \\\\", width, 1, 0);
    printCentered("              /      /                         \\ \\", width, 1, 0);
    printCentered("             /     /                            \\  \\", width, 1, 0);
    printCentered("           /     /             _----_            \\   \\", width, 1, 0);
    printCentered("          /     /           _-~      ~-_         |   |", width, 1, 0);
    printCentered("         (      (        _-~    _--_    ~-_     _/   |", width, 1, 0);
    printCentered("          \\      ~-____-~    _-~    ~-_    ~-_-~    /", width, 1, 0);
    printCentered("            ~-_         _-~          ~-_       _-~", width, 1, 0);
    printCentered("               ~--______-~                ~-___-~", width, 1, 0);
    attroff(COLOR_PAIR(1));

    printw("\n\n");

    printCentered("Welcome,", width, 0, 0);
    attron(COLOR_PAIR(1));
    printw(" %s!\n", nickname);
    attroff(COLOR_PAIR(1));

        printCentered("Press 'S' to Start Game", width, 0, 0);
        printCentered("Press 'R' to View Results", width, 0, 0);
        printCentered("Press 'Q' to Quit", width, 0, 0);

        refresh();
        choice = getch();

        if (choice == 'S' || choice == 's') {
            clearScreen();
            int score = start_game(nickname);

            int current_highscore = read_highscore(nickname);
            if (score > current_highscore) {
                saugotiHighscore(nickname, score);
            }
        } else if (choice == 'R' || choice == 'r') {
            ekranoRezultatai();
        } else if (choice == 'Q' || choice == 'q') {
            clearScreen();
            printw("Exiting the game...\n");
            refresh();
            break;
        }
    }
}

int main() {
    char nickname[51];

    initscr();
    start_color();
    cbreak();
    noecho();
    curs_set(0);

    if (!has_colors()) {
        endwin();
        printf("Your terminal does not support color.\n");
        return 1;
    }

    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);

    gautiNickname(nickname);
    menu(nickname);

    endwin();
    return 0;
}
