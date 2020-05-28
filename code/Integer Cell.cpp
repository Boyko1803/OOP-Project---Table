#include "Integer Cell.h"

IntegerCell::IntegerCell(const char* string)
{
	integer = atoll(string);
}

IntegerCell::IntegerCell(long long int integer_)
{
	integer = integer_;
}

size_t IntegerCell::getContentLength() const
{
	size_t numberOfSymbols = 0;
	long long int integerCopy = integer;
	if (integerCopy < 0) numberOfSymbols++;
	integerCopy = abs(integerCopy);
	do
	{
		numberOfSymbols++;
		integerCopy /= 10;
	} while (integerCopy > 0);

	return numberOfSymbols;
}

double IntegerCell::getContentAsRealNumber() const
{
	return (double)integer;
}

Cell* IntegerCell::clone() const
{
	return new IntegerCell(*this);
}

char IntegerCell::getType() const
{
	return type;
}

void IntegerCell::print(std::ostream& os) const
{
	os << std::right << integer;
}

void IntegerCell::write(std::ostream& os) const
{
	os << ' ' << integer << ' ';
}