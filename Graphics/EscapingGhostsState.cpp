#include "EscapingGhostsState.h"
#include "CollectingCoinsState.h"
#include "PacmanLoseState.h"

void EscapingGhostsState::OnEnter(GameManager* gameManager)
{
	gameManager->setCollectingCoins(false);
	gameManager->setEscapingGhosts(true);
}

void EscapingGhostsState::Transition(GameManager* gameManager, State* newState)
{
	OnExit(gameManager);
	if (dynamic_cast<EscapingGhostsState*>(newState))
	{
		gameManager->setCurrentState(new EscapingGhostsState());
	}
	else if (dynamic_cast<PacmanLoseState*>(newState))
	{
		gameManager->setCurrentState(new PacmanLoseState());
	}
	else
	{
		gameManager->setCurrentState(new CollectingCoinsState());
	}
	gameManager->getCurrentState()->OnEnter(gameManager);
}

void EscapingGhostsState::OnExit(GameManager* gameManager)
{
	gameManager->setCollectingCoins(true);
	gameManager->setEscapingGhosts(false);
}

