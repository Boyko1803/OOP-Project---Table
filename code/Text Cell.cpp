#include "Text Cell.h"

void TextCell::deleteData()
{
	delete[] text;
}

void TextCell::copyData(const TextCell& other)
{
	text = new char[strlen(other.text) + 1];
	strcpy(text, other.text);
}

TextCell::TextCell(const char* string)
{
	size_t index = 0, newIndex = 0;

	text = new char[strlen(string) + 3];
	while (string[index] != '"' && string[index] != '\0') index++;
	if (string[index] != '\0') index++;
	while (string[index] != '"' && string[index] != '\0')
	{
		if (string[index] == '\\' && string[index + 1] == '"')
		{
			text[newIndex] = '"';
			newIndex++;
			index += 2;
		}
		else if (string[index] == '\\' && string[index + 1] == '\\')
		{
			text[newIndex] = '\\';
			newIndex++;
			index += 2;
		}
		else
		{
			text[newIndex] = string[index];
			newIndex++;
			index++;
		}
	}
	text[newIndex] = '\0';
}

TextCell::TextCell(const TextCell& other)
{
	copyData(other);
}

TextCell& TextCell::operator=(const TextCell& other)
{
	if (this != &other)
	{
		deleteData();
		copyData(other);
	}
	return *this;
}

TextCell::~TextCell()
{
	deleteData();
}

size_t TextCell::getContentLength() const
{
	return strlen(text);
}

double TextCell::getContentAsRealNumber() const
{
	char type = Cell::checkType(text);
	if (type == 'I' || type == 'R') return atof(text);
	else return 0;
}

Cell* TextCell::clone() const
{
	return new TextCell(*this);
}

char TextCell::getType() const
{
	return type;
}

void TextCell::print(std::ostream& os) const
{
	os << std::left << text;
}

void TextCell::write(std::ostream& os) const
{
	size_t index = 0, newIndex = 0, length;
	char* newString;

	length = strlen(text);
	newString = new char[length + 51];
	while (index < length)
	{
		if (text[index] == '"')
		{
			newString[newIndex] = '\\';
			newString[newIndex + 1] = '"';
			newIndex += 2;
			index++;
		}
		else if (text[index] == '\\')
		{
			newString[newIndex] = '\\';
			newString[newIndex + 1] = '\\';
			newIndex += 2;
			index++;
		}
		else
		{
			newString[newIndex] = text[index];
			index++;
			newIndex++;
		}
	}
	newString[newIndex] = '\0';

	os << ' ' << '"' << newString << '"' << ' ';
	delete[] newString;
}