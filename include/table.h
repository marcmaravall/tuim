#ifndef TUIM_TABLE_H
#define TUIM_TABLE_H

#include "element.h"
#include "color.h"

// fat pointer for table cell
typedef struct {
	TuimElement* element;
	// idk its just a wrapper ...

} TuimTableCell;

typedef struct {
	// typedef struct TuimTableStyle {
	// 	
	// } TuimTableStyle;
	// 
	// TuimTableStyle* style;

	TuimTableCell** data;
	size_t rows;
	size_t columns;

} TuimTable;

void tuim_table_init(TuimTable* table, size_t rows, size_t columns);

void tuim_table_update(const TuimContext* ctx, TuimTable* table);
void tuim_table_draw(const TuimContext* ctx, const TuimTable* table);
void tuim_table_destroy(TuimTable* table);

void tuim_table_set(TuimTable* table, const size_t row, const size_t column, TuimElement* element);
TuimElement* tuim_table_get(const TuimTable* table, const size_t row, const size_t column);

TuimElement tuim_table_to_element(TuimTable* table);
TuimSizeHint tuim_table_measure(const TuimTable* table);
void tuim_table_layout(TuimTable* table, const TuimRect rect);

#endif // TUIM_TABLE_H