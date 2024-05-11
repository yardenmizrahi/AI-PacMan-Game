#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "glut.h"
#include <queue>
#include "Cell.h"
#include <iostream>
#include "Maze.h"
#include "GameManager.h"
#include <time.h>
#include <thread>


using namespace std;

// Constructor
Maze::Maze() {
	this->maze = new int* [MSZ];
	for (int i = 0; i < MSZ; ++i) {
		this->maze[i] = new int[MSZ];
	}

	// Initialize maze elements
	for (int i = 0; i < MSZ; ++i) {
		for (int j = 0; j < MSZ; ++j) {
			this->maze[i][j] = 0;
		}
	}
}

// Method to initialize the maze layout
void Maze::init(GameManager* gameManager)
{
	glClearColor(0.5, 0.5, 0.5, 0);// color of window background
	glOrtho(0, MSZ, 0, MSZ, -1, 1); // set the coordinates system

	srand(time(0));

	InitMaze(gameManager);
}

void Maze::InitMaze(GameManager* gameManager)
{
	int i, j;

	for (i = 0; i < MSZ; i++)
	{
		maze[0][i] = WALL;
		maze[MSZ - 1][i] = WALL;
		maze[i][0] = WALL;
		maze[i][MSZ - 1] = WALL;
	}

	for (i = 1; i < MSZ - 1; i++)
		for (j = 1; j < MSZ - 1; j++)
		{
			if (i % 2 == 1) // mostly spaces
			{
				if (rand() % 10 < 2) // WALL
					maze[i][j] = WALL;
				else maze[i][j] = SPACE;
			}
			else // even lines
			{
				if (rand() % 10 < 4) // space
					maze[i][j] = SPACE;
				else
					maze[i][j] = WALL;
			}
		}
	gameManager->setPacman(generateCells(maze));
	int pacRow = gameManager->getPacman()->getRow();
	int pacCol = gameManager->getPacman()->getCol();
	maze[pacRow][pacCol] = PACMAN;


	for (int i = 0; i < NUM_GHOSTS; i++)
	{
		Cell* ghost = generateCells(maze);
		int x = ghost->getRow();
		int y = ghost->getCol();
		maze[x][y] = GHOST;
		gameManager->setGhostsList(ghost);
		std::cout << "ghost list: " << "[" << gameManager->getGhostsList().at(i)->getRow() << "]" << "[" << gameManager->getGhostsList().at(i)->getCol() << "]" << std::endl;

		gameManager->getGhostCellStartPos().push_back(new Cell(x, y, nullptr));
		gameManager->getGhostPriorityQueues().push_back(priority_queue<Cell*, vector<Cell*>, CompareCost>());// PriorityQueues of each ghost
		gameManager->getGhostPriorityQueues().at(i).push(ghost);
		gameManager->getGhostCanToMoveList().push_back(true);// Flag list to each ghost for movement
		gameManager->getGhostStuckList().push_back(false);// Flag list to each ghost for checking if the ghost is stuck
		gameManager->getGhostFoundPathList().push_back(true);// Flag list to each ghost for checking id the ghost is stuck
	}

	for (int i = 0; i < NUM_COINS; i++)
	{
		Cell* coin = generateCells(maze);
		int x = coin->getRow();
		int y = coin->getCol();
		maze[x][y] = COIN;
		gameManager->setCoinsList(coin);
		std::cout << "coins list: " << "["  << gameManager->getCoinsList().at(i)->getRow() << "]"  << "[" << gameManager->getCoinsList().at(i)->getCol() << "]" << std::endl;
	}

}

Cell* Maze::generateCells(int** maze)
{
	int x = rand() % MSZ;
	int y = rand() % MSZ;
	while (x == 0 || x == MSZ - 1)
		x = rand() % MSZ;
	while (y == 0 || y == MSZ - 1)
		y = rand() % MSZ;
	while (maze[x][y] != SPACE)
	{
		x = rand() % MSZ;
		y = rand() % MSZ;
	}
	return new Cell(x, y, nullptr);
}

// Method to draw the maze
void Maze::drawMaze() {
	int i, j;

	for (i = 0; i < MSZ; i++)
		for (j = 0; j < MSZ; j++)
		{
			switch (maze[i][j]) // set convinient color of a cell
			{
			case SPACE:
				glColor3d(1, 1, 1); // white
				break;
			case WALL:
				glColor3d(0, 0, 0); // black
				break;
			case PACMAN:
				glColor3d(0.5, 0.5, 1); // blue
				break;
			case GHOST:
				glColor3d(1, 0, 0); // red
				break;
			case COIN:
				glColor3d(1, 1, 0); // yellow
				break;
			case PATH:
				glColor3d(0.8, 1, 0.8); // green
				break;
			case GRAY:
				glColor3d(0.5, 1, 0.5); // gray
				break;
			}// switch

			glBegin(GL_POLYGON);
			glVertex2d(j, i);
			glVertex2d(j, i + 1);
			glVertex2d(j + 1, i + 1);
			glVertex2d(j + 1, i);
			glEnd();
		}
}

// Method to check if a position is valid within the maze boundaries
bool Maze::isValidPosition(int x, int y) {
	// Check if the position (x, y) is within the maze boundaries
	return x >= 0 && x < MSZ&& y >= 0 && y < MSZ&& maze[x][y] == SPACE;
}

void Maze::display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	drawMaze();

	glutSwapBuffers(); // show all
}

void Maze::idle(GameManager* gameManager) const
{

	if (gameManager->getIsRunPacman())
		gameManager->Run_Game(maze);
	this_thread::sleep_for(chrono::milliseconds(250)); // Adjust the delay time as needed

	glutPostRedisplay(); // indirect call to display
}

void Maze::menu(int choice, GameManager* gameManager)
{
	switch (choice)
	{
	case 1: // start packman
		gameManager->setRunPacman(true);
		//gameManager->Run_Game(maze);
		break;
	}
}
