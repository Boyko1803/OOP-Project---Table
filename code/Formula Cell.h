#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "Cell.h"

#include <iostream>
#include <cstring>
#include <iomanip>

#define FORMULA_ERROR_MESSAGE "ERROR"

class FormulaCell : public Cell
{
private:
	const static char type = 'F';

	char* formula;

	void deleteData();
	void copyData(const FormulaCell& other);

public:
	FormulaCell(const char* string);
	FormulaCell(const FormulaCell& other);
	FormulaCell& operator=(const FormulaCell& other);
	~FormulaCell();

	virtual size_t getContentLength() const;
	virtual double getContentAsRealNumber() const;
	virtual Cell* clone() const;
	virtual char getType() const;
	char* getFormulaExpression() const;

	virtual void print(std::ostream& os = std::cout) const;
	virtual void write(std::ostream& os) const;
};