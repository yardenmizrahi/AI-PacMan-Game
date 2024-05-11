#include "CompareCost.h"

bool CompareCost::operator()(const Cell* cell1, const Cell* cell2) const
{
	return cell1->getCost() < cell2->getCost();
}