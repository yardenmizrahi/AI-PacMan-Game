#ifndef COMPARECOST_H
#define COMPARECOST_H
#include "Cell.h"

// Define a functor to compare cells based on their costs
class CompareCost 
{
public:
    bool operator()(const Cell* cell1, const Cell* cell2) const;
};

#endif // COMPARECOST_H
