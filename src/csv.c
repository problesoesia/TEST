#include "csv.h"

// Function to split a string by a delimiter
char** split_string(const char *str, char delimiter, int *count) {
    char *temp = strdup(str);
    char *token;
    char **result = NULL;
    int tokens = 0;

    token = strtok(temp, &delimiter);
    while (token != NULL) {
        result = realloc(result, sizeof(char*) * (tokens + 1));
        result[tokens] = strdup(token);
        tokens++;
        token = strtok(NULL, &delimiter);
    }

    free(temp);
    *count = tokens;
    return result;
}

// Function to read a CSV file
tApiError csv_read(tCSVFile* csv, const char *filename, char delimiter) {
    if (csv) {
        FILE *file = fopen(filename, "r");
        if (file) {
            csv->rows = NULL;
            csv->row_count = 0;
            csv->headers = NULL;
            csv->header_count = 0;

            char line[1024];
            int is_first_line = 1;
            while (fgets(line, sizeof(line), file)) {
                line[strcspn(line, "\n")] = '\0'; // Remove newline character

                if (is_first_line) {
                    csv->headers = split_string(line, delimiter, &csv->header_count);
                    is_first_line = 0;
                    continue;
                }

                int field_count;
                char **fields = split_string(line, delimiter, &field_count);

                csv->rows = realloc(csv->rows, sizeof(tCSVRow) * (csv->row_count + 1));
                csv->rows[csv->row_count].fields = fields;
                csv->rows[csv->row_count].field_count = field_count;
                csv->row_count++;
            }

            fclose(file);
            return E_SUCCESS;
        } else {
            return E_FILE_NOT_FOUND;
        } 
    } else {
        return E_MEMORY_ERROR;
    }
}

// Function to free the memory allocated for the CSV file
void csv_free(tCSVFile *csv) {
    for (int i = 0; i < csv->row_count; i++) {
        for (int j = 0; j < csv->rows[i].field_count; j++) {
            free(csv->rows[i].fields[j]);
        }
        free(csv->rows[i].fields);
    }
    for (int i = 0; i < csv->header_count; i++) {
        free(csv->headers[i]);
    }
    free(csv->headers);
    free(csv->rows);
}

// Function to get the index of a column by name
int csv_get_column_index(tCSVFile *csv, const char *column) {
    for (int i = 0; i < csv->header_count; i++) {
        if (strcmp(csv->headers[i], column) == 0) {
            return i;
        }
    }
    return -1; // Column not found
}

// Function to get an item by row and column name
char* csv_get_item_by_row_and_column_name(tCSVFile *csv, int row, const char *column) {
    int col_index = csv_get_column_index(csv, column);
    if (col_index == -1 || row >= csv->row_count) {
        return NULL; // Column or row not found
    }
    return csv->rows[row].fields[col_index];
}

// Function to get the number of columns in the CSV file
int csv_get_column_count(tCSVFile *csv) {
    return csv->header_count;
}

// Function to get the number of rows in the CSV file
int csv_get_row_count(tCSVFile *csv) {
    return csv->row_count;
}
