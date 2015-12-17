#include <ncurses.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "diceRoller.h"
#include "/home/cubanfrog/Documents/Programs/mtwist-1.5/mtwist.h"
#define MAX_THROW_SIZE 256

//Clear screen method
int clear_screen(int screen_size)
{
        int number = 8, j;
        for (j = 8; j <= screen_size ; j++) {
                move(number,0);
                clrtoeol();
                number++;
        }
        return 1;
}

int modifier(int *results)
{
        int size = results[0], i, sum = 0;
        int value = results[size-1];
        for (i = 1; i < size-1;i++) {
                sum += results[i];
        }

        return value - sum;
}

//run method
void run() {
        initscr();
        raw();
        mt_seed();
//      keypad(stdcr,TRUE);
        int x,y,i,roll = 1, results[1000][MAX_THROW_SIZE];

        char diceRolls[1000][1000];
        char throw[MAX_THROW_SIZE];

        while (true) {
                getmaxyx(stdscr,y,x);
                if (roll == 950) {
                        mvprintw(y,0,"Clearing screen at 1000 rolls");
                }

                if (roll == 1000) {
                        clear_screen(y);
                        roll = 1;
                }

                mvprintw(0,0,"*******************");
                mvprintw(1,0,"*-Dice Wizard 0.1-*");
                mvprintw(2,0,"*******************");
                mvprintw(3,0,"Type 'quit' for quitting, 'clear' for clearing screen");
                mvprintw(4,0,"Dice format: XdY(-/+)Z without braces");

                for (i = 0; i < x; i++) {
                        mvprintw(5,i,"-");
                }

                mvprintw(6,0,"Input: ");

                for (i = 0; i < x; i++) {
                        mvprintw(7,i,"-");
                }

                //move and clear old input      
                move(6,7);
                clrtoeol();
                refresh();

                //User syntax
                if (scanw("%s",throw) < 0) {continue;}

                //quit the program
                if (strcmp("quit",throw) == 0) {
                        break;
                }

                //clear the window
                if (strcmp("clear", throw) == 0) {
                        clear_screen(y);
                        roll = 1;
                        continue;
                }

                int *result = throwDice(throw);
                //analyse Roll
                if (result[0] != 0) {
                        for (i = 0; i < result[0];i++) {results[roll][i] = result[i]; }
                        strcpy(diceRolls[roll], throw);
                } else {
                        continue;
                }

                int j, number = 0, raja = 1, rivi=0, riviRaja = 13;
                //write to the screen

                if (roll+7 >= y) {raja = roll+9-y;}

                for (j = roll; j >= raja; j--) {
                        move(8+number,0);
                        clrtoeol();
                        printw("%d. Roll: %s, answer: %d, Rolls:", j, diceRolls[j], results[j][results[j][0]-1]);
                        if (modifier(results[j]) != 0) {
                                for (i = 1; i < results[j][0]-1;i++){printw(" %d,",results[j][i]);}
                                printw(" Modifiers: %d",modifier(results[j]));
                        } else {
                                for (i = 1; i < results[j][0]-2;i++){
                                        if (i > riviRaja) {
                                                printw("\n");
                                                riviRaja += 22;
                                                number++;
                                        }
                                        printw(" %d,",results[j][i]);
                                }
                                printw(" %d",results[j][i]);
                        }
                        riviRaja = 13;

                        number++;
                }

                roll++;
        }

        endwin();
}

