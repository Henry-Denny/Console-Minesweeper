#ifndef MINEFIELD_H
#define MINEFIELD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <conio.h>

#include "Colours.h"

#define ROWS 10
#define COLS 10

typedef struct Tile Tile;

struct Tile
{
    bool    isMine;
    bool    isHidden;
    int     nearbyMines;
};

int cmpFunc(const void * a, const void * b);

void DisplayMinefield(Tile* mineField[ROWS][COLS], int sRow, int sCol);
void SetupMinefield(Tile* mineField[ROWS][COLS], int numMines);
void PlantMines(Tile* mineField[ROWS][COLS], int numMines);
void DeleteMinefield(Tile* mineField[ROWS][COLS]);
bool InteractTile(Tile* minefield[ROWS][COLS], int row, int col, int *numDiscovered);
void DiscoverTile(Tile* minefield[ROWS][COLS], int row, int col, int *numDiscovered);
int NumNearbyMines(Tile* minefield[ROWS][COLS], int row, int col);
void DiscoverNearbyTiles(Tile *minefield[ROWS][COLS], int row, int col, int *numDiscovered);

#endif