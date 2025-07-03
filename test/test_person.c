#include "unity.h"
#include "person.h"
#include "date.h"
#include <string.h>

void setUp(void) {
    // This function is run before each test
}

void tearDown(void) {
    // This function is run after each test
}

void test_population_init(void) {
    tPopulation population;
    population_init(&population);
    TEST_ASSERT_EQUAL_INT(0, population.count);
    TEST_ASSERT_NULL(population.elems);
}

void test_person_free(void) {
    tPerson person;
    person.document = strdup("12345");
    person.name = strdup("John");
    person.surname = strdup("Doe");
    person.cp = strdup("12345");
    person.email = strdup("john.doe@example.com");
    person.address = strdup("123 Main St");
    sscanf("01/01/2000", "%d/%d/%d", &(person.birthday.day), &(person.birthday.month), &(person.birthday.year));

    person_free(&person);

    TEST_ASSERT_NULL(person.document);
    TEST_ASSERT_NULL(person.name);
    TEST_ASSERT_NULL(person.surname);
    TEST_ASSERT_NULL(person.cp);
    TEST_ASSERT_NULL(person.email);
    TEST_ASSERT_NULL(person.address);
}

void test_population_add_and_find(void) {
    tPopulation population;
    population_init(&population);

    tPerson person;
    person.document = strdup("12345");
    person.name = strdup("John");
    person.surname = strdup("Doe");
    person.cp = strdup("12345");
    person.email = strdup("john.doe@example.com");
    person.address = strdup("123 Main St");
    sscanf("01/01/2000", "%d/%d/%d", &(person.birthday.day), &(person.birthday.month), &(person.birthday.year));

    population_add(&population, person);
    TEST_ASSERT_EQUAL_INT(1, population.count);
    TEST_ASSERT_EQUAL_STRING("12345", population.elems[0].document);

    int index = population_find(population, "12345");
    TEST_ASSERT_EQUAL_INT(0, index);

    person_free(&person);
    population_free(&population);
}

void test_population_del(void) {
    tPopulation population;
    population_init(&population);

    tPerson person;
    person.document = strdup("12345");
    person.name = strdup("John");
    person.surname = strdup("Doe");
    person.cp = strdup("12345");
    person.email = strdup("john.doe@example.com");
    person.address = strdup("123 Main St");
    sscanf("01/01/2000", "%d/%d/%d", &(person.birthday.day), &(person.birthday.month), &(person.birthday.year));

    population_add(&population, person);
    population_del(&population, "12345");

    TEST_ASSERT_EQUAL_INT(0, population.count);
    TEST_ASSERT_NULL(population.elems);

    person_free(&person); // Clean up temporary person
    population_free(&population); // Clean up population
}

void test_population_print(void) {
    tPopulation population;
    population_init(&population);

    tPerson person;
    person.document = strdup("12345");
    person.name = strdup("John");
    person.surname = strdup("Doe");
    person.cp = strdup("12345");
    person.email = strdup("john.doe@example.com");
    person.address = strdup("123 Main St");
    sscanf("01/01/2000", "%d/%d/%d", &(person.birthday.day), &(person.birthday.month), &(person.birthday.year));

    population_add(&population, person);

    // Redirect stdout to a string
    char buffer[256];
    FILE* f = fmemopen(buffer, sizeof(buffer), "w");
    stdout = f;
    population_print(population);
    fclose(f);
    stdout = stdout; // Restore stdout

    const char* expected_output = "0;12345;John;Doe;john.doe@example.com;123 Main St;12345;01/01/2000\n";
    TEST_ASSERT_EQUAL_STRING(expected_output, buffer);

    person_free(&person); // Clean up temporary person
    population_free(&population); // Clean up population
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_population_init);
    RUN_TEST(test_person_free);
    RUN_TEST(test_population_add_and_find);
    RUN_TEST(test_population_del);
    return UNITY_END();
}
