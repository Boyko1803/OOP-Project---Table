#pragma once

#include "Row.h"
#include "Stack.hpp"

#define MAX_ROWS_IN_TABLE_LIMIT 4096

class Table
{
	const static size_t INITIAL_ROWS_IN_TABLE_LIMIT = 2;

	private:
		struct PreviousContent
		{
			size_t row;
			size_t column;
			size_t previousRows;
			size_t previousColumns;
			Cell* content;

			PreviousContent(const size_t row = 0, const size_t column = 0, const size_t previousRows = 0, const size_t previousColumns = 0, const Cell* const& cellPointer = nullptr);
			PreviousContent(const PreviousContent& other);
			PreviousContent(PreviousContent&& other) noexcept;
			~PreviousContent();
		};

		struct FormulaEvaluation
		{
			size_t row;
			size_t column;
			double value;
			size_t state;
		};

		Row* tableRows;
		size_t numberOfRows;
		size_t limitOfRows;
		Stack<PreviousContent> history;

		void expandTable();

		void getOperands(double& first, double& second, Stack<double>& values) const;
		void transferValue(Stack<double>& receivingStack , Stack<double>& givingStack) const;
		size_t findFormulaIndex(const size_t row, const size_t column, FormulaEvaluation* allFormulas) const;
		double evaluateExpression(size_t& index, char* content, bool& errorFlag, FormulaEvaluation* allFormulas) const;
		void evaluateFormula(const size_t index, FormulaEvaluation* allFormulas) const;
		FormulaEvaluation* evaluateAllFormulas() const;

	public:
		Table();
		~Table();

		size_t getNumberOfRows() const;
		void printTable(std::ostream& os = std::cout) const;
		bool validCell(const size_t row, const size_t column) const;

		bool nextRow();
		char writeInNextCell(const char* string);
		char writeInCell(const size_t row, const size_t column, const char* string);
		bool undoLastChange();
		bool undoAllChanges();

		const Row& operator[](const size_t row) const;
		Row& operator[](const size_t row);
};