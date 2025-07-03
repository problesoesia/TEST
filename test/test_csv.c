#include "unity.h"
#include "csv.h"

void setUp(void) {
    // This is run before EACH test
}

void tearDown(void) {
    // This is run after EACH test
}

void test_read_csv_file(void) {
    tCSVFile *csv = (tCSVFile*) malloc(sizeof(tCSVFile));
    TEST_ASSERT_NOT_NULL(csv);

    tApiError error = csv_read(csv, "example_1.csv", ',');
    TEST_ASSERT_EQUAL(E_SUCCESS, error);

    TEST_ASSERT_EQUAL(4, csv->row_count);
    TEST_ASSERT_EQUAL(7, csv->header_count);
    TEST_ASSERT_EQUAL(0, csv_get_column_index(csv, "name"));
    TEST_ASSERT_EQUAL(1, csv_get_column_index(csv, "surname"));
    TEST_ASSERT_EQUAL(4, csv_get_column_index(csv, "document"));
    
    csv_free(csv);
    free(csv);
}

void test_get_item_by_row_and_column_name(void) {
    tCSVFile *csv = (tCSVFile*) malloc(sizeof(tCSVFile));
    TEST_ASSERT_NOT_NULL(csv);

    tApiError error = csv_read(csv, "example_1.csv", ',');
    TEST_ASSERT_EQUAL(E_SUCCESS, error);
    
    TEST_ASSERT_EQUAL_STRING("angel", csv_get_item_by_row_and_column_name(csv, 0, "name"));
    TEST_ASSERT_EQUAL_STRING("panizo", csv_get_item_by_row_and_column_name(csv, 0, "surname"));
    TEST_ASSERT_EQUAL_STRING("98001567", csv_get_item_by_row_and_column_name(csv, 0, "document"));

    TEST_ASSERT_EQUAL_STRING("harry", csv_get_item_by_row_and_column_name(csv, 1, "name"));
    TEST_ASSERT_EQUAL_STRING("potter", csv_get_item_by_row_and_column_name(csv, 1, "surname"));
    TEST_ASSERT_EQUAL_STRING("49001567", csv_get_item_by_row_and_column_name(csv, 1, "document"));
    
    csv_free(csv);
    free(csv);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_read_csv_file);
    RUN_TEST(test_get_item_by_row_and_column_name);
    return UNITY_END();
}
