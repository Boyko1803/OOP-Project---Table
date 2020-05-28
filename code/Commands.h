#pragma once

#include "Table File.h"

class Commands
{
	const static size_t INITIAL_COMMAND_READ_LIMIT = 64;
	const static size_t HELP_OUTPUT_WIDTH = 32;
	const static size_t INTEGER_DIGITS_LIMIT = 8;

private:
	Commands();

	static void expandString(char*& string, size_t& stringLimit);
	static char* inputContent(std::istream& is, const char endSymbol = '\n', const char fillerSymbol = ' ');
	static char* inputCommand(std::istream& is, bool& endOfLine);
	static int inputNonnegativeInteger(std::istream& is, bool& validInput, bool& endOfLine, const char endSymbol = '\n', const char fillerSymbol = ' ');
	static bool detectExtraCharacters(std::istream& is, const char endSymbol = '\n', const char fillerSymbol = ' ');
	static void outputContent(std::ostream& is, const char* content, const char fillerSymbol = ' ');
public:
	Commands(const Commands&) = delete;
	Commands& operator=(const Commands&) = delete;

	static void openFile(TableFile& file);
	static void closeFile(TableFile& file);
	static void saveAsFile(TableFile& file);
	static void saveFile(TableFile& file);
	static void help();
	static void exit(TableFile& file);

	static void edit(TableFile& file);
	static void print(TableFile& file);
	static void undo(TableFile& file);
	static void undoAll(TableFile& file);

	static bool menu(TableFile& file);
};