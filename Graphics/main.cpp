
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "glut.h"
#include <queue>
#include <vector>
#include "Cell.h"
#include <iostream>
#include "Maze.h"
#include "GameManager.h"

using namespace std;

Maze* maze;
GameManager* gameManager;

// drawings are here
void display()
{
	maze->display();
}

void idle() 
{

	maze->idle(gameManager);
}

void menu(int choice)
{
	maze->menu(choice, gameManager);
}

void main(int argc, char* argv[]) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(900, 900);
	glutInitWindowPosition(400, 100);
	glutCreateWindow("Pacman Game");

	maze = new Maze();
	gameManager = new GameManager();

	glutDisplayFunc(display); // refresh function
	glutIdleFunc(idle);  // updates function

	// menu
	glutCreateMenu(menu);
	glutAddMenuEntry("run Pacman game", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	maze->init(gameManager);

	glutMainLoop();

	delete maze;
	delete gameManager;
}