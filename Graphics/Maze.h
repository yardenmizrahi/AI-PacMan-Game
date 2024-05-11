#ifndef MAZE_H
#define MAZE_H
#include <vector>
#include <iostream> 
#include <queue>
#include "Cell.h"
#include "Consts.h"

class GameManager;

class Maze {
private:
    int** maze; // space


public:
    Maze();
    void init(GameManager* gameManager);
    void InitMaze(GameManager* gameManager);
    void drawMaze();
    bool isValidPosition(int x, int y);
    void display();
    void idle(GameManager* gameManager) const;
    void menu(int choice, GameManager* gameManager);
    Cell* generateCells(int** maze);

};
#endif // MAZE_H

