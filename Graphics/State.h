#pragma once
#include "GameManager.h"
class GameManager;

// pure abstract class (in Java it is interface)
class State
{
public:
	virtual void OnEnter(GameManager* gameManager) = 0;
	virtual void Transition(GameManager* gameManager, State* newState) = 0; // to another state
	virtual void OnExit(GameManager* gameManager) = 0;
};

