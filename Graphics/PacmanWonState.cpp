#include "PacmanWonState.h"


void PacmanWonState::OnEnter(GameManager* gameManager)
{
	gameManager->setRunPacman(false);
}

void PacmanWonState::Transition(GameManager* gameManager, State* newState)
{
	// BYE
}

void PacmanWonState::OnExit(GameManager* gameManager)
{
	// BYE
}
