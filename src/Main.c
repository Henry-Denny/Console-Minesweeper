#include <stdio.h>

#include "Menu.h"
#include "Minefield.h"
#include "Keys.h"

#define MINES 10

void NewGame();
void GameOver();
void GameWon();

int main()
{
    Menu *mainMenu = CreateMenu
    (
        "Welcome to:\n"
        Blue
        " __  __ ___ _   _ _____ ______        _______ _____ ____  _____ ____  \n"
        Cyan
        "|  \\/  |_ _| \\ | | ____/ ___\\ \\      / / ____| ____|  _ \\| ____|  _ \\ \n"
        Green
        "| |\\/| || ||  \\| |  _| \\___ \\\\ \\ /\\ / /|  _| |  _| | |_) |  _| | |_) |\n"
        Purple
        "| |  | || || |\\  | |___ ___) |\\ V  V / | |___| |___|  __/| |___|  _ < \n"
        Red
        "|_|  |_|___|_| \\_|_____|____/  \\_/\\_/  |_____|_____|_|   |_____|_| \\_\\\n"
        Clear
        "\n"
        "Start a new game to begin",
        1,
        "New Game"
    );
    LinkFunction(mainMenu, NewGame, 1);
    
    RunMenu(mainMenu);

    DeleteMenu(mainMenu);
    return 0;
}

void NewGame()
{
    Tile* minefield[ROWS][COLS];
    SetupMinefield(minefield, MINES);

    bool done = false;
    int selected[2] = {0, 0};

    int numDiscovered = 0;
    while(!done)
    {
        DisplayMinefield(minefield, selected[1], selected[0]);
        int ch = getch();
        switch(ch)
        {
            case KEY_UP:
                if (selected[1] > 0) selected[1]--;
                break;
            case KEY_DOWN:
                if (selected[1] < ROWS - 1) selected[1]++;
                break;
            case KEY_RIGHT:
                if (selected[0] < COLS - 1) selected[0]++;
                break;
            case KEY_LEFT:
                if (selected[0] > 0) selected[0]--;
                break;
            case KEY_ENTER:
                if (InteractTile(minefield, selected[1], selected[0], &numDiscovered))
                {
                    done = true;
                    GameOver();
                }
                
                break;
            case KEY_Q:
                done = true;
                break;
            default:
                // printf("%i", ch);
                break;
        }
        // Check for Win
        if (numDiscovered == (ROWS * COLS) - MINES) 
        {
            done = true;
            GameWon();
        }
    }

    DeleteMinefield(minefield);
}

void GameOver()
{
    system("cls");
    printf
    (
        Red
        "  ____                         ___                 \n"  
        " / ___| __ _ _ __ ___   ___   / _ \\__   _____ _ __ \n"
        "| |  _ / _` | '_ ` _ \\ / _ \\ | | | \\ \\ / / _ \\ '__|\n"
        "| |_| | (_| | | | | | |  __/ | |_| |\\ V /  __/ |   \n"
        " \\____|\\__,_|_| |_| |_|\\___|  \\___/  \\_/ \\___|_|   \n"
        "\n"
        Clear
    );

    printf("YOU HIT A MINE\n\n");

    printf("Press any key to return to the main menu... ");
    getch();
}

void GameWon()
{
    system("cls");
    printf
    (
        Green
        "__   __           __        _____  _   _\n"
        "\\ \\ / /__  _   _  \\ \\      / / _ \\| \\ | |\n"
        " \\ V / _ \\| | | |  \\ \\ /\\ / / | | |  \\| |\n"
        "  | | (_) | |_| |   \\ V  V /| |_| | |\\  |\n"
        "  |_|\\___/ \\__,_|    \\_/\\_/  \\___/|_| \\_|\n"
        "\n"
        Clear
    );
    printf("Press any key to return to the main menu...");
    getch();
}