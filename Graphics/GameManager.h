#pragma once
#include "Cell.h"
#include "State.h"
#include "Maze.h"
#include <vector>
#include <queue>
#include "Consts.h"
#include <vector>
#include "CompareCost.h"

class State;
class Cell;

class GameManager {
private:
	State* currentState;
	bool runPacman;
	Cell* pacman;
	std::vector<Cell*> grays;

	int numCoinsCollected;
	int currentCoinsLeft;
	std::vector<Cell*> coinsList;
	bool IsCollectingCoins;

	bool IsEscapingGhosts;
	std::vector<Cell*> ghostsList;
	std::vector<std::priority_queue<Cell*, std::vector<Cell*>, CompareCost>> ghostPriorityQueues;
	std::vector<bool> ghostCanToMoveList;
	std::vector<bool> ghostStuckList;
	std::vector<bool> ghostFoundPathList;
	std::vector<Cell*> ghostCellStartPos;



public:
	GameManager(); // Default constructor declaration
	void Run_Game(int** maze);
	void Update_Maze(int** maze);

	void Pacman_Iteration(Cell* pacman, int** maze, std::vector<Cell*> ghostsList, std::vector<Cell*>& grays);
	bool CheckProximityToGhosts(Cell* pacman, std::vector<Cell*> ghostsList);
	void RunAwayFromGhosts(Cell* pacman, std::vector<Cell*> ghostsList, int** maze);
	void CollectCoins(Cell* pacman, int** maze, std::vector<Cell*>& grays, bool* pacmanNeedToCollectCoins, bool* runPacman);
	void collectCoin(int row, int col, int** maze);
	double GetDistanceBetweenTwoDots(int x1, int y1, int x2, int y2);

	void Ghost_Iteration(Cell* pacamn, int** maze);
	void oneGhostIteration(int indexGhost, Cell* pacman, int** maze);
	void CheckNeighborForGhost(int row, int column, Cell* pcurrent, Cell*& ghost, int indexGhost, Cell* pacman, int** maze);
	void RestorePathForGhost(Cell* pc, Cell*& ghost, int indexGhost, int** maze, Cell* pacman);

	void CheckNeighbourPacman(int row, int col, Cell* pCurrent, Cell* pacman, int** maze);
	void RestorePacmanPath(Cell* pc, Cell* pacman, int** maze);
	State* getCurrentState() { return currentState; }
	Cell* getPacman() { return pacman; }
	bool getIsRunPacman() { return runPacman; }

	std::vector<Cell*> getGhostsList() { return ghostsList; }
	std::vector<Cell*> getCoinsList() { return coinsList; }
	std::vector<bool>& getGhostCanToMoveList() { return ghostCanToMoveList; }
	std::vector<bool>& getGhostStuckList() { return ghostStuckList; }
	std::vector<bool>& getGhostFoundPathList() { return ghostFoundPathList; }
	std::vector<Cell*>& getGhostCellStartPos() { return ghostCellStartPos; }
	std::vector<std::priority_queue<Cell*, std::vector<Cell*>, CompareCost>>& getGhostPriorityQueues() { return ghostPriorityQueues; }

	void setGhostsList(Cell* ghost) { ghostsList.insert(ghostsList.end(), ghost); }
	void setCoinsList(Cell* coin) { coinsList.insert(coinsList.end(), coin); }
	void setRunPacman(bool b) { runPacman = b; }
	void setPacman(Cell* p) { pacman = p; }
	void setCollectingCoins(bool b) { IsCollectingCoins = b; }
	void setEscapingGhosts(bool b) { IsEscapingGhosts = b; }
	void setCurrentState(State* state) { currentState = state; }
};
