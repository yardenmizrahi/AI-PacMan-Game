#include "PacmanLoseState.h"


void PacmanLoseState::OnEnter(GameManager* gameManager)
{
	gameManager->setCollectingCoins(false);
	gameManager->setEscapingGhosts(false);
	gameManager->setRunPacman(false);
}

void PacmanLoseState::Transition(GameManager* gameManager, State* newState)
{
	// BYE
}

void PacmanLoseState::OnExit(GameManager* gameManager)
{
	// BYE
}
