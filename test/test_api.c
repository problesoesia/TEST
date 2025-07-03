#include "unity.h"
#include "api.h"
#include "error.h"
#include "person.h"
#include "date.h"

void setUp(void) {
    
}

void tearDown(void) {
    // This is run after EACH test
}

void test_api_loadData_no_reset(void) {

    tApiData data;
    api_init(&data);

    tApiError result = api_loadData(&data, "example_1.csv", false);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    TEST_ASSERT_EQUAL_INT(4, data.population.count);
    TEST_ASSERT_GREATER_THAN_INT(-1, population_find(data.population, "98001567"));
    TEST_ASSERT_GREATER_THAN_INT(-1, population_find(data.population, "49001567"));
    TEST_ASSERT_GREATER_THAN_INT(-1, population_find(data.population, "37001567"));
    TEST_ASSERT_GREATER_THAN_INT(-1, population_find(data.population, "27001567"));

    TEST_ASSERT_EQUAL_STRING("98001567", data.population.elems[0].document);
    TEST_ASSERT_EQUAL_STRING("angel", data.population.elems[0].name);
    TEST_ASSERT_EQUAL_STRING("panizo", data.population.elems[0].surname);
    TEST_ASSERT_EQUAL_STRING("28047", data.population.elems[0].cp);
    TEST_ASSERT_EQUAL_STRING("angel.panizo@upm.es", data.population.elems[0].email);
    TEST_ASSERT_EQUAL_STRING("C/Alan Turing", data.population.elems[0].address);
    TEST_ASSERT_EQUAL_INT(9, data.population.elems[0].birthday.day);
    TEST_ASSERT_EQUAL_INT(4, data.population.elems[0].birthday.month);
    TEST_ASSERT_EQUAL_INT(1990, data.population.elems[0].birthday.year);

    api_free(&data);
}


void test_api_loadData_reset(void) {

    tApiData data;
    api_init(&data);

    tApiError result = api_loadData(&data, "example_1.csv", false);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);
    result = api_loadData(&data, "example_2.csv", true);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    TEST_ASSERT_EQUAL_INT(2, data.population.count);
    TEST_ASSERT_EQUAL_INT(-1, population_find(data.population, "98001567"));
    TEST_ASSERT_EQUAL_INT(-1, population_find(data.population, "49001567"));
    TEST_ASSERT_GREATER_THAN_INT(-1, population_find(data.population, "37001567"));
    TEST_ASSERT_GREATER_THAN_INT(-1, population_find(data.population, "27001567"));

    TEST_ASSERT_EQUAL_STRING("37001567", data.population.elems[0].document);
    TEST_ASSERT_EQUAL_STRING("bob", data.population.elems[0].name);
    TEST_ASSERT_EQUAL_STRING("smith", data.population.elems[0].surname);
    TEST_ASSERT_EQUAL_STRING("28048", data.population.elems[0].cp);
    TEST_ASSERT_EQUAL_STRING("bob.smith@upm.es", data.population.elems[0].email);
    TEST_ASSERT_EQUAL_STRING("C/Baker Street", data.population.elems[0].address);
    TEST_ASSERT_EQUAL_INT(5, data.population.elems[0].birthday.day);
    TEST_ASSERT_EQUAL_INT(5, data.population.elems[0].birthday.month);
    TEST_ASSERT_EQUAL_INT(1992, data.population.elems[0].birthday.year);

    api_free(&data);
}

void test_api_add_person_geolocation_ordering(void) {
    tApiData data;
    api_init(&data);

    tApiError result = api_loadData(&data, "example_1.csv", false);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    tCoordinate coord = {40.0, -3.0};
    char* document = "98001567";
    
    tTime time = {0, 0};
    
    tDate date1 = {1, 1, 2020};
    tDateTime datetime1 = {date1, time};

    tDate date2 = {2, 1, 2020};
    tDateTime datetime2 = {date2, time};

    tDate date3 = {3, 1, 2020};
    tDateTime datetime3 = {date3, time};

    tDate date4 = {4, 1, 2020};
    tDateTime datetime4 = {date4, time};

    result = api_add_person_geolocation(&data, datetime3, document, coord);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    result = api_add_person_geolocation(&data, datetime1, document, coord);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    result = api_add_person_geolocation(&data, datetime2, document, coord);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    result = api_add_person_geolocation(&data, datetime4, document, coord);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    TEST_ASSERT_EQUAL_INT(4, data.numNodes);

    tTemporalNode* node = data.temporal_node;
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(0, dateTime_cmp(node->date, datetime1));
    TEST_ASSERT_EQUAL_INT(0, dateTime_cmp(node->next->date, datetime2));
    TEST_ASSERT_EQUAL_INT(0, dateTime_cmp(node->next->next->date, datetime3));
    TEST_ASSERT_EQUAL_INT(0, dateTime_cmp(node->next->next->next->date, datetime4));
    TEST_ASSERT_NULL(node->next->next->next->next);
    
    api_free(&data);
}

void test_api_add_person_geolocation_repeated_time(void) {
    tApiData data;
    api_init(&data);

    tApiError result = api_loadData(&data, "example_1.csv", false);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    tCoordinate coord = {40.0, -3.0};
    
    tTime time = {0, 0};
    tDate date = {1, 1, 2020};
    tDateTime datetime = {date, time};

    char* document1 = "98001567";
    char* document2 = "49001567";

    result = api_add_person_geolocation(&data, datetime, document1, coord);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    result = api_add_person_geolocation(&data, datetime, document2, coord);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    TEST_ASSERT_EQUAL_INT(1, data.numNodes);

    tTemporalNode* node = data.temporal_node;
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(0, dateTime_cmp(node->date, datetime));
    TEST_ASSERT_NULL(node->next);

    api_free(&data);
}

void test_api_remove_person(void) {
    tApiData data;
    api_init(&data);

    tApiError result = api_loadData(&data, "example_1.csv", false);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    tCoordinate coord = {40.0, -3.0};
    tTime time = {0, 0};
    
    tDate date1 = {1, 1, 2020};
    tDateTime datetime1 = {date1, time};

    tDate date2 = {2, 1, 2020};
    tDateTime datetime2 = {date2, time};

    tDate date3 = {3, 1, 2020};
    tDateTime datetime3 = {date3, time};

    tDate date4 = {4, 1, 2020};
    tDateTime datetime4 = {date4, time};

    char* document1 = "98001567";
    char* document2 = "49001567";
    char* document3 = "37001567";
    char* document4 = "27001567";

    result = api_add_person_geolocation(&data, datetime1, document1, coord);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    result = api_add_person_geolocation(&data, datetime1, document2, coord);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    result = api_add_person_geolocation(&data, datetime2, document2, coord);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    result = api_add_person_geolocation(&data, datetime3, document3, coord);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    result = api_add_person_geolocation(&data, datetime4, document4, coord);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    result = api_remove_person(&data, document2);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    TEST_ASSERT_EQUAL_INT(3, data.numNodes);

    tTemporalNode* node = data.temporal_node;
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(0, dateTime_cmp(node->date, datetime1));
    TEST_ASSERT_EQUAL_INT(0, dateTime_cmp(node->next->date, datetime3));
    TEST_ASSERT_EQUAL_INT(0, dateTime_cmp(node->next->next->date, datetime4));
    TEST_ASSERT_NULL(node->next->next->next);

    result = api_remove_person(&data, document1);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    TEST_ASSERT_EQUAL_INT(2, data.numNodes);
    node = data.temporal_node;
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(0, dateTime_cmp(node->date, datetime3));
    TEST_ASSERT_EQUAL_INT(0, dateTime_cmp(node->next->date, datetime4));
    TEST_ASSERT_NULL(node->next->next);

    result = api_remove_person(&data, document4);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    TEST_ASSERT_EQUAL_INT(1, data.numNodes);
    node = data.temporal_node;
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(0, dateTime_cmp(node->date, datetime3));
    TEST_ASSERT_NULL(node->next);

    result = api_remove_person(&data, document3);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    TEST_ASSERT_EQUAL_INT(0, data.numNodes);
    TEST_ASSERT_NULL(data.temporal_node);

    api_free(&data);
}

void test_api_remove_geodata(void) {
    tApiData data;
    api_init(&data);

    tApiError result = api_loadData(&data, "example_1.csv", false);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    tCoordinate coord = {40.0, -3.0};
    tTime time = {0, 0};
    char* document = "98001567";
    
    tDate date1 = {1, 1, 2020};
    tDateTime datetime1 = {date1, time};

    tDate date2 = {2, 1, 2020};
    tDateTime datetime2 = {date2, time};

    tDate date3 = {3, 1, 2020};
    tDateTime datetime3 = {date3, time};

    tDate date4 = {4, 1, 2020};
    tDateTime datetime4 = {date4, time};

    tDate date5 = {5, 1, 2020};
    tDateTime datetime5 = {date5, time};

    result = api_add_person_geolocation(&data, datetime1, document, coord);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    result = api_add_person_geolocation(&data, datetime2, document, coord);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    result = api_add_person_geolocation(&data, datetime3, document, coord);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    result = api_add_person_geolocation(&data, datetime4, document, coord);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    result = api_add_person_geolocation(&data, datetime5, document, coord);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    result = api_remove_geodata(&data, datetime2, datetime3);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    TEST_ASSERT_EQUAL_INT(3, data.numNodes);
    tTemporalNode* node = data.temporal_node;
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(0, dateTime_cmp(node->date, datetime1));
    TEST_ASSERT_EQUAL_INT(0, dateTime_cmp(node->next->date, datetime4));
    TEST_ASSERT_EQUAL_INT(0, dateTime_cmp(node->next->next->date, datetime5));
    TEST_ASSERT_NULL(node->next->next->next);

    result = api_remove_geodata(&data, datetime5, datetime5);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    TEST_ASSERT_EQUAL_INT(2, data.numNodes);
    node = data.temporal_node;
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(0, dateTime_cmp(node->date, datetime1));
    TEST_ASSERT_EQUAL_INT(0, dateTime_cmp(node->next->date, datetime4));
    TEST_ASSERT_NULL(node->next->next);

    result = api_remove_geodata(&data, datetime1, datetime4);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    TEST_ASSERT_EQUAL_INT(0, data.numNodes);
    TEST_ASSERT_NULL(data.temporal_node);

    api_free(&data);
}

void test_api_persons_in_contact(void) {
    tApiData data;
    api_init(&data);

    tApiError result = api_loadData(&data, "example_1.csv", false);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    tCoordinate coord = {40.0, -3.0};
    tTime time = {0, 0};
    
    tDate date1 = {1, 1, 2020};
    tDateTime datetime1 = {date1, time};

    tDate date2 = {2, 1, 2020};
    tDateTime datetime2 = {date2, time};

    tDate date3 = {3, 1, 2020};
    tDateTime datetime3 = {date3, time};

    char* document1 = "98001567";
    char* document2 = "49001567";
    char* document3 = "37001567";
    char* document4 = "27001567";

    result = api_add_person_geolocation(&data, datetime1, document1, coord);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    result = api_add_person_geolocation(&data, datetime1, document2, coord);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    result = api_add_person_geolocation(&data, datetime2, document3, coord);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    result = api_add_person_geolocation(&data, datetime2, document1, coord);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    result = api_add_person_geolocation(&data, datetime3, document4, coord);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    tPopulation population;
    population_init(&population);

    result = api_persons_in_contact(&data, datetime1, datetime3, document1, &population);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    TEST_ASSERT_EQUAL_INT(2, population.count);
    TEST_ASSERT_EQUAL_INT(-1, population_find(population, document1));
    TEST_ASSERT_GREATER_THAN_INT(-1, population_find(population, document2));
    TEST_ASSERT_GREATER_THAN_INT(-1, population_find(population, document3));
    TEST_ASSERT_EQUAL_INT(-1, population_find(population, document4));

    population_free(&population);

    population_init(&population);

    result = api_persons_in_contact(&data, datetime1, datetime1, document1, &population);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    TEST_ASSERT_EQUAL_INT(1, population.count);
    TEST_ASSERT_EQUAL_INT(-1, population_find(population, document1));
    TEST_ASSERT_EQUAL_INT(-1, population_find(population, document3));
    TEST_ASSERT_EQUAL_INT(-1, population_find(population, document4));
    TEST_ASSERT_GREATER_THAN_INT(-1, population_find(population, document2));

    population_free(&population);

    population_init(&population);

    result = api_persons_in_contact(&data, datetime3, datetime3, document1, &population);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    TEST_ASSERT_EQUAL_INT(0, population.count);

    population_free(&population);

    population_init(&population);

    result = api_persons_in_contact(&data, datetime1, datetime3, document4, &population);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    TEST_ASSERT_EQUAL_INT(0, population.count);

    population_free(&population);

    population_init(&population);
    
    char* document_fake = "0000000";

    result = api_persons_in_contact(&data, datetime1, datetime3, document_fake, &population);
    TEST_ASSERT_EQUAL_INT(E_PERSON_NOT_FOUND, result);

    population_free(&population);

    api_free(&data);
}

void test_api_get_dates(void) {
    tApiData data;
    api_init(&data);

    tApiError result = api_loadData(&data, "example_1.csv", false);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    tCoordinate coord = {40.0, -3.0};
    tTime time = {0, 0};
    char* document = "98001567";
    
    tDate date1 = {1, 1, 2020};
    tDateTime datetime1 = {date1, time};

    tDate date2 = {2, 1, 2020};
    tDateTime datetime2 = {date2, time};

    tDate date3 = {3, 1, 2020};
    tDateTime datetime3 = {date3, time};

    tDate date4 = {4, 1, 2020};
    tDateTime datetime4 = {date4, time};

    tDate date5 = {5, 1, 2020};
    tDateTime datetime5 = {date5, time};

    result = api_add_person_geolocation(&data, datetime1, document, coord);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    result = api_add_person_geolocation(&data, datetime2, document, coord);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    result = api_add_person_geolocation(&data, datetime3, document, coord);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    result = api_add_person_geolocation(&data, datetime4, document, coord);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    result = api_add_person_geolocation(&data, datetime5, document, coord);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    tDateTime min, max;
    result = api_get_dates(&data, &min, &max);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    TEST_ASSERT_EQUAL_INT(0, dateTime_cmp(min, datetime1));
    TEST_ASSERT_EQUAL_INT(0, dateTime_cmp(max, datetime5));

    api_free(&data);
}

void test_api_get_coordinates_in_date_exists(void) {
    tApiData data;
    api_init(&data);

    tApiError result = api_loadData(&data, "example_1.csv", false);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    tTime time = {0, 0};
    char* document = "98001567";
    
    tDate date1 = {1, 1, 2020};
    tDateTime datetime1 = {date1, time};

    tDate date2 = {2, 1, 2020};
    tDateTime datetime2 = {date2, time};

    tDate date3 = {3, 1, 2020};
    tDateTime datetime3 = {date3, time};

    tCoordinate coord1 = {40.0, -3.0};
    tCoordinate coord2 = {41.0, -4.0};
    tCoordinate coord3 = {42.0, -5.0};

    result = api_add_person_geolocation(&data, datetime1, document, coord1);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    result = api_add_person_geolocation(&data, datetime1, document, coord2);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    result = api_add_person_geolocation(&data, datetime2, document, coord3);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    tCoordinateArray coordinates = {NULL, 0};
    result = api_get_coordinates_in_date(&data, datetime1, &coordinates);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    TEST_ASSERT_EQUAL_INT(2, coordinates.count);
    TEST_ASSERT_EQUAL_FLOAT(coord1.latitude, coordinates.coordinates[0].latitude);
    TEST_ASSERT_EQUAL_FLOAT(coord1.longitude, coordinates.coordinates[0].longitude);
    TEST_ASSERT_EQUAL_FLOAT(coord2.latitude, coordinates.coordinates[1].latitude);
    TEST_ASSERT_EQUAL_FLOAT(coord2.longitude, coordinates.coordinates[1].longitude);

    free(coordinates.coordinates);
    coordinates.coordinates = NULL;
    coordinates.count = 0;

    result = api_get_coordinates_in_date(&data, datetime2, &coordinates);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    TEST_ASSERT_EQUAL_INT(1, coordinates.count);
    TEST_ASSERT_EQUAL_FLOAT(coord3.latitude, coordinates.coordinates[0].latitude);
    TEST_ASSERT_EQUAL_FLOAT(coord3.longitude, coordinates.coordinates[0].longitude);

    free(coordinates.coordinates);
    coordinates.coordinates = NULL;
    coordinates.count = 0;

    result = api_get_coordinates_in_date(&data, datetime3, &coordinates);
    TEST_ASSERT_EQUAL_INT(E_DATE_NOT_FOUND, result);

    TEST_ASSERT_EQUAL_INT(0, coordinates.count);

    free(coordinates.coordinates);
    coordinates.coordinates = NULL;
    coordinates.count = 0;

    api_free(&data);
}

void test_api_get_located_persons(void) {
    tApiData data;
    api_init(&data);

    tApiError result = api_loadData(&data, "example_1.csv", false);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    tCoordinate coord = {40.0, -3.0};
    tTime time = {0, 0};
    
    tDate date1 = {1, 1, 2020};
    tDateTime datetime1 = {date1, time};

    tDate date2 = {2, 1, 2020};
    tDateTime datetime2 = {date2, time};

    tDate date3 = {3, 1, 2020};
    tDateTime datetime3 = {date3, time};

    char* document1 = "98001567";
    char* document2 = "49001567";
    char* document3 = "37001567";
    char* document4 = "27001567";

    result = api_add_person_geolocation(&data, datetime1, document1, coord);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    result = api_add_person_geolocation(&data, datetime1, document2, coord);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    result = api_add_person_geolocation(&data, datetime2, document3, coord);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    result = api_add_person_geolocation(&data, datetime2, document1, coord);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    result = api_add_person_geolocation(&data, datetime3, document4, coord);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    tPopulation population;
    population_init(&population);

    result = api_get_located_persons(&data, datetime1, datetime3, &population);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    TEST_ASSERT_EQUAL_INT(4, population.count);
    TEST_ASSERT_GREATER_THAN_INT(-1, population_find(population, document1));
    TEST_ASSERT_GREATER_THAN_INT(-1, population_find(population, document2));
    TEST_ASSERT_GREATER_THAN_INT(-1, population_find(population, document3));
    TEST_ASSERT_GREATER_THAN_INT(-1, population_find(population, document4));

    population_free(&population);

    population_init(&population);

    result = api_get_located_persons(&data, datetime1, datetime1, &population);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    TEST_ASSERT_EQUAL_INT(2, population.count);
    TEST_ASSERT_GREATER_THAN_INT(-1, population_find(population, document1));
    TEST_ASSERT_GREATER_THAN_INT(-1, population_find(population, document2));
    TEST_ASSERT_EQUAL_INT(-1, population_find(population, document3));
    TEST_ASSERT_EQUAL_INT(-1, population_find(population, document4));

    population_free(&population);

    population_init(&population);

    result = api_get_located_persons(&data, datetime3, datetime3, &population);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    TEST_ASSERT_EQUAL_INT(1, population.count);
    TEST_ASSERT_EQUAL_INT(-1, population_find(population, document1));
    TEST_ASSERT_EQUAL_INT(-1, population_find(population, document2));
    TEST_ASSERT_EQUAL_INT(-1, population_find(population, document3));
    TEST_ASSERT_GREATER_THAN_INT(-1, population_find(population, document4));

    population_free(&population);

    population_init(&population);

    tDate date4 = {4, 1, 2020};
    tDateTime datetime4 = {date4, time};

    result = api_get_located_persons(&data, datetime4, datetime4, &population);
    TEST_ASSERT_EQUAL_INT(E_SUCCESS, result);

    TEST_ASSERT_EQUAL_INT(0, population.count);

    population_free(&population);

    api_free(&data);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_api_loadData_no_reset);    
    RUN_TEST(test_api_loadData_reset);
    RUN_TEST(test_api_add_person_geolocation_ordering);
    RUN_TEST(test_api_add_person_geolocation_repeated_time);
    RUN_TEST(test_api_remove_person);
    RUN_TEST(test_api_remove_geodata);
    RUN_TEST(test_api_persons_in_contact);
    RUN_TEST(test_api_get_dates);
    RUN_TEST(test_api_get_coordinates_in_date_exists);
    RUN_TEST(test_api_get_located_persons);

    return UNITY_END();
}

