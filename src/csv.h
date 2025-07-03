#ifndef CSV_H
#define CSV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"

// Structure to store a single CSV row
typedef struct _CSVRow {
    char **fields;
    int field_count;
} tCSVRow;

// Structure to store the entire CSV data
typedef struct _CSVFile {
    char **headers;
    int header_count;
    tCSVRow *rows;
    int row_count;
} tCSVFile;

// Function prototypes
tApiError csv_read(tCSVFile* csv, const char *filename, char delimiter);
void csv_free(tCSVFile *csv);
char* csv_get_item_by_row_and_column_name(tCSVFile *csv, int row, const char *column);
int csv_get_column_index(tCSVFile *csv, const char *column);
int csv_get_column_count(tCSVFile *csv);
int csv_get_row_count(tCSVFile *csv);

#endif // CSV_H
