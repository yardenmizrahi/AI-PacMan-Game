#pragma once
class Cell
{
private:
	int row;
	int col;
	Cell* parent;
	double cost; // Add a cost attribute

public:
	Cell();
	Cell(int r, int c, Cell* p);
	Cell(int r, int c, double cost, Cell* p);
	int getRow() { return row; }
	int getCol() { return col; }
	Cell* getParent() { return parent; }
	void setRow(int newRow) { row = newRow; }
	void setCol(int newCol) { col = newCol; }
	void setParent(Cell* newParent) { parent = newParent; }
	double getCost() const { return cost; } // Getter for cost
	void setCost(double newCost) { cost = newCost; } // Setter for cost
};

