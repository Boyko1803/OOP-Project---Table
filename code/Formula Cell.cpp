#include "Formula Cell.h"

void FormulaCell::deleteData()
{
	delete[] formula;
}

void FormulaCell::copyData(const FormulaCell& other)
{
	formula = new char[strlen(other.formula) + 1];
	strcpy(formula, other.formula);
}

FormulaCell::FormulaCell(const char* string)
{
	size_t index = 0;

	formula = new char[strlen(string) + 1];
	while (string[index] != '=' && string[index] != '\0') index++;
	strcpy(formula, string + index);
	if (index < strlen(string))
	{
		index = strlen(string) - index - 1;
		while (formula[index] == ' ') index--;
		formula[index + 1] = '\0';
	}
}

FormulaCell::FormulaCell(const FormulaCell& other)
{
	copyData(other);
}

FormulaCell& FormulaCell::operator=(const FormulaCell& other)
{
	if (this != &other)
	{
		deleteData();
		copyData(other);
	}
	return *this;
}

FormulaCell::~FormulaCell()
{
	deleteData();
}

size_t FormulaCell::getContentLength() const
{
	return strlen(formula);
}

double FormulaCell::getContentAsRealNumber() const
{
	return 0.;
}

Cell* FormulaCell::clone() const
{
	return new FormulaCell(*this);
}

char FormulaCell::getType() const
{
	return type;
}

char* FormulaCell::getFormulaExpression() const
{
	size_t length = strlen(formula);
	char* temp = new char[length + 1];
	strcpy(temp, formula);
	temp[length] = '\0';
	return temp;
}

void FormulaCell::print(std::ostream& os) const
{
	os << std::left << formula;
}

void FormulaCell::write(std::ostream& os) const
{
	os << ' ' << formula << ' ';
}