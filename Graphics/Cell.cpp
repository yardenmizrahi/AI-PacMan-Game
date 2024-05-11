#include "Cell.h"

Cell::Cell()
{
	parent = nullptr;
}

Cell::Cell(int r, int c, Cell* p)
{
	row = r;
	col = c;
	parent = p;
}
Cell::Cell(int r, int c, double co, Cell* p)
{
	row = r;
	col = c;
	cost = co;
	parent = p;
}