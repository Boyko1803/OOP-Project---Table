#pragma once

#include "Cell.h"
#include "Integer Cell.h"
#include "Real Cell.h"
#include "Text Cell.h"
#include "Formula Cell.h"

#define MAX_CELLS_IN_ROW_LIMIT 4096

class Row
{
	const static size_t INITIAL_CELLS_IN_ROW_LIMIT = 2;

	private:
		Cell** rowCells;
		size_t numberOfCells;
		size_t limitOfCells;
		
		void expandRow();
		void deleteData();
		void copyData(const Row& other);

	public:
		Row();
		Row(const Row& other);
		Row& operator=(const Row& other);
		~Row();

		size_t getNumberOfCells() const;
		bool validCell(const size_t position) const;

		char writeInNextCell(const char* string);
		char writeInCell(const size_t position, const char* string);
		bool restractRow(const size_t size);
		
		const Cell* const& operator[](const size_t position) const;
		Cell*& operator[](const size_t position);
};