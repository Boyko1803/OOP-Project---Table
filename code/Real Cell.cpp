#include "Real Cell.h"

RealCell::RealCell(const char* string)
{
	real = atof(string);
}

RealCell::RealCell(double real_)
{
	real = real_;
}

size_t RealCell::getContentLength() const
{
	double realCopy = real;
	size_t numberOfSymbols = 0;

	if (isZero(realCopy)) return 1;
	numberOfSymbols = FLOAT_OUTPUT_PRECISION + 1;
	if (realCopy < 0) numberOfSymbols++;
	realCopy = fabs(realCopy);
	if (realCopy > pow(10, FLOAT_OUTPUT_PRECISION) || realCopy < 1/pow(10, 4)) numberOfSymbols += 4;
	else while (realCopy < .1)
	{
		realCopy *= 10;
		numberOfSymbols++;
	}

	return numberOfSymbols;
}

double RealCell::getContentAsRealNumber() const
{
	return real;
}

Cell* RealCell::clone() const
{
	return new RealCell(*this);
}

char RealCell::getType() const
{
	return type;
}

void RealCell::print(std::ostream& os) const
{
	os << std::right << std::setprecision(FLOAT_OUTPUT_PRECISION) << real;
}

void RealCell::write(std::ostream& os) const
{
	os << ' ' << std::fixed << std::setprecision(FLOAT_WRITE_PRECISION) << real << ' ';
}