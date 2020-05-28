#include "Table File.h"

char TableFile::readCell(std::istream& is, char*& cellContent, const char endOfCell, const char endOfRow, const char fillerSymbol)
{
	size_t size = 0, limit = INITIAL_FILE_READ_LIMIT;
	char* temp = new char[limit + 1];
	char character = '\0';
	size_t quotation = 0;
	bool ignoreNext = 0;

	while (!is.eof())
	{
		is.get(character);
		if (character == endOfRow || (character == endOfCell && quotation != 1)) break;

		if (ignoreNext == 1) ignoreNext = 0;
		else if (quotation == 1)
		{
			if (character == '\\') ignoreNext = 1;
			if (character == '"') quotation = 2;
		}
		if (quotation == 0)
		{
			if (character == '"') quotation = 1;
			else if (character != fillerSymbol) quotation = 2;
		}

		if (size >= limit)
		{
			limit *= 2;
			char* newTemp = new char[limit + 1];
			strncpy(newTemp, temp, size);
			delete[] temp;
			temp = newTemp;
		}
		temp[size] = character;
		size++;
	}
	if (is.eof()) size--;
	temp[size] = '\0';
	cellContent = temp;

	if (character == endOfCell) return 0;
	if (character == endOfRow) return 1;
	return 2;
}

TableFile::TableFile() :
	currentTable(nullptr),
	address(nullptr),
	openFlag(0)
{}

TableFile::~TableFile()
{
	if (openFlag)
	{
		delete currentTable;
		delete[] address;
	}
}

bool TableFile::isOpen() const
{
	return openFlag;
}

const char* const TableFile::getAddress() const
{
	return address;
}

char TableFile::writeTableAsFile(const char* fileAddress, const char endOfCell, const char endOfRow) const
{
	if (!openFlag) return 'N';
	if (fileAddress == nullptr) fileAddress = address;

	std::ofstream outputFile(fileAddress, std::ios::out | std::ios::trunc);
	if (!outputFile) return 'L';

	size_t numberOfColumns, numberOfRows = currentTable->getNumberOfRows();
	for (size_t row = 0; row < numberOfRows; row++)
	{
		numberOfColumns = (*currentTable)[row].getNumberOfCells();

		if (numberOfColumns > 0) (*currentTable)[row][0]->write(outputFile);

		for (size_t column = 1; column < numberOfColumns; column++)
		{
			outputFile << endOfCell;
			(*currentTable)[row][column]->write(outputFile);
		}
		if (row < numberOfRows - 1) outputFile << endOfRow;
	}

	outputFile.close();
	return 'S';
}

Table& TableFile::getTable()
{
	return *currentTable;
}

char TableFile::openNewFile(const char* address_, size_t& errorRow, size_t& errorColumn, char** errorContent, const char endOfCell, const char endOfRow, const char fillerSymbol)
{
	if (openFlag) return 'O';
	std::ifstream inputFile(address_, std::ios::in);
	if (!inputFile) return 'L';

	openFlag = 1;
	currentTable = new Table;
	address = new char[strlen(address_) + 1];
	strcpy(address, address_);
	
	char* cellContent = nullptr;
	char cellState = 'E';
	size_t inputState = 2;

	currentTable->nextRow();
	while (!inputFile.eof())
	{
		inputState = readCell(inputFile, cellContent, endOfCell, endOfRow, fillerSymbol);

		cellState = currentTable->writeInNextCell(cellContent);
		if (!Cell::validCellType(cellState))
		{
			inputFile.close();
			errorRow = currentTable->getNumberOfRows();
			errorColumn = (*currentTable)[errorRow - 1].getNumberOfCells();
			if (errorContent != nullptr)
			{
				*errorContent = new char[strlen(cellContent) + 1];
				strcpy(*errorContent, cellContent);
				delete[] cellContent;
			}
			return cellState;
		}

		if (inputState == 1) currentTable->nextRow();

		delete[] cellContent;
	}

	inputFile.close();

	return 'S';
}

bool TableFile::closeCurrentFile()
{
	if (!openFlag) return 0;
	openFlag = 0;
	if (currentTable != nullptr) delete currentTable;
	if (address != nullptr) delete[] address;
	currentTable = nullptr;
	address = nullptr;
	return 1;
}