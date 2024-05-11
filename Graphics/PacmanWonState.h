#pragma once
#include "State.h"

class PacmanWonState : public State
{
public:
	void OnEnter(GameManager* gameManager) override;
	void Transition(GameManager* gameManager, State* newState) override;
	void OnExit(GameManager* gameManager) override;

};

