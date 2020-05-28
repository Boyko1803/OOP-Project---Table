#include "Table.h"

Table::PreviousContent::PreviousContent(const size_t rowIndex, const size_t columnIndex, const size_t previousRowsNumber, const size_t previousColumnsNumber, const Cell* const& cellPointer) :
	row(rowIndex),
	column(columnIndex),
	previousRows(previousRowsNumber),
	previousColumns(previousColumnsNumber)
{
	if (cellPointer == nullptr) content = new Cell;
	else content = cellPointer->clone();
}

Table::PreviousContent::PreviousContent(const Table::PreviousContent& other) :
	row(other.row),
	column(other.column),
	previousRows(other.previousRows),
	previousColumns(other.previousColumns),
	content(other.content->clone())
{}

Table::PreviousContent::PreviousContent(Table::PreviousContent&& other) noexcept :
	row(other.row),
	column(other.column),
	content(other.content),
	previousRows(other.previousRows),
	previousColumns(other.previousColumns)
{
	other.content = nullptr;
}

Table::PreviousContent::~PreviousContent()
{
	if (content != nullptr) delete content;
}

void Table::expandTable()
{
	Row* temp = new Row[2 * limitOfRows];
	for (size_t i = 0; i < numberOfRows; i++) temp[i] = tableRows[i];
	delete[] tableRows;
	tableRows = temp;
	limitOfRows *= 2;
}

void Table::getOperands(double& first, double& second, Stack<double>& values) const
{
	first = values.top();
	values.pop();
	second = values.top();
	values.pop();
}

void Table::transferValue(Stack<double>& first, Stack<double>& second) const
{
	second.push(first.top());
	first.pop();
}

size_t Table::findFormulaIndex(const size_t row, const size_t column, FormulaEvaluation* allFormulas) const
{
	size_t index = 0;
	while (allFormulas[index].row != row || allFormulas[index].column != column) index++;
	return index;
}

double Table::evaluateExpression(size_t& index, char* content, bool& errorFlag, FormulaEvaluation* allFormulas) const
{
	double firstOperand, secondOperand, result;
	char operation;
	bool unaryOperation = 1;
	Stack<double> firstIterationValues, secondIterationValues, thirdIterationValues, tempValues;
	Stack<char> firstIterationOperations, secondIterationOperations, thirdIterationOperations, tempOperations;

	while (content[index] != '\0' && content[index] != ')')
	{
		if (content[index] == ' ') index++;
		else if (content[index] == '(')
		{
			unaryOperation = 0;
			index++;
			result = evaluateExpression(index, content, errorFlag, allFormulas);
			if (errorFlag) return 0;
			firstIterationValues.push(result);
			if (content[index] == ')') index++;
		}
		else if (content[index] == '+' || content[index] == '-' || content[index] == '*' || content[index] == '/' || content[index] == '^')
		{
			if (unaryOperation) firstIterationValues.push(0.);
			unaryOperation = 0;
			firstIterationOperations.push(content[index]);
			index++;
		}
		else if (content[index] == '.' || (content[index] >= '0' && content[index] <= '9'))
		{
			unaryOperation = 0;
			firstIterationValues.push(atof(content + index));
			while (content[index] == '.' || (content[index] >= '0' && content[index] <= '9')) index++;
		}
		else if (content[index] == 'R')
		{
			unaryOperation = 0;
			size_t row, column;
			index++;
			row = atoi(content + index) - 1;
			while (content[index] >= '0' && content[index] <= '9') index++;
			index++;
			column = atoi(content + index) - 1;
			while (content[index] >= '0' && content[index] <= '9') index++;

			if (!validCell(row, column)) firstIterationValues.push(0.);
			else
			{
				if (tableRows[row][column]->getType() != 'F') firstIterationValues.push(tableRows[row][column]->getContentAsRealNumber());
				else
				{
					size_t dependentFormulaIndex = findFormulaIndex(row, column, allFormulas);
					evaluateFormula(dependentFormulaIndex, allFormulas);
					if (allFormulas[dependentFormulaIndex].state != 3)
					{
						errorFlag = 1;
						return 0;
					}
					firstIterationValues.push(allFormulas[dependentFormulaIndex].value);
				}
			}
		}
		else
		{
			errorFlag = 1;
			return 0;
		}
	}
	if (firstIterationValues.getSize() != firstIterationOperations.getSize() + 1)
	{
		errorFlag = 1;
		return 0;
	}

	while (!firstIterationOperations.isEmpty())
	{
		operation = firstIterationOperations.top();
		firstIterationOperations.pop();
		if (operation == '^')
		{
			getOperands(secondOperand, firstOperand, firstIterationValues);
			if (isZero(firstOperand) && (isZero(secondOperand) || secondOperand < 0))
			{
				errorFlag = 1;
				return 0;
			}
			result = pow(firstOperand, secondOperand);
			firstIterationValues.push(result);
		}
		else
		{
			secondIterationOperations.push(operation);
			transferValue(firstIterationValues, secondIterationValues);
		}
	}
	
	transferValue(firstIterationValues, secondIterationValues);
	while (!secondIterationOperations.isEmpty())
	{
		operation = secondIterationOperations.top();
		secondIterationOperations.pop();
		if (operation == '*' || operation == '/')
		{
			getOperands(firstOperand, secondOperand, secondIterationValues);
			if (operation == '/' && isZero(secondOperand))
			{
				errorFlag = 1;
				return 0;
			}
			if (operation == '*') result = firstOperand * secondOperand;
			else result = firstOperand / secondOperand;
			secondIterationValues.push(result);
		}
		else
		{
			tempOperations.push(operation);
			transferValue(secondIterationValues, tempValues);
		}
	}
	
	transferValue(secondIterationValues, tempValues);
	thirdIterationValues.transferStackInReverse(tempValues);
	thirdIterationOperations.transferStackInReverse(tempOperations);

	while (!thirdIterationOperations.isEmpty())
	{
		operation = thirdIterationOperations.top();
		thirdIterationOperations.pop();
		getOperands(firstOperand, secondOperand, thirdIterationValues);
		if (operation == '+') result = firstOperand + secondOperand;
		else result = firstOperand - secondOperand;
		thirdIterationValues.push(result);
	}

	return thirdIterationValues.top();
}

void Table::evaluateFormula(const size_t formulaIndex, FormulaEvaluation* allFormulas) const
{
	if (allFormulas[formulaIndex].state != 1) return;
	allFormulas[formulaIndex].state = 2;
	char* content = ((FormulaCell*)tableRows[allFormulas[formulaIndex].row][allFormulas[formulaIndex].column])->getFormulaExpression();
	double result;
	size_t index = 1;
	bool errorFlag = 0;
	
	result = evaluateExpression(index, content, errorFlag, allFormulas);
	delete[] content;

	if (errorFlag) allFormulas[formulaIndex].state = 0;
	else
	{
		allFormulas[formulaIndex].state = 3;
		allFormulas[formulaIndex].value = result;
	}
}

Table::FormulaEvaluation* Table::evaluateAllFormulas() const
{
	size_t numberOfColumns, formulaIndex = 0, numberOfFormulas = 0;

	for (size_t row = 0; row < numberOfRows; row++)
	{
		numberOfColumns = tableRows[row].getNumberOfCells();
		for (size_t column = 0; column < numberOfColumns; column++)
		{
			if (tableRows[row][column]->getType() == 'F') numberOfFormulas++;
		}
	}

	FormulaEvaluation* allFormulas = new FormulaEvaluation[numberOfFormulas + 2];

	for (size_t row = 0; row < numberOfRows; row++)
	{
		numberOfColumns = tableRows[row].getNumberOfCells();
		for (size_t column = 0; column < numberOfColumns; column++)
		{
			if (tableRows[row][column]->getType() == 'F')
			{
				allFormulas[formulaIndex] = { row, column, 0., 1 };
				formulaIndex++;
			}
		}
	}

	for (formulaIndex = 0; formulaIndex < numberOfFormulas; formulaIndex++)
	{
		evaluateFormula(formulaIndex, allFormulas);
	}

	return allFormulas;
}

Table::Table()
{
	limitOfRows = INITIAL_ROWS_IN_TABLE_LIMIT;
	tableRows = new Row[limitOfRows];
	numberOfRows = 0;
}

Table::~Table()
{
	delete[] tableRows;
}

size_t Table::getNumberOfRows() const
{
	return numberOfRows;
}

void Table::printTable(std::ostream& os) const
{
	FormulaEvaluation* allFormulas = evaluateAllFormulas();
	size_t numberOfColumns = 0, formulaIndex = 0;

	for (size_t row = 0; row < numberOfRows; row++)
	{
		if (numberOfColumns < tableRows[row].getNumberOfCells()) numberOfColumns = tableRows[row].getNumberOfCells();
	}

	size_t* columnSpace = new size_t[numberOfColumns];

	for (size_t column = 0; column < numberOfColumns; column++)
	{
		columnSpace[column] = 1;
		for (size_t row = 0; row < numberOfRows; row++)
		{
			if (!validCell(row, column)) continue;

			if (tableRows[row][column]->getType() != 'F')
			{
				if (columnSpace[column] < tableRows[row][column]->getContentLength()) columnSpace[column] = tableRows[row][column]->getContentLength();
			}
			else
			{
				formulaIndex = findFormulaIndex(row, column, allFormulas);
				if (allFormulas[formulaIndex].state == 0 && columnSpace[column] < strlen(FORMULA_ERROR_MESSAGE)) columnSpace[column] = strlen(FORMULA_ERROR_MESSAGE);
				else if (allFormulas[formulaIndex].state == 3)
				{
					RealCell temp(allFormulas[formulaIndex].value);
					if (columnSpace[column] < temp.getContentLength()) columnSpace[column] = temp.getContentLength();
				}
			}
		}
	}

	formulaIndex = 0;
	for (size_t row = 0; row < numberOfRows; row++)
	{
		for (size_t column = 0; column < numberOfColumns; column++)
		{
			if (validCell(row, column))
			{
				if (tableRows[row][column]->getType() != 'F')
				{
					os << std::setw(columnSpace[column]);
					tableRows[row][column]->print(os);
				}
				else
				{
					if (allFormulas[formulaIndex].state == 3)
					{
						RealCell temp(allFormulas[formulaIndex].value);
						os << std::setw(columnSpace[column]);
						temp.print(os);
					}
					else os << std::setw(columnSpace[column]) << std::left << FORMULA_ERROR_MESSAGE;
					formulaIndex++;
				}
			}
			else os << std::setw(columnSpace[column]) << ' ';
			os << " | ";
		}
		os << std::endl;
	}

	delete[] allFormulas;
	delete[] columnSpace;
}

bool Table::validCell(const size_t row, const size_t column) const
{
	if (row >= 0 && row < numberOfRows) return tableRows[row].validCell(column);
	return 0;
}

bool Table::nextRow()
{
	if (numberOfRows >= MAX_ROWS_IN_TABLE_LIMIT) return 0;
	if (numberOfRows >= limitOfRows) expandTable();
	numberOfRows++;
	return 1;
}

char Table::writeInNextCell(const char* string)
{
	if (numberOfRows == 0) return 'X';
	return tableRows[numberOfRows - 1].writeInNextCell(string);
}

char Table::writeInCell(const size_t row, const size_t column, const char* string)
{
	char type;
	if (row >= 0 && row < MAX_ROWS_IN_TABLE_LIMIT)
	{
		if (validCell(row, column)) history.push(std::move(PreviousContent(row, column, numberOfRows, tableRows[row].getNumberOfCells(), tableRows[row][column])));
		else
		{
			if (row >= numberOfRows) history.push(std::move(PreviousContent(row, column, numberOfRows, 0, nullptr)));
			else  history.push(std::move(PreviousContent(row, column, numberOfRows, tableRows[row].getNumberOfCells(), nullptr)));
		}

		while (row >= limitOfRows) expandTable();
		type = tableRows[row].writeInCell(column, string);
		if (row >= numberOfRows) numberOfRows = row + 1;
		return type;
	}
	return 'P';
}

bool Table::undoLastChange()
{
	if (history.isEmpty()) return 0;
	delete tableRows[history.top().row][history.top().column];
	tableRows[history.top().row][history.top().column] = history.top().content->clone();
	tableRows[history.top().row].restractRow(history.top().previousColumns);
	if (history.top().previousRows < numberOfRows) numberOfRows = history.top().previousRows;

	history.pop();
	return 1;
}

bool Table::undoAllChanges()
{
	bool state = undoLastChange();
	while (!history.isEmpty()) undoLastChange();
	return state;
}

const Row& Table::operator[](const size_t row) const
{
	return tableRows[row];
}

Row& Table::operator[](const size_t row)
{
	return tableRows[row];
}