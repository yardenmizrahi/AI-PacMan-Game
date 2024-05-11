#include "CollectingCoinsState.h"
#include "EscapingGhostsState.h"
#include "PacmanLoseState.h"
#include "PacmanWonState.h"

void CollectingCoinsState::OnEnter(GameManager* gameManager)
{
	gameManager->setCollectingCoins(true);
	gameManager->setEscapingGhosts(false);
}

void CollectingCoinsState::Transition(GameManager* gameManager, State* newState)
{
	if (dynamic_cast<EscapingGhostsState*>(newState))
	{
		gameManager->setCurrentState(new EscapingGhostsState());
	}
	else if (dynamic_cast<PacmanLoseState*>(newState))
	{
		gameManager->setCurrentState(new PacmanLoseState());
	}
	else if (dynamic_cast<CollectingCoinsState*>(newState))
	{
		gameManager->setCurrentState(new CollectingCoinsState());
	}
	else
	{
		gameManager->setCurrentState(new PacmanWonState());
	}
	gameManager->getCurrentState()->OnEnter(gameManager);
}

void CollectingCoinsState::OnExit(GameManager* gameManager)
{
	gameManager->setCollectingCoins(false);
}
