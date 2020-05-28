#include "Cell.h"
#include "Integer Cell.h"
#include "Real Cell.h"
#include "Text Cell.h"
#include "Formula Cell.h"

Cell::~Cell() {}

size_t Cell::getContentLength() const
{
	return 1;
}

double Cell::getContentAsRealNumber() const
{
	return 0.;
}

Cell* Cell::clone() const
{
	return new Cell;
}

char Cell::getType() const
{
	return type;
}

void Cell::print(std::ostream& os) const
{
	os << ' ';
}

void Cell::write(std::ostream& os) const
{
	os << ' ';
}

char Cell::checkType(const char* originalString)
{
	int index = 0;
	int length = strlen(originalString);

	while (originalString[index] == ' ' && originalString[index] != '\0') index++;
	if (originalString[index] == '\0') return 'E';
	if (originalString[index] == '=') return 'F';
	if (originalString[index] == '"')
	{
		index++;
		while (index < length)
		{
			if (originalString[index] == '\\' && (originalString[index + 1] == '\\' || originalString[index + 1] == '"')) index += 2;
			else if (originalString[index] == '"') break;
			else index++;
		}
		if (index == length) return 'Q';

		index++;
		if (originalString[index] != ' ' && originalString[index] != '\0') return 'U';
		while (index < length)
		{
			if (originalString[index] != ' ') return 'C';
			index++;
		}

		return 'T';
	}
	if (originalString[index] == '+' || originalString[index] == '-' || originalString[index] == '.' || (originalString[index] <= '9' && originalString[index] >= '0'))
	{
		bool sign = 1, comma = 0, space = 0;
		if (originalString[index] == '-') sign = 0;
		if (originalString[index] == '.') comma = 1;
		if (originalString[index] == '+' || originalString[index] == '-' || originalString[index] == '.') index++;
		while (originalString[index] == '.' || (originalString[index] <= '9' && originalString[index] >= '0'))
		{
			if (originalString[index] == '.')
			{
				if (comma) return 'U';
				else comma = 1;
			}
			index++;
		}
		while (index < length)
		{
			if (originalString[index] != ' ')
			{
				if (space) return 'C';
				return 'U';
			}
			index++;
			space = 1;
		}

		if (comma) return 'R';
		return 'I';
	}

	return 'U';
}

bool Cell::validCellType(const char type)
{
	if (type == 'E' || type == 'I' || type == 'R' || type == 'T' || type == 'F') return 1;
	return 0;
}

bool isZero(const double number)
{
	return (fabs(number) <= DOUBLE_DEVIATION_LIMIT);
}

Cell* Cell::createCell(const char* string, char type)
{
	if (type == '\0') type = checkType(string);

	switch (type)
	{
		case 'E': return new Cell;
		case 'I': return new IntegerCell(string);
		case 'R': return new RealCell(string);
		case 'T': return new TextCell(string);
		case 'F': return new FormulaCell(string);
		default: return new Cell;
	}
}