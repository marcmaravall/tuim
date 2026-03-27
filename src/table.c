#include "table.h"

void tuim_table_init(TuimTable* table, size_t rows, size_t columns) {
	assert(table);

	table->columns = columns;
	table->rows = rows;

	table->data = malloc(sizeof(TuimTableCell*) * columns);
	for (size_t i = 0; i < columns; ++i) {
		table->data[i] = malloc(sizeof(TuimTableCell) * rows);
		
		for (size_t j = 0; j < rows; ++j) {
			table->data[i][j].element = NULL;
		}
	}
}

void tuim_table_update(const TuimContext* ctx, TuimTable* table) {
	assert(ctx);

	for (size_t i = 0; i < table->columns; ++i) {
		for (size_t j = 0; j < table->rows; ++j) {
			TuimElement* element = table->data[i][j].element;
			// TODO: implement: 
		}
	}
}

void tuim_table_draw(const TuimContext* ctx, const TuimTable* table) {
	assert(ctx);
}

void tuim_table_set(TuimTable* table, const size_t row, const size_t column, TuimElement* element) {
	assert(table);

	if (row >= table->rows || column >= table->columns) {
		return;
	}

	table->data[column][row].element = element;
}

TuimElement* tuim_table_get(const TuimTable* table, const size_t row, const size_t column) {
	assert(table);
	
	if (row >= table->rows || column >= table->columns) {
		return NULL;
	}

	return table->data[column][row].element;
}
