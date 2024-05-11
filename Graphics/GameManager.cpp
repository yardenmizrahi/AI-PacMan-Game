#include "GameManager.h"
#include <iostream>
#include <math.h>
#include "CollectingCoinsState.h"
#include "EscapingGhostsState.h"
#include "PacmanLoseState.h"
#include "PacmanWonState.h"
#include "Consts.h"
#include <unordered_set>


GameManager::GameManager()
{
	this->currentState = new CollectingCoinsState();
	this->currentState->OnEnter(this);

	this->runPacman = false;
	this->pacman = nullptr;
	this->grays = std::vector<Cell*>();

	this->coinsList = std::vector<Cell*>();
	this->numCoinsCollected = 0;
	this->currentCoinsLeft = NUM_COINS;
	this->IsCollectingCoins = false;

	this->IsEscapingGhosts = false;
	this->ghostsList = std::vector<Cell*>();
	this->ghostCanToMoveList = std::vector<bool>();
	this->ghostStuckList = std::vector<bool>();
	this->ghostFoundPathList = std::vector<bool>();
	this->ghostCellStartPos = std::vector<Cell*>();
	this->ghostPriorityQueues = std::vector<std::priority_queue<Cell*, std::vector<Cell*>, CompareCost>>(NUM_GHOSTS);
}

void GameManager::Update_Maze(int** maze)
{

	for (int i = 1; i < MSZ - 1; i++)
	{
		for (int j = 1; j < MSZ - 1; j++)
		{
			if (maze[i][j] == GRAY || maze[i][j] == PATH || maze[i][j] == BLACK || maze[i][j] == PACMAN_GRAYS)
			{
				maze[i][j] = SPACE;
			}
		}
	}
	for (Cell* cell : coinsList)
	{
		maze[cell->getRow()][cell->getCol()] = COIN;
	}
	for (Cell* ghost : ghostsList)
	{
		maze[ghost->getRow()][ghost->getCol()] = GHOST;
	}
	maze[pacman->getRow()][pacman->getCol()] = PACMAN;
}


void GameManager::Run_Game(int** maze)
{
	if (runPacman)
	{
		if (currentCoinsLeft > 0)
		{
			Ghost_Iteration(pacman, maze);
			Pacman_Iteration(pacman, maze, ghostsList, grays);
		}
		else
		{
			std::cout << "Pacman collected all the coins! Pacman won!!!!!!!!!!!!!!!!!!!!!!!!!\n";
			currentState->Transition(this, new PacmanWonState());
		}
	}
	else
	{
		std::cout << "Pacman collected " << numCoinsCollected << "Game Over!" << "!\n";
	}
}


void GameManager::Pacman_Iteration(Cell* pacman, int** maze, std::vector<Cell*> ghostsList, std::vector<Cell*>& grays)
{
	currentState->Transition(this, new CollectingCoinsState());
	std::vector<double> distances;
	double minDistance = MSZ + 1;

	for (int i = 0; i < NUM_GHOSTS; i++)
	{
		if (!ghostStuckList.at(i))
			distances.push_back(GetDistanceBetweenTwoDots(ghostsList.at(i)->getRow(), ghostsList.at(i)->getCol(), pacman->getRow(), pacman->getCol()));
		else
			distances.push_back((double)MSZ); // The ghost is stuck so there is no need to calculate the distance to it
	}

	int indexClosestGhost = 0;

	for (int i = 0; i < NUM_GHOSTS; i++)
	{
		if (distances[i] < minDistance)
		{
			indexClosestGhost = i;
			minDistance = distances[i];
		}
	}

	if (minDistance <= PROXIMITY)
	{
		RunAwayFromGhosts(pacman, ghostsList, maze);
		Update_Maze(maze);
		currentState->Transition(this, new EscapingGhostsState());
	}
	else
		currentState->Transition(this, new CollectingCoinsState());

	if (IsCollectingCoins)
	{
		// if getting here- there are no ghosts nearby & pacman can chase coins
		// check if pacman found a coin next to him:
		int pacR = pacman->getRow(), pacC = pacman->getCol();
		// check if coin is one step up from pacman
		if (maze[pacR + 1][pacC] == COIN)
		{
			collectCoin(pacR + 1, pacC, maze); // erase coin
			// move pacman Up
			maze[pacR][pacC] = SPACE;
			pacman->setRow(pacR + 1);
			pacman->setCol(pacC);
			maze[pacR + 1][pacC] = PACMAN;
			Update_Maze(maze);
			return;
		}
		// check if coin is down step down from pacman
		else if (maze[pacR - 1][pacC] == COIN)
		{
			collectCoin(pacR - 1, pacC, maze); // erase coin
			// move pacman Down
			maze[pacR][pacC] = SPACE;
			pacman->setRow(pacR - 1);
			pacman->setCol(pacC);
			maze[pacR - 1][pacC] = PACMAN;
			Update_Maze(maze);
			return;
		}
		// check if coin is one step right from pacman
		else if (maze[pacR][pacC + 1] == COIN)
		{
			collectCoin(pacR, pacC + 1, maze); // erase coin
			// move pacman Right
			maze[pacR][pacC] = SPACE;
			pacman->setRow(pacR);
			pacman->setCol(pacC + 1);
			maze[pacR][pacC + 1] = PACMAN;
			Update_Maze(maze);
			return;
		}
		// check if coin is one step left from pacman
		else if (maze[pacR][pacC - 1] == COIN)
		{
			collectCoin(pacR, pacC - 1, maze); // erase coin
			// move pacman Left
			maze[pacR][pacC] = SPACE;
			pacman->setRow(pacR);
			pacman->setCol(pacC - 1);
			maze[pacR][pacC - 1] = PACMAN;
			Update_Maze(maze);
			return;
		}
		// if coins are further than 1 step - start BFS & move one step to first coin pacman found:
		CollectCoins(pacman, maze, grays, &IsCollectingCoins, &runPacman);
	}
}


void GameManager::RestorePacmanPath(Cell* pc, Cell* pacman, int** maze)
{
	double distanceBetweenPacmanToPc = GetDistanceBetweenTwoDots(pc->getRow(), pc->getCol(), pacman->getRow(), pacman->getCol());
	grays.clear();// Clearing irrelavent cells for path
	while (distanceBetweenPacmanToPc > 1) // iterate over path to next coin until next move of pacman
	{
		grays.push_back(pc);//inserting back the relevant cell exceot the closest one to pacman
		pc = pc->getParent();
		distanceBetweenPacmanToPc = GetDistanceBetweenTwoDots(pc->getRow(), pc->getCol(), pacman->getRow(), pacman->getCol());
	}
	// move pacman one step
	maze[pacman->getRow()][pacman->getCol()] = SPACE;
	maze[pc->getRow()][pc->getCol()] = PACMAN;
	pacman->setRow(pc->getRow());
	pacman->setCol(pc->getCol());
	IsCollectingCoins = false; // bool flag of pacman movement set to false
	Update_Maze(maze);
}

void GameManager::CheckNeighbourPacman(int row, int col, Cell* pCurrent, Cell* pacman, int** maze)
{
	if (maze[row][col] == COIN) // if pacman found a coin to path -> move one step forward to it
	{
		RestorePacmanPath(pCurrent, pacman, maze);
		return;
	}
	else if (maze[row][col] == SPACE)
	{
		Cell* pn = new Cell(row, col, pCurrent);
		grays.push_back(pn);
		maze[row][col] = PACMAN_GRAYS;
	}
}

bool GameManager::CheckProximityToGhosts(Cell* pacman, std::vector<Cell*> ghostsList)
{
	// Calculate distances between pacman and ghosts
	double minDistance = std::numeric_limits<double>::max();

	for (Cell* ghost : ghostsList)
	{
		double distance = GetDistanceBetweenTwoDots(ghost->getRow(), ghost->getCol(), pacman->getRow(), pacman->getCol());
		if (distance < minDistance)
		{
			minDistance = distance;
		}
	}

	// Check if pacman is close to any ghosts based on a threshold distance
	return minDistance <= PROXIMITY;
}

void GameManager::RunAwayFromGhosts(Cell* pacman, std::vector<Cell*> ghostsList, int** maze)
{
	int pacRow = pacman->getRow(), pacCol = pacman->getCol();
	std::cout << "RunAwayFromGhosts = " << "[" << pacRow  << "]" <<  "[" << pacCol << "]" << std::endl;
	int ghostRow, ghostCol;

	// Find the closest ghost
	double minDistance = std::numeric_limits<double>::max();
	Cell* closestGhost = nullptr;

	for (Cell* ghost : ghostsList)
	{
		double distance = GetDistanceBetweenTwoDots(ghost->getRow(), ghost->getCol(), pacRow, pacCol);
		std::cout << "distance = " << distance << std::endl;
		if (distance < minDistance)
		{
			minDistance = distance;
			closestGhost = ghost;
		}
	}

	if (closestGhost == nullptr) {
		std::cerr << "Something went wrong - no close ghost found!\n";
		return;
	}

	ghostRow = closestGhost->getRow();
	ghostCol = closestGhost->getCol();

	// check which direction is the best for pacman to run away in one step away
	std::vector<Cell*> cellPointers = {
		new Cell(pacRow + 1,pacCol,nullptr), // UP
		new Cell(pacRow - 1,pacCol,nullptr), // DOWN
		new Cell(pacRow,pacCol + 1,nullptr), // RIGHT
		new Cell(pacRow,pacCol - 1,nullptr) }; // LEFT

	std::vector<double> distances(4, -1); // Initialize distances to -1

	double maxDistance = -1;
	int maxDistIndex = 0;

	for (int i = 0; i < (int)cellPointers.size(); i++)
	{
		// Calculate distance to the ghost for each direction
		double tempDist = GetDistanceBetweenTwoDots(cellPointers[i]->getRow(), cellPointers[i]->getCol(), ghostRow, ghostCol);

		// Check if the cell is valid and not occupied by a wall or ghost
		if (maze[cellPointers[i]->getRow()][cellPointers[i]->getCol()] != WALL &&
			maze[cellPointers[i]->getRow()][cellPointers[i]->getCol()] != GHOST)
		{
			// Store the distance for valid cells
			distances[i] = tempDist;

			// Update the maximum distance and its index
			if (tempDist > maxDistance)
			{
				maxDistance = tempDist;
				maxDistIndex = i;
			}
		}
	}

	// Check if the maximum distance cell is valid
	if (distances[maxDistIndex] != -1)
	{
		// Move Pacman to the cell with maximum distance from the ghost
		collectCoin(cellPointers[maxDistIndex]->getRow(), cellPointers[maxDistIndex]->getCol(), maze);
		maze[pacRow][pacCol] = SPACE; // Updating the current position of pacman to an empty space
		maze[cellPointers[maxDistIndex]->getRow()][cellPointers[maxDistIndex]->getCol()] = PACMAN; // updating the actual position of pacman on board
		pacman->setRow(cellPointers[maxDistIndex]->getRow()); // updating the pacman object row position
		pacman->setCol(cellPointers[maxDistIndex]->getCol());  // updating the pacman object column position
	}

	// Clean up memory allocated for cell pointers
	for (Cell* cellPtr : cellPointers)
	{
		delete cellPtr;
	}
}



void GameManager::CollectCoins(Cell* pacman, int** maze, std::vector<Cell*>& grays, bool* pacmanNeedToCollectCoins, bool* runPacman)
{
	grays.push_back(pacman);
	while (*pacmanNeedToCollectCoins)
	{
		Cell* pcurrent;
		int r, c;
		if (*pacmanNeedToCollectCoins)
		{
			if (grays.empty()) // no solution exists
			{
				std::cout << "Pacman cannot reach for the coins - Game Over!!!!!!!!!!!!!!\n";
				*pacmanNeedToCollectCoins = false;
				*runPacman = false;
				Update_Maze(maze);
				maze[pacman->getRow()][pacman->getCol()] = PACMAN;
				currentState->Transition(this, new PacmanLoseState());
				break;
			}
			else // grays is not empty
			{
				pcurrent = *grays.begin();
				r = pcurrent->getRow();
				c = pcurrent->getCol();
				maze[r][c] = BLACK;
				grays.erase(grays.begin());

				// UP
				if (*pacmanNeedToCollectCoins && (maze[r + 1][c] == SPACE || maze[r + 1][c] == COIN || maze[r + 1][c] == GRAY || maze[r + 1][c] == GHOST && r != pacman->getRow() && c != pacman->getCol()))
					CheckNeighbourPacman(r + 1, c, pcurrent, pacman, maze);
				// DOWN
				if (*pacmanNeedToCollectCoins && (maze[r - 1][c] == SPACE || maze[r - 1][c] == COIN || maze[r - 1][c] == GRAY || maze[r - 1][c] == GHOST && r != pacman->getRow() && c != pacman->getCol()))
					CheckNeighbourPacman(r - 1, c, pcurrent, pacman, maze);
				// LEFT
				if (*pacmanNeedToCollectCoins && (maze[r][c - 1] == SPACE || maze[r][c - 1] == COIN || maze[r][c - 1] == GRAY || maze[r][c - 1] == GHOST && r != pacman->getRow() && c != pacman->getCol()))
					CheckNeighbourPacman(r, c - 1, pcurrent, pacman, maze);
				// RIGHT
				if (*pacmanNeedToCollectCoins && (maze[r][c + 1] == SPACE || maze[r][c + 1] == COIN || maze[r][c + 1] == GRAY || maze[r][c + 1] == GHOST && r != pacman->getRow() && c != pacman->getCol()))
					CheckNeighbourPacman(r, c + 1, pcurrent, pacman, maze);
			}
		}
	}
}


void GameManager::collectCoin(int row, int col, int** maze)
{
	int i = 0;
	for (Cell* coin : coinsList)
	{
		if (coin->getRow() == row && coin->getCol() == col) // if we found coin
		{
			std::cout << "Pacman collected coin on [" << row << "][" << col << "]!\n";
			currentCoinsLeft--; // less coin remain
			numCoinsCollected++; // pacman collected another coin
			maze[row][col] = SPACE;
			coinsList.erase(coinsList.begin() + i); // remove the coin from coins vector
		}
		i++;
	}
}



double GameManager::GetDistanceBetweenTwoDots(int x1, int y1, int x2, int y2)
{
	// Calculate the Euclidean distance between two points
	double distance = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
	return distance;
}

void GameManager::Ghost_Iteration(Cell* pacman, int** maze) {
	for (int indexGhost = 0; indexGhost < NUM_GHOSTS; indexGhost++)
	{
		if (ghostFoundPathList.at(indexGhost)) // if the ghost have path to pacman using A* algorithm
		{ 
			double distance = GetDistanceBetweenTwoDots(ghostsList.at(indexGhost)->getRow(), ghostsList.at(indexGhost)->getCol(), pacman->getRow(), pacman->getCol());
			std::cout << "Ghost " << indexGhost << ": Found path = " << ghostFoundPathList.at(indexGhost) << ", Distance = " << distance << std::endl;
			if (distance > 2.1)
			{
				ghostCanToMoveList.at(indexGhost) = true;
				oneGhostIteration(indexGhost, pacman, maze);
				//break;
			}
			else if (distance <= 2.1) {
				std::cout << "Ghost " << indexGhost << " catch pacman and GAME OVER!!!!!!!!!!!!\n";
				currentState->Transition(this, new PacmanLoseState());
			}
		}
		else
		{
			ghostStuckList.at(indexGhost) = true;
			ghostFoundPathList.at(indexGhost) = false;
		}
	}

}

void GameManager::oneGhostIteration(int indexGhost, Cell* pacman, int** maze)
{
	if (ghostFoundPathList.at(indexGhost)) // if the ghost have path to pacman using A* algorithm
	{
		while (ghostCanToMoveList.at(indexGhost))
		{
			Cell* pcurrent;
			int r, c;
			if (ghostPriorityQueues.at(indexGhost).empty()) // The ghost is blocked
			{
				ghostStuckList.at(indexGhost) = true;// the ghost doesn't have path to packaman
				ghostFoundPathList.at(indexGhost) = false;// the ghost doesn't have path to packaman
				ghostCanToMoveList.at(indexGhost) = false; // Stopping the ghost movement
				Update_Maze(maze);
				return;
			}
			else // grays is not empty
			{
				pcurrent = ghostPriorityQueues.at(indexGhost).top(); // get cell with cheapest cost (distance to start + distance to pacman)
				r = pcurrent->getRow();
				c = pcurrent->getCol();
				ghostPriorityQueues.at(indexGhost).pop();
				// add all white neighbors of pcurrent to grays
				// UP
				if (ghostFoundPathList.at(indexGhost) && maze[r + 1][c] == SPACE || maze[r + 1][c] == PACMAN || maze[r + 1][c] == COIN || maze[r + 1][c] == PACMAN_GRAYS || maze[r + 1][c] == GHOST)
					CheckNeighborForGhost(r + 1, c, pcurrent, ghostsList.at(indexGhost), indexGhost, pacman, maze);
				// DOWN
				if (ghostFoundPathList.at(indexGhost) && maze[r - 1][c] == SPACE || maze[r - 1][c] == PACMAN || maze[r - 1][c] == COIN || maze[r - 1][c] == PACMAN_GRAYS || maze[r - 1][c] == GHOST)
					CheckNeighborForGhost(r - 1, c, pcurrent, ghostsList.at(indexGhost), indexGhost, pacman, maze);
				// LEFT
				if (ghostFoundPathList.at(indexGhost) && maze[r][c - 1] == SPACE || maze[r][c - 1] == PACMAN || maze[r][c - 1] == COIN || maze[r][c - 1] == PACMAN_GRAYS || maze[r][c - 1] == GHOST)
					CheckNeighborForGhost(r, c - 1, pcurrent, ghostsList.at(indexGhost), indexGhost, pacman, maze);
				// RIGHT
				if (ghostFoundPathList.at(indexGhost) && maze[r][c + 1] == SPACE || maze[r][c + 1] == PACMAN || maze[r][c + 1] == COIN || maze[r][c + 1] == PACMAN_GRAYS || maze[r][c + 1] == GHOST)
					CheckNeighborForGhost(r, c + 1, pcurrent, ghostsList.at(indexGhost), indexGhost, pacman, maze);
			}
		}
	}
}

void GameManager::CheckNeighborForGhost(int row, int column, Cell* pcurrent, Cell*& ghost, int indexGhost, Cell* pacman, int** maze)
{
	if (maze[row][column] == PACMAN) // if found root to the pacman -> restore path to WHITE and move the ghost 1 step
	{
		RestorePathForGhost(pcurrent, ghost, indexGhost, maze, pacman); // move one step
		return;
	}
	else // maze[row][column] is WHITE or maze[row][column] is COIN
	{
		double distanceCellFromStartMonster = GetDistanceBetweenTwoDots(row, column, ghostCellStartPos.at(indexGhost)->getRow(), ghostCellStartPos.at(indexGhost)->getCol());
		double distancePackmanFromStartMonster = GetDistanceBetweenTwoDots(row, column, pacman->getRow(), pacman->getCol());
		Cell* pcAStar = new Cell(row, column, distanceCellFromStartMonster + distancePackmanFromStartMonster, pcurrent);
		// paint this neighbor gray
		maze[row][column] = GRAY;
		ghostPriorityQueues.at(indexGhost).push(pcAStar);
	}
}

void GameManager::RestorePathForGhost(Cell* pc, Cell*& ghost, int indexGhost, int** maze, Cell* pacman)
{
	while (pc->getParent() != ghost) // all path return to be SPACE instead of gray
	{
		pc = pc->getParent();
	}
	maze[ghost->getRow()][ghost->getCol()] = SPACE; // remove old location of the ghost
	double distanceToPacMan = GetDistanceBetweenTwoDots(pc->getRow(), pc->getCol(), pacman->getRow(), pacman->getCol());
	ghost = pc;
	ghost->setRow(pc->getRow());
	ghost->setCol(pc->getCol());
	maze[ghost->getRow()][ghost->getCol()] = GHOST;
	Cell* cell;
	while (!ghostPriorityQueues.at(indexGhost).empty())
	{
		cell = ghostPriorityQueues.at(indexGhost).top();
		maze[cell->getRow()][cell->getCol()] = SPACE;
		ghostPriorityQueues.at(indexGhost).pop();
	}
	ghostPriorityQueues.at(indexGhost).push(ghost);
	ghostCanToMoveList.at(indexGhost) = false;
	Update_Maze(maze);
}
