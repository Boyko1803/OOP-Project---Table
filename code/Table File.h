#pragma once

#include "Table.h"

#include <fstream>

class TableFile
{
	const static size_t INITIAL_FILE_READ_LIMIT = 64;

	private:
		Table* currentTable;
		char* address;
		bool openFlag;

		char readCell(std::istream& is, char*& cellContent, const char endOfCell, const char endOfRow, const char fillerSymbol);

	public:
		TableFile();
		~TableFile();

		bool isOpen() const;
		const char* const getAddress() const;
		char writeTableAsFile(const char* address = nullptr, const char endOfCell = ',', const char endOfRow = '\n') const;

		Table& getTable();
		char openNewFile(const char* address, size_t& errorRow, size_t& errorColumn, char** errorContent = nullptr, const char endOfCell = ',', const char endOfRow = '\n', const char fillerSymbol = ' ');
		bool closeCurrentFile();
};