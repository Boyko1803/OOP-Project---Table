#pragma once

#include "Cell.h"

#include <iostream>
#include <cstring>
#include <iomanip>

class IntegerCell : public Cell
{
private:
	const static char type = 'I';

	long long int integer;

public:
	IntegerCell(const char* string);
	IntegerCell(long long int number);

	virtual size_t getContentLength() const;
	virtual double getContentAsRealNumber() const;
	virtual Cell* clone() const;
	virtual char getType() const;

	virtual void print(std::ostream& os = std::cout) const;
	virtual void write(std::ostream& os) const;
};