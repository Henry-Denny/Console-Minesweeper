#include "Minefield.h"

// Funciton definitions

void DisplayMinefield(Tile* mineField[ROWS][COLS], int sRow, int sCol)
{
    system("cls");
    printf("\n");
    for (int row = 0; row < ROWS; ++row)
    {
        char lines[3][1 + (20 * COLS) + 1] = {" ", " ", " "};
        for (int col = 0; col < COLS; ++col)
        {
            if (row == sRow && col == sCol)
            {
                strcat(lines[0], "#######");
                strcat(lines[1], "#######");
                strcat(lines[2], "#######");
            }
            else if (mineField[row][col]->isHidden)
            {
                strcat(lines[0], Blue);
                strcat(lines[1], Blue);
                strcat(lines[2], Blue);
                strcat(lines[0], "-------");
                strcat(lines[1], "-------");
                strcat(lines[2], "-------");
                strcat(lines[0], Clear);
                strcat(lines[1], Clear);
                strcat(lines[2], Clear);
            }
            else
            {
                if (mineField[row][col]->isMine)
                {
                    strcat(lines[0], "#-#-#-#");
                    strcat(lines[1], "-#-#-#-");
                    strcat(lines[2], "#-#-#-#");
                }
                else
                {
                    int nearbyMines = mineField[row][col]->nearbyMines;
                    if (nearbyMines == 0)
                    {
                        strcat(lines[0], "       ");
                        strcat(lines[1], "       ");
                        strcat(lines[2], "       ");
                    }
                    else
                    {
                        strcat(lines[0], "`     `");
                        strcat(lines[1], "`  ");
                        char buffer[2];
                        sprintf(buffer, "%i", nearbyMines);
                        strcat(lines[1], Clear);
                        strcat(lines[1], buffer);
                        strcat(lines[1], "  `");
                        strcat(lines[2], "`     `");
                    }
                }
            }
            strcat(lines[0], " ");
            strcat(lines[1], " ");
            strcat(lines[2], " ");
        }
        printf("%s\n%s\n%s\n\n", lines[0], lines[1], lines[2]);
    }
}

void SetupMinefield(Tile* mineField[ROWS][COLS], int numMines)
{
    int remainingMines = numMines;
    for (int row = 0; row < ROWS; ++row)
    {
        for (int col = 0; col < COLS; ++col)
        {
            Tile *tile = malloc(sizeof(Tile));
            tile->isHidden = true;
            tile->isMine = false;
            tile->nearbyMines = -1;
            mineField[row][col] = tile;
        }
    }

    PlantMines(mineField, numMines);

    // Update the number of nearby mines of each tile
    for (int row = 0; row < ROWS; ++row)
    {
        for (int col = 0; col < COLS; ++col)
        {
            if (mineField[row][col]->isMine)
            {
                mineField[row][col]->nearbyMines = -1;
                continue;
            }
            mineField[row][col]->nearbyMines = NumNearbyMines(mineField, row, col);
        }
    }
}

int cmpFunc(const void * a, const void * b)
{
    return ( *(int*)b - *(int*)a );
}

void PlantMines(Tile* mineField[ROWS][COLS], int numMines)
{
    srand(time(0));
    int availableTiles[ROWS * COLS];
    int numAvailableTiles = ROWS * COLS;
    for (int i = numAvailableTiles - 1; i >= 0; --i)
    {
        availableTiles[i] = i;
    }

    for (int i = 0; i < numMines; ++i)
    {
        int randIndex = rand() % (numAvailableTiles + 1);
        int tileID = availableTiles[randIndex];

        // Do stuff with the ID
        int col = tileID % COLS;
        int row = (tileID - col) / COLS;
        mineField[row][col]->isMine = true;

        availableTiles[randIndex] = 0;
        qsort(availableTiles, numAvailableTiles, sizeof(int), cmpFunc);
        numAvailableTiles--;
    }
}

void DeleteMinefield(Tile* mineField[ROWS][COLS])
{
    for (int x = 0; x < ROWS; ++x)
    {
        for (int y = 0; y < COLS; ++y)
        {
            free(mineField[x][y]);
            mineField[x][y] = NULL;
        }
    }
}

bool InteractTile(Tile* minefield[ROWS][COLS], int row, int col, int *numDiscovered)
{
    if (minefield[row][col]->isMine) return true;   // Return true if the tile is a mine
    if (minefield[row][col]->isHidden)
    {
        DiscoverTile(minefield, row, col, numDiscovered);
    }
    return false;
}

void DiscoverTile(Tile* minefield[ROWS][COLS], int row, int col, int *numDiscovered)
{
    minefield[row][col]->isHidden = false;
    (*numDiscovered)++;
    if (minefield[row][col]->nearbyMines == 0)
    {
        DiscoverNearbyTiles(minefield, row, col, numDiscovered);
    }
}

int NumNearbyMines(Tile* minefield[ROWS][COLS], int row, int col)
{
    int nearbyMines = 0;
    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            // Boundary check
            if (i == 0  && j == 0) continue;
            if (row + i < 0 || row + i > ROWS - 1) continue;
            if (col + j < 0 || col + j > COLS - 1) continue;

            // Check if mine
            if (minefield[row + i][col + j]->isMine) nearbyMines++;
        }
    }
    return nearbyMines;
}

void DiscoverNearbyTiles(Tile *minefield[ROWS][COLS], int row, int col, int *numDiscovered)
{
    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            // Boundary check
            if (i == 0 && j == 0) continue;
            if (row + i < 0 || row + i > ROWS - 1) continue;
            if (col + j < 0 || col + j > COLS - 1) continue;

            // Check if has been discoverd
            if (!minefield[row + i][col + j]->isHidden) continue;
            // Check if mine
            DiscoverTile(minefield, row + i, col + j, numDiscovered);
        }
    }
}