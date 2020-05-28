#pragma once

#include "Cell.h"

#include <iostream>
#include <cstring>
#include <iomanip>

class RealCell : public Cell
{
private:
	const static char type = 'R';

	const static size_t FLOAT_OUTPUT_PRECISION = 5;
	const static size_t FLOAT_WRITE_PRECISION = 12;

	double real;

public:
	RealCell(const char* string);
	RealCell(double number);

	virtual size_t getContentLength() const;
	virtual double getContentAsRealNumber() const;
	virtual Cell* clone() const;
	virtual char getType() const;

	virtual void print(std::ostream& os = std::cout) const;
	virtual void write(std::ostream& os) const;
};