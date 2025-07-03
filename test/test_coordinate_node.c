# include "unity.h"
# include "coordinate_node.h"
# include <stdlib.h>

// Test functions

void setUp(void) {
    // This function is run before each test
}

void tearDown(void) {
    // This function is run after each test
}

void test_coordinateNode_init(void) {
    tCoordinateNode *node = (tCoordinateNode*) malloc(sizeof(tCoordinateNode));
    TEST_ASSERT_NOT_NULL(node);
    coordinateNode_init(node);
    
    tCoordinate coord = {1.234, 5.678};
    node->coordinate = coord;
    
    TEST_ASSERT_EQUAL_FLOAT(1.234, node->coordinate.latitude);
    TEST_ASSERT_EQUAL_FLOAT(5.678, node->coordinate.longitude);
    TEST_ASSERT_EQUAL_INT(0, node->numPersons);
    TEST_ASSERT_NULL(node->persons);
    TEST_ASSERT_NULL(node->next);
    coordinateNode_free(node);
    free(node);
}

void test_coordinateNode_add_and_find_person(void) {
    tCoordinate coord = {1.234, 5.678};
    tCoordinateNode *node = (tCoordinateNode*) malloc(sizeof(tCoordinateNode));
    TEST_ASSERT_NOT_NULL(node);
    coordinateNode_init(node);
    node->coordinate = coord;

    coordinateNode_addPerson(node, "Alice");
    TEST_ASSERT_EQUAL_INT(1, coordinateNode_countPersons(node));
    TEST_ASSERT_EQUAL_INT(0, coordinateNode_findPerson(node, "Alice"));

    coordinateNode_addPerson(node, "Alice");
    TEST_ASSERT_EQUAL_INT(1, coordinateNode_countPersons(node));
    TEST_ASSERT_EQUAL_INT(0, coordinateNode_findPerson(node, "Alice"));

    coordinateNode_addPerson(node, "Bob");
    TEST_ASSERT_EQUAL_INT(2, coordinateNode_countPersons(node));
    TEST_ASSERT_EQUAL_INT(0, coordinateNode_findPerson(node, "Alice"));
    TEST_ASSERT_EQUAL_INT(1, coordinateNode_findPerson(node, "Bob"));
    
    coordinateNode_free(node);
    free(node);
}

void test_coordinateNode_remove_person(void) {
    tCoordinate coord = {1.234, 5.678};
    tCoordinateNode *node = (tCoordinateNode*) malloc(sizeof(tCoordinateNode));
    TEST_ASSERT_NOT_NULL(node);
    coordinateNode_init(node);
    node->coordinate = coord;

    coordinateNode_addPerson(node, "Alice");
    coordinateNode_addPerson(node, "Bob");

    coordinateNode_removePerson(node, "Jane");
    TEST_ASSERT_EQUAL_INT(2, coordinateNode_countPersons(node));
    TEST_ASSERT_EQUAL_INT(0, coordinateNode_findPerson(node, "Alice"));
    TEST_ASSERT_EQUAL_INT(1, coordinateNode_findPerson(node, "Bob"));
    
    coordinateNode_removePerson(node, "Alice");
    TEST_ASSERT_EQUAL_INT(1, coordinateNode_countPersons(node));
    TEST_ASSERT_EQUAL_INT(-1, coordinateNode_findPerson(node, "Alice"));
    TEST_ASSERT_EQUAL_INT(0, coordinateNode_findPerson(node, "Bob"));

    coordinateNode_removePerson(node, "Alice");
    TEST_ASSERT_EQUAL_INT(1, coordinateNode_countPersons(node));
    TEST_ASSERT_EQUAL_INT(-1, coordinateNode_findPerson(node, "Alice"));
    TEST_ASSERT_EQUAL_INT(0, coordinateNode_findPerson(node, "Bob"));

    coordinateNode_removePerson(node, "Bob");
    TEST_ASSERT_EQUAL_INT(0, coordinateNode_countPersons(node));
    TEST_ASSERT_EQUAL_INT(-1, coordinateNode_findPerson(node, "Bob"));
    TEST_ASSERT_NULL(node->persons);

    coordinateNode_free(node);
    free(node);
}

void test_coordinateNode_countPersons(void) {
    tCoordinate coord = {1.234, 5.678};
    tCoordinateNode *node = (tCoordinateNode*) malloc(sizeof(tCoordinateNode));
    TEST_ASSERT_NOT_NULL(node);
    coordinateNode_init(node);
    node->coordinate = coord;

    coordinateNode_addPerson(node, "Alice");
    TEST_ASSERT_EQUAL_INT(1, coordinateNode_countPersons(node));

    coordinateNode_addPerson(node, "Alice");
    TEST_ASSERT_EQUAL_INT(1, coordinateNode_countPersons(node));

    coordinateNode_addPerson(node, "Bob");
    TEST_ASSERT_EQUAL_INT(2, coordinateNode_countPersons(node));

    coordinateNode_removePerson(node, "Bob");
    TEST_ASSERT_EQUAL_INT(1, coordinateNode_countPersons(node));

    coordinateNode_free(node);
    free(node);
}

void test_coordinateNode_hasCoordinate(void) {
    tCoordinate coord = {1.234, 5.678};
    tCoordinateNode *node = (tCoordinateNode*) malloc(sizeof(tCoordinateNode));
    TEST_ASSERT_NOT_NULL(node);
    coordinateNode_init(node);
    node->coordinate = coord;

    TEST_ASSERT_EQUAL_INT(1, coordinateNode_hasCoordinate(node, coord));
    coordinateNode_free(node);
    free(node);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_coordinateNode_init);
    RUN_TEST(test_coordinateNode_add_and_find_person);
    RUN_TEST(test_coordinateNode_remove_person);
    RUN_TEST(test_coordinateNode_countPersons);
    RUN_TEST(test_coordinateNode_hasCoordinate);
    return UNITY_END();
}