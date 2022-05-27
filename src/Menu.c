#include "Menu.h"

#include "Keys.h"

Menu *CreateMenu(const char *preamble, int numChoices, ...)
{
    Menu *menu = malloc(sizeof(Menu));
    menu->isBase = true;
    numChoices++;   // Include the quit option in the menu list
    menu->numChoices = numChoices;
    strcpy(menu->preamble, preamble);

    char **choices = malloc(numChoices * sizeof(char *));

    va_list valist;
    va_start(valist, numChoices);
    for (int i = 0; i < (numChoices - 1); ++i)
    {
        choices[i] = (char *)malloc(MENU_OPTION_BUFFER_SIZE);
        choices[i] = va_arg(valist, char *);
    }
    choices[numChoices - 1] = (menu->isBase) ? "Quit" : "Back";   // If the menu is not the main one, make the 'quit' button a 'back' button instead
    va_end(valist);
    
    menu->choices = choices;

    // Setup functions and sub menus
    void (**functions)(void) = malloc((numChoices - 1) * sizeof(void *));
    Menu **subMenus = malloc((numChoices - 1) * sizeof(Menu *));

    for (int i = 0; i < (numChoices - 1); ++i)
    {
        functions[i] = NULL;
        subMenus[i] = NULL;
    }
    menu->functions = functions;
    menu->subMenus = subMenus;

    return menu;
}

void DeleteMenu(Menu *menu)
{
    for (int i = 0; i < menu->numChoices; ++i)
    {
        free(menu->choices[i]);
    }
    free(menu->choices);
    free(menu->functions);
    free(menu);
    menu = NULL;
}

void DisplayMenu(Menu *menu, int selected, bool clearing)
{
    if (clearing) system("cls");
    printf("%s\n\n", menu->preamble);

    for (int opt = 1; opt <= menu->numChoices; ++opt)
    {
        if (opt == selected)
        {
            printf("\033[3;42;30m[%i]: %s\n\033[0m", opt, menu->choices[opt - 1]);
        }
        else
        {
            printf("%i: %s\n", opt, menu->choices[opt - 1]);
        }
    }
    printf("\n\n");
}

bool RunMenu(Menu *menu)
{
    bool menuOpen = true;
    int choice = 1;
    while(menuOpen)
    {
        DisplayMenu(menu, choice, true);
        int ch = getch();

        switch(ch) {
        case KEY_UP:
            if (choice > 1) choice--;
            break;
        case KEY_DOWN:
            if (choice < menu->numChoices) choice++;
            break;
        case KEY_ENTER:
        case KEY_RIGHT:
            // Process the selection of the chosen menu option
            if (choice == menu->numChoices) menuOpen = false;    // The last option (quit) has been selected
            else
            {
                if (menu->functions[choice - 1])
                {
                    menu->functions[choice - 1]();
                }
                else if (menu->subMenus[choice - 1])
                {
                    if(RunMenu(menu->subMenus[choice - 1])) return true;    // If should quit, then return true
                }
                else
                {
                    printf("[NO FUNCTION LINKED TO THIS MENU OPTION] ");
                    getch();
                }
            }
            break;
        case KEY_LEFT:
            if (!menu->isBase) menuOpen = false;
            break;
        case KEY_Q:
            return true;
        default:
            // printf("%i", ch);
            break;
        }
    }
    return false;
}

void LinkSubMenu(Menu *menu, Menu *subMenu, int opt)
{
    if (opt >= 1 && opt < menu->numChoices)
    {
        menu->subMenus[opt - 1] = subMenu;
        subMenu->isBase = false;
        subMenu->choices[subMenu->numChoices - 1] = "Back";
        return;
    }
    printf("[ERROR LINKING SUBMENU TO MENU OPTION] ");
    getch();
}

void LinkFunction(Menu *menu, void (*function)(void), int opt)
{
    if (opt >= 1 && opt < menu->numChoices)
    {
        menu->functions[opt - 1] = function;
        return;
    }
    printf("[ERROR LINKING FUNCTION TO MENU OPTION] ");
    getch();
}