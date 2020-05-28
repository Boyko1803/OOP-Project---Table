#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstring>
#include <iomanip>
#include <cmath>

#define DOUBLE_DEVIATION_LIMIT 1e-12

class Cell
{
	private:
		const static char type = 'E';

	public:
		virtual ~Cell();

		virtual size_t getContentLength() const;
		virtual double getContentAsRealNumber() const;
		virtual Cell* clone() const;
		virtual char getType() const;

		virtual void print(std::ostream& os = std::cout) const;
		virtual void write(std::ostream& os) const;

		static char checkType(const char* string);
		static bool validCellType(const char type);
		static Cell* createCell(const char* string, char type = '\0');
};

bool isZero(const double number);