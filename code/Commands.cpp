#include "Commands.h"

void Commands::expandString(char*& string, size_t& limit)
{
	char* newString = new char[2 * limit + 1];
	strncpy(newString, string, limit);
	limit *= 2;
	delete[] string;
	string = newString;
}

char* Commands::inputContent(std::istream& is, const char endSymbol, const char fillerSymbol)
{
	size_t size = 0, limit = INITIAL_COMMAND_READ_LIMIT;
	char* temp = new char[limit + 1];
	char character;

	character = is.get();
	while (character != endSymbol && character == fillerSymbol) is.get(character);
	while (character != endSymbol)
	{
		if (size == limit) expandString(temp, limit);
		temp[size] = character;
		size++;
		is.get(character);
	}
	while (size > 0 && temp[size - 1] == fillerSymbol) size--;
	temp[size] = '\0';

	return temp;
}

char* Commands::inputCommand(std::istream& is, bool& endOfLine)
{
	size_t size = 0, limit = INITIAL_COMMAND_READ_LIMIT;
	char* temp = new char[limit + 1];
	char character;

	character = is.get();
	while (character != ' ' && character != '\n')
	{
		if (size == limit) expandString(temp, limit);
		temp[size] = character;
		size++;
		is.get(character);
	}
	temp[size] = '\0';

	if (character == '\n') endOfLine = 1;
	else endOfLine = 0;

	return temp;
}

int Commands::inputNonnegativeInteger(std::istream& is, bool& validInput, bool& endOfLine, const char endSymbol, const char fillerSymbol)
{
	size_t size = 0, limit = INITIAL_COMMAND_READ_LIMIT, result;
	char temp[INTEGER_DIGITS_LIMIT + 1];
	char character;

	character = is.get();
	while (character != endSymbol && character == fillerSymbol) is.get(character);
	if (character >= '0' && character <= '9')
	{
		while (character >= '0' && character <= '9')
		{
			if (size < INTEGER_DIGITS_LIMIT)
			{
				temp[size] = character;
				size++;
			}
			is.get(character);
		}
		temp[size] = '\0';
		if (character == fillerSymbol)
		{
			validInput = 1;
			endOfLine = 0;
		}
		else if (character == endSymbol)
		{
			validInput = 1;
			endOfLine = 1;
		}
		else
		{
			validInput = 0;
			endOfLine = 0;
		}

		if (size == INTEGER_DIGITS_LIMIT) validInput = 0;

		result = atoi(temp);
		return result;
	}
	else
	{
		endOfLine = (character == '\n');
		validInput = 0;
		return 0;
	}
}

bool Commands::detectExtraCharacters(std::istream& is, const char endSymbol, const char fillerSymbol)
{
	char character;

	is.get(character);
	while (character == fillerSymbol) is.get(character);
	if (character == endSymbol) return 0;
	while (is.get() != endSymbol);
	return 1;
}

void Commands::outputContent(std::ostream& os, const char* content, const char fillerSymbol)
{
	int index = 0;

	while (content[index] != '\0' && content[index] == fillerSymbol) index++;

	os << content + index << std::endl;
}

void Commands::help()
{
	std::cout << "The following commands are supported:" << std::endl;
	std::cout << std::left << std::setw(HELP_OUTPUT_WIDTH) << "open <file>" << "opens <file>" << std::endl;
	std::cout << std::left << std::setw(HELP_OUTPUT_WIDTH) << "close" << "closes the currently opened file" << std::endl;
	std::cout << std::left << std::setw(HELP_OUTPUT_WIDTH) << "save" << "saves the currently opened file" << std::endl;
	std::cout << std::left << std::setw(HELP_OUTPUT_WIDTH) << "saveas <file>" << "saves the currently opened file in <file>" << std::endl;
	std::cout << std::left << std::setw(HELP_OUTPUT_WIDTH) << "edit <row> <column> <content>" << "overwrites the cell in row <row> and column <column> with <content>" << std::endl;
	std::cout << std::left << std::setw(HELP_OUTPUT_WIDTH) << "print" << "prints the table" << std::endl;
	std::cout << std::left << std::setw(HELP_OUTPUT_WIDTH) << "undo" << "undoes the last change to the table" << std::endl;
	std::cout << std::left << std::setw(HELP_OUTPUT_WIDTH) << "undoall" << "undoes all changes to the table" << std::endl;
	std::cout << std::left << std::setw(HELP_OUTPUT_WIDTH) << "help" << "prints this information" << std::endl;
	std::cout << std::left << std::setw(HELP_OUTPUT_WIDTH) << "exit" << "exits the program" << std::endl;
}

void Commands::openFile(TableFile& file)
{
	char* address = inputContent(std::cin);

	char** errorContent = new char*;
	*errorContent = nullptr;
	size_t errorRow, errorColumn;
	char state = file.openNewFile(address, errorRow, errorColumn, errorContent);

	switch (state)
	{
	case 'O':
		std::cout << "The current file should be closed before a new one can be opened!" << std::endl;
		break;
	case 'L':
		std::cout << "The file could not be found! Check if you entered a correct address." << std::endl;
		break;
	case 'Q':
		std::cout << "A quotation mark is probably missing on row " << errorRow << ", column " << errorColumn << ":" << std::endl;
		outputContent(std::cout, *errorContent);
		break;
	case 'C':
		std::cout << "A comma is probably missing on row " << errorRow << ", column " << errorColumn << ":" << std::endl;
		outputContent(std::cout, *errorContent);
		break;
	case 'U':
		std::cout << "There is an unknown type on row " << errorRow << ", column " << errorColumn << ":" << std::endl;
		outputContent(std::cout, *errorContent);
		break;
	case 'P':
		std::cout << "The table is too large!" << std::endl;
		std::cout << "The number of rows should be between " << 1 << " and " << MAX_ROWS_IN_TABLE_LIMIT << std::endl;
		std::cout << "The number of columns should be between " << 1 << " and " << MAX_CELLS_IN_ROW_LIMIT << std::endl;
		break;
	case 'S':
		std::cout << "File successfully opened!" << std::endl;
		break;
	default:
		std::cout << "An unknown error has occurred! " << std::endl;
	}

	if (state != 'S' && state != 'O') file.closeCurrentFile();
	if (*errorContent != nullptr) delete[] *errorContent;
	delete errorContent;
	delete[] address;
}

void Commands::closeFile(TableFile& file)
{
	if (file.closeCurrentFile()) std::cout << "The file was successfully closed!" << std::endl;
	else std::cout << "No file is currently opened!" << std::endl;
}

void Commands::saveAsFile(TableFile& file)
{
	char* address = inputContent(std::cin);

	char state = file.writeTableAsFile(address);

	switch (state)
	{
	case 'N':
		std::cout << "No file is currently opened!" << std::endl;
		break;
	case 'L':
		std::cout << "The given address is invalid!" << std::endl;
		break;
	case 'S':
		std::cout << "File successfully saved!" << std::endl;
		break;
	default:
		std::cout << "An unknown error has occured!" << std::endl;
	}

	delete[] address;
}

void Commands::saveFile(TableFile& file)
{
	char state = file.writeTableAsFile(nullptr);

	switch (state)
	{
	case 'N':
		std::cout << "No file is currently opened!" << std::endl;
		break;
	case 'S':
		std::cout << "File successfully saved!" << std::endl;
		break;
	default:
		std::cout << "An unknown error has occured!" << std::endl;
	}
}

void Commands::exit(TableFile& file)
{
	file.closeCurrentFile();
	std::cout << "Successfully exited the program!" << std::endl;
}

void Commands::edit(TableFile& file)
{
	if (!file.isOpen())
	{
		std::cout << "No file is currently opened!" << std::endl;
		return;
	}

	size_t row, column;
	char* content;
	char cellType;
	bool validInput, endOfLine;

	row = inputNonnegativeInteger(std::cin, validInput, endOfLine);
	if (!validInput || endOfLine)
	{
		std::cout << "The given arguments are invalid!" << std::endl;
		if (!endOfLine) while (std::cin.get() != '\n');
		return;
	}

	column = inputNonnegativeInteger(std::cin, validInput, endOfLine);
	if (!validInput)
	{
		std::cout << "The given arguments are invalid!" << std::endl;
		if (!endOfLine) while (std::cin.get() != '\n');
		return;
	}

	if (!endOfLine) content = inputContent(std::cin);
	else
	{
		content = new char[1];
		content[0] = '\0';
	}

	cellType = Cell::checkType(content);
	if (!Cell::validCellType(cellType))
	{
		switch (cellType)
		{
		case 'Q':
			std::cout << "The expression is probably missing a quotation mark!" << std::endl;
			break;
		default:
				std::cout << "The expression is of unknown type!" << std::endl;
		}

	}
	else if (file.getTable().writeInCell(row - 1, column - 1, content) == 'P')
	{
		std::cout << "The given coordinates are out of range!" << std::endl;
		std::cout << "The number of rows should be between " << 1 << " and " << MAX_ROWS_IN_TABLE_LIMIT << std::endl;
		std::cout << "The number of columns should be between " << 1 << " and " << MAX_CELLS_IN_ROW_LIMIT << std::endl;
	}
	else std::cout << "The cell was successfully edited!" << std::endl;

	delete[] content;
}

void Commands::print(TableFile& file)
{
	if (!file.isOpen()) std::cout << "No file is currently opened!" << std::endl;
	else
	{
		std::cout << "Table:" << std::endl;
		file.getTable().printTable();
	}
}

void Commands::undo(TableFile& file)
{
	if (!file.isOpen()) std::cout << "No file is currently opened!" << std::endl;
	else
	{
		if (file.getTable().undoLastChange()) std::cout << "The last change was successfully undone!" << std::endl;
		else std::cout << "There are no changes to undo!" << std::endl;
	}
}

void Commands::undoAll(TableFile& file)
{
	if (!file.isOpen()) std::cout << "No file is currently opened!" << std::endl;
	else
	{
		if (file.getTable().undoAllChanges()) std::cout << "All changes were successfully undone!" << std::endl;
		else std::cout << "There are no changes to undo!" << std::endl;
	}
}

bool Commands::menu(TableFile& file)
{
	char* command;
	bool endOfLine;

	std::cout << '>';

	command = inputCommand(std::cin, endOfLine);

	if (!strcmp(command, "open"))
	{
		if (endOfLine) std::cout << "\"open\" should take arguments!" << std::endl;
		else openFile(file);
	}
	else if (!strcmp(command, "close"))
	{
		if (!endOfLine && detectExtraCharacters(std::cin)) std::cout << "\"close\" does not take any arguments!" << std::endl;
		else closeFile(file);
	}
	else if (!strcmp(command, "saveas"))
	{
		if (endOfLine) std::cout << "\"saveas\" should take arguments!" << std::endl;
		else saveAsFile(file);
	}
	else if (!strcmp(command, "save"))
	{
		if (!endOfLine && detectExtraCharacters(std::cin)) std::cout << "\"save\" does not take any arguments!" << std::endl;
		else saveFile(file);
	}
	else if (!strcmp(command, "help"))
	{
		if (!endOfLine && detectExtraCharacters(std::cin)) std::cout << "\"help\" does not take any arguments!" << std::endl;
		else help();
	}
	else if (!strcmp(command, "edit"))
	{
		if (endOfLine) std::cout << "\"edit\" should take arguments!" << std::endl;
		else edit(file);
	}
	else if (!strcmp(command, "print"))
	{
		if (!endOfLine && detectExtraCharacters(std::cin)) std::cout << "\"print\" does not take any arguments!" << std::endl;
		else print(file);
	}
	else if (!strcmp(command, "undo"))
	{
		if (!endOfLine && detectExtraCharacters(std::cin)) std::cout << "\"undo\" does not take any arguments!" << std::endl;
		else undo(file);
	}
	else if (!strcmp(command, "undoall"))
	{
		if (!endOfLine && detectExtraCharacters(std::cin)) std::cout << "\"undoall\" does not take any arguments!" << std::endl;
		else undoAll(file);
	}
	else if (!strcmp(command, "exit"))
	{
		if (!endOfLine && detectExtraCharacters(std::cin)) std::cout << "\"exit\" does not take any arguments!" << std::endl;
		else
		{
			exit(file);
			return 0;
		}
	}
	else
	{
		if (!endOfLine) while (std::cin.get() != '\n');
		std::cout << "Unknown command!" << std::endl;
	}

	return 1;
}
