#include "Row.h"

void Row::expandRow()
{
	Cell** temp = new Cell*[2 * limitOfCells];
	for (size_t i = 0; i < limitOfCells; i++) temp[i] = rowCells[i];
	for (size_t i = limitOfCells; i < 2 * limitOfCells; i++) temp[i] = new Cell;
	delete[] rowCells;
	rowCells = temp;
	limitOfCells *= 2;
}

void Row::deleteData()
{
	for (size_t i = 0; i < limitOfCells; i++) delete rowCells[i];
	delete[] rowCells;
}

void Row::copyData(const Row& other)
{
	limitOfCells = other.limitOfCells;
	numberOfCells = other.numberOfCells;
	rowCells = new Cell*[limitOfCells];
	for (size_t i = 0; i < limitOfCells; i++) rowCells[i] = other.rowCells[i]->clone();
}

Row::Row()
{
	limitOfCells = INITIAL_CELLS_IN_ROW_LIMIT;
	rowCells = new Cell*[limitOfCells];
	for (size_t i = 0; i < limitOfCells; i++) rowCells[i] = new Cell;
	numberOfCells = 0;
}

Row::Row(const Row& other)
{
	copyData(other);
}

Row& Row::operator=(const Row& other)
{
	if (this != &other)
	{
		deleteData();
		copyData(other);
	}
	return *this;
}

Row::~Row()
{
	deleteData();
}

size_t Row::getNumberOfCells() const
{
	return numberOfCells;
}

bool Row::validCell(const size_t position) const
{
	if (position >= 0 && position < numberOfCells) return 1;
	return 0;
}

char Row::writeInNextCell(const char* string)
{
	return writeInCell(numberOfCells, string);
}

char Row::writeInCell(const size_t position, const char* string)
{
	char type;
	if (position >= 0 && position < MAX_CELLS_IN_ROW_LIMIT)
	{
		while (position >= limitOfCells) expandRow();

		type = Cell::checkType(string);
		delete rowCells[position];
		rowCells[position] = Cell::createCell(string, type);
		if (position >= numberOfCells) numberOfCells = position + 1;
		return type;
	}
	return 'P';
}

bool Row::restractRow(const size_t size)
{
	if (size >= numberOfCells) return 0;
	numberOfCells = size;
	return 1;
}

const Cell* const& Row::operator[](const size_t position) const
{
	return rowCells[position];
}

Cell*& Row::operator[](const size_t position)
{
	return rowCells[position];
}