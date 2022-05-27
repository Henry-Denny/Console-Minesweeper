#ifndef MENU_H
#define MENU_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <conio.h>

#define MENU_OPTION_BUFFER_SIZE 80

typedef struct Menu Menu;

struct Menu
{
    int numChoices;

    char preamble[1000];
    char **choices;

    bool isBase;                // The menu is the base (it is not the submenu of some other menu)
    void (**functions)(void);   // The function linked to choice[i]
    Menu **subMenus;            // The menu linked to choice[i]
};

Menu *CreateMenu(const char preamble[50], int numChoices, ...);
void DeleteMenu(Menu *menu);
void DisplayMenu(Menu *menu, int selected, bool clearing);
bool RunMenu(Menu *menu);
void LinkSubMenu(Menu *menu, Menu *subMenu, int opt);
void LinkFunction(Menu *menu, void (*function)(void), int opt);

#endif