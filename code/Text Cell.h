#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "Cell.h"

#include <iostream>
#include <cstring>
#include <iomanip>

class TextCell : public Cell
{
private:
	const static char type = 'T';

	char* text;

	void deleteData();
	void copyData(const TextCell& other);

public:
	TextCell(const char* string);
	TextCell(const TextCell& other);
	TextCell& operator=(const TextCell& other);
	~TextCell();

	virtual size_t getContentLength() const;
	virtual double getContentAsRealNumber() const;
	virtual Cell* clone() const;
	virtual char getType() const;

	virtual void print(std::ostream& os = std::cout) const;
	virtual void write(std::ostream& os) const;
};