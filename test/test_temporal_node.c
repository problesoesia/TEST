#include <stdlib.h>
#include "unity.h"
#include "temporal_node.h"
#include "coordinate_node.h"
#include "date.h"

void setUp(void) {
    // This function is run before each test
}

void tearDown(void) {
    // This function is run after each test
}

void test_temporalNode_init(void) {
    tDate date = {1, 1, 2023};
    tTime time = {0, 0};
    tDateTime dateTime = {date, time};

    tTemporalNode *node = (tTemporalNode*) malloc(sizeof(tTemporalNode));
    temporalNode_init(node);
    node->date = dateTime;

    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(1, node->date.date.day);
    TEST_ASSERT_EQUAL_INT(1, node->date.date.month);
    TEST_ASSERT_EQUAL_INT(2023, node->date.date.year);
    TEST_ASSERT_EQUAL_INT(0, node->date.time.hour);
    TEST_ASSERT_EQUAL_INT(0, node->date.time.minutes);
    TEST_ASSERT_NULL(node->coordinateNode);
    TEST_ASSERT_NULL(node->next);
    temporalNode_free(node);
    free(node);
}

void test_temporalNode_addPersonCoordinate_same_coordinates(void) {
    tDate date = {1, 1, 2023};
    tTime time = {0, 0};
    tDateTime dateTime = {date, time};
    
    tTemporalNode *node = (tTemporalNode*) malloc(sizeof(tTemporalNode));
    temporalNode_init(node);
    node->date = dateTime;

    tCoordinate coord1 = {1.234, 5.678};
    temporalNode_addPersonCoordinate(node, coord1, "Alice");

    tListCoordinates coordinates = {NULL, 0};
    tListDocuments persons = {NULL, 0};
    
    TEST_ASSERT_EQUAL_INT(1, node->numCoordinates);    
    temporalNode_getCoordinates(node, &coordinates);
    TEST_ASSERT_EQUAL_INT(1, coordinates.numCoordinates);
    TEST_ASSERT_EQUAL_FLOAT(1.234, coordinates.coordinates[0].latitude);
    TEST_ASSERT_EQUAL_FLOAT(5.678, coordinates.coordinates[0].longitude);
    free(coordinates.coordinates);
    coordinates.coordinates = NULL;
    coordinates.numCoordinates = 0;

    temporalNode_getPersons(node, &persons);
    TEST_ASSERT_EQUAL_INT(1, persons.numDocuments);
    TEST_ASSERT_EQUAL_STRING("Alice", persons.documents[0]);
    for (int i = 0; i < persons.numDocuments; i++) {
        free(persons.documents[i]);
    }
    free(persons.documents);
    persons.documents = NULL;
    persons.numDocuments = 0;

    temporalNode_addPersonCoordinate(node, coord1, "Bob");

    TEST_ASSERT_EQUAL_INT(1, node->numCoordinates);
    temporalNode_getCoordinates(node, &coordinates);
    TEST_ASSERT_EQUAL_INT(1, coordinates.numCoordinates);
    TEST_ASSERT_EQUAL_FLOAT(1.234, coordinates.coordinates[0].latitude);
    TEST_ASSERT_EQUAL_FLOAT(5.678, coordinates.coordinates[0].longitude);
    free(coordinates.coordinates);
    coordinates.coordinates = NULL;
    coordinates.numCoordinates = 0;

    temporalNode_getPersons(node, &persons);
    TEST_ASSERT_EQUAL_INT(2, persons.numDocuments);
    TEST_ASSERT_EQUAL_STRING("Alice", persons.documents[0]);
    TEST_ASSERT_EQUAL_STRING("Bob", persons.documents[1]);
    for (int i = 0; i < persons.numDocuments; i++) {
        free(persons.documents[i]);
    }
    free(persons.documents);
    persons.documents = NULL;
    persons.numDocuments = 0;

    temporalNode_free(node);
    free(node);
}

void test_temporalNode_addPersonCoordinate_different_coordinates(void) {
    tDate date = {1, 1, 2023};
    tTime time = {0, 0};
    tDateTime dateTime = {date, time};
    
    tTemporalNode *node = (tTemporalNode*) malloc(sizeof(tTemporalNode));
    temporalNode_init(node);
    node->date = dateTime;

    tCoordinate coord1 = {1.234, 5.678};
    tCoordinate coord2 = {3.234, 6.678};
    temporalNode_addPersonCoordinate(node, coord1, "Alice");
    temporalNode_addPersonCoordinate(node, coord2, "Bob");

    tListCoordinates coordinates = {NULL, 0};
    tListDocuments persons = {NULL, 0};

    TEST_ASSERT_EQUAL_INT(2, node->numCoordinates);
    temporalNode_getCoordinates(node, &coordinates);
    TEST_ASSERT_EQUAL_INT(2, coordinates.numCoordinates);
    TEST_ASSERT_EQUAL_FLOAT(1.234, coordinates.coordinates[0].latitude);
    TEST_ASSERT_EQUAL_FLOAT(5.678, coordinates.coordinates[0].longitude);
    TEST_ASSERT_EQUAL_FLOAT(3.234, coordinates.coordinates[1].latitude);
    TEST_ASSERT_EQUAL_FLOAT(6.678, coordinates.coordinates[1].longitude);
    free(coordinates.coordinates);
    coordinates.coordinates = NULL;
    coordinates.numCoordinates = 0;
    
    temporalNode_getPersons(node, &persons);
    TEST_ASSERT_EQUAL_INT(2, persons.numDocuments);
    TEST_ASSERT_EQUAL_STRING("Alice", persons.documents[0]);
    TEST_ASSERT_EQUAL_STRING("Bob", persons.documents[1]);
    for (int i = 0; i < persons.numDocuments; i++) {
        free(persons.documents[i]);
    }
    free(persons.documents);
    persons.documents = NULL;
    persons.numDocuments = 0;
    
    temporalNode_free(node);
    free(node);
}

void test_temporalNode_removePerson_single_node(void) {
    tDate date = {1, 1, 2023};
    tTime time = {0, 0};
    tDateTime dateTime = {date, time};
    
    tTemporalNode *node = (tTemporalNode*) malloc(sizeof(tTemporalNode));
    temporalNode_init(node);
    node->date = dateTime;

    tCoordinate coord1 = {1.234, 5.678};
    temporalNode_addPersonCoordinate(node, coord1, "Alice");
    temporalNode_removePerson(node, "Alice");

    TEST_ASSERT_EQUAL_INT(0, node->numCoordinates);
    TEST_ASSERT_NULL(node->coordinateNode);

    temporalNode_free(node);
    free(node);
}

void test_temporalNode_removePerson_with_more_persons_same_node(void) {
    tDate date = {1, 1, 2023};
    tTime time = {0, 0};
    tDateTime dateTime = {date, time};
    
    tTemporalNode *node = (tTemporalNode*) malloc(sizeof(tTemporalNode));
    temporalNode_init(node);
    node->date = dateTime;

    tCoordinate coord1 = {1.234, 5.678};

    temporalNode_addPersonCoordinate(node, coord1, "Alice");
    temporalNode_addPersonCoordinate(node, coord1, "Bob");
    temporalNode_removePerson(node, "Alice");

    tListCoordinates coordinates = {NULL, 0};

    TEST_ASSERT_EQUAL_INT(1, node->numCoordinates);
    temporalNode_getCoordinates(node, &coordinates);
    TEST_ASSERT_EQUAL_INT(1, coordinates.numCoordinates);
    TEST_ASSERT_EQUAL_FLOAT(1.234, coordinates.coordinates[0].latitude);
    TEST_ASSERT_EQUAL_FLOAT(5.678, coordinates.coordinates[0].longitude);
    free(coordinates.coordinates);
    coordinates.coordinates = NULL;
    coordinates.numCoordinates = 0;

    tListDocuments persons = {NULL, 0};
    
    temporalNode_getPersons(node, &persons);
    TEST_ASSERT_EQUAL_INT(1, persons.numDocuments);
    TEST_ASSERT_EQUAL_STRING("Bob", persons.documents[0]);
    for (int i = 0; i < persons.numDocuments; i++) {
        free(persons.documents[i]);
    }
    free(persons.documents);
    persons.documents = NULL;
    persons.numDocuments = 0;

    temporalNode_free(node);
    free(node);
}

void test_temporalNode_removePerson_several_nodes(void) {
    tDate date = {1, 1, 2023};
    tTime time = {0, 0};
    tDateTime dateTime = {date, time};
    
    tTemporalNode *node = (tTemporalNode*) malloc(sizeof(tTemporalNode));
    temporalNode_init(node);
    node->date = dateTime;

    tCoordinate coord1 = {1.234, 5.678};
    tCoordinate coord2 = {3.234, 6.678};

    temporalNode_addPersonCoordinate(node, coord1, "Alice");
    temporalNode_addPersonCoordinate(node, coord2, "Alice");
    temporalNode_removePerson(node, "Alice");

    TEST_ASSERT_EQUAL_INT(0, node->numCoordinates);
    TEST_ASSERT_NULL(node->coordinateNode);

    temporalNode_free(node);
    free(node);
}

void test_temporalNode_removePerson_not_exists(void) {
    tDate date = {1, 1, 2023};
    tTime time = {0, 0};
    tDateTime dateTime = {date, time};
    
    tTemporalNode *node = (tTemporalNode*) malloc(sizeof(tTemporalNode));
    temporalNode_init(node);
    node->date = dateTime;

    tCoordinate coord1 = {1.234, 5.678};
    tCoordinate coord2 = {3.234, 6.678};
    tCoordinate coord3 = {9.234, 3.678};

    temporalNode_addPersonCoordinate(node, coord1, "Alice");
    temporalNode_addPersonCoordinate(node, coord2, "Bob");
    temporalNode_addPersonCoordinate(node, coord3, "Jane");

    temporalNode_removePerson(node, "Batman");

    tListCoordinates coordinates = {NULL, 0};

    TEST_ASSERT_EQUAL_INT(3, node->numCoordinates);
    temporalNode_getCoordinates(node, &coordinates);
    TEST_ASSERT_EQUAL_INT(3, coordinates.numCoordinates);
    TEST_ASSERT_EQUAL_FLOAT(1.234, coordinates.coordinates[0].latitude);
    TEST_ASSERT_EQUAL_FLOAT(5.678, coordinates.coordinates[0].longitude);
    TEST_ASSERT_EQUAL_FLOAT(3.234, coordinates.coordinates[1].latitude);
    TEST_ASSERT_EQUAL_FLOAT(6.678, coordinates.coordinates[1].longitude);
    TEST_ASSERT_EQUAL_FLOAT(9.234, coordinates.coordinates[2].latitude);
    TEST_ASSERT_EQUAL_FLOAT(3.678, coordinates.coordinates[2].longitude);
    free(coordinates.coordinates);
    coordinates.coordinates = NULL;
    coordinates.numCoordinates = 0;

    tListDocuments persons = {NULL, 0};
    
    temporalNode_getPersons(node, &persons);
    TEST_ASSERT_EQUAL_INT(3, persons.numDocuments);
    TEST_ASSERT_EQUAL_STRING("Alice", persons.documents[0]);
    TEST_ASSERT_EQUAL_STRING("Bob", persons.documents[1]);
    TEST_ASSERT_EQUAL_STRING("Jane", persons.documents[2]);
    for (int i = 0; i < persons.numDocuments; i++) {
        free(persons.documents[i]);
    }
    free(persons.documents);
    persons.documents = NULL;
    persons.numDocuments = 0;

    temporalNode_free(node);
    free(node);
}

void test_temporalNode_removePerson_empty_list_one_by_one(void) {
    tDate date = {1, 1, 2023};
    tTime time = {0, 0};
    tDateTime dateTime = {date, time};
    
    tTemporalNode *node = (tTemporalNode*) malloc(sizeof(tTemporalNode));
    temporalNode_init(node);
    node->date = dateTime;

    tCoordinate coord1 = {1.234, 5.678};
    tCoordinate coord2 = {3.234, 6.678};
    tCoordinate coord3 = {9.234, 3.678};

    temporalNode_addPersonCoordinate(node, coord1, "Alice");
    temporalNode_addPersonCoordinate(node, coord2, "Bob");
    temporalNode_addPersonCoordinate(node, coord3, "Jane");

    tListDocuments persons = {NULL, 0};
    tListCoordinates coordinates = {NULL, 0};

    temporalNode_removePerson(node, "Alice");

    TEST_ASSERT_EQUAL_INT(2, node->numCoordinates);
    temporalNode_getCoordinates(node, &coordinates);
    TEST_ASSERT_EQUAL_INT(2, coordinates.numCoordinates);
    TEST_ASSERT_EQUAL_FLOAT(3.234, coordinates.coordinates[0].latitude);
    TEST_ASSERT_EQUAL_FLOAT(6.678, coordinates.coordinates[0].longitude);
    TEST_ASSERT_EQUAL_FLOAT(9.234, coordinates.coordinates[1].latitude);
    TEST_ASSERT_EQUAL_FLOAT(3.678, coordinates.coordinates[1].longitude);
    free(coordinates.coordinates);
    coordinates.coordinates = NULL;
    coordinates.numCoordinates = 0;

    temporalNode_getPersons(node, &persons);
    TEST_ASSERT_EQUAL_INT(2, persons.numDocuments);
    TEST_ASSERT_EQUAL_STRING("Bob", persons.documents[0]);
    TEST_ASSERT_EQUAL_STRING("Jane", persons.documents[1]);
    for (int i = 0; i < persons.numDocuments; i++) {
        free(persons.documents[i]);
    }
    free(persons.documents);
    persons.documents = NULL;
    persons.numDocuments = 0;

    temporalNode_removePerson(node, "Bob");

    TEST_ASSERT_EQUAL_INT(1, node->numCoordinates);
    temporalNode_getCoordinates(node, &coordinates);
    TEST_ASSERT_EQUAL_INT(1, coordinates.numCoordinates);
    TEST_ASSERT_EQUAL_FLOAT(9.234, coordinates.coordinates[0].latitude);
    TEST_ASSERT_EQUAL_FLOAT(3.678, coordinates.coordinates[0].longitude);
    free(coordinates.coordinates);
    coordinates.coordinates = NULL;
    coordinates.numCoordinates = 0;

    temporalNode_getPersons(node, &persons);
    TEST_ASSERT_EQUAL_INT(1, persons.numDocuments);
    TEST_ASSERT_EQUAL_STRING("Jane", persons.documents[0]);
    for (int i = 0; i < persons.numDocuments; i++) {
        free(persons.documents[i]);
    }
    free(persons.documents);
    persons.documents = NULL;
    persons.numDocuments = 0;

    temporalNode_removePerson(node, "Jane");

    TEST_ASSERT_EQUAL_INT(0, node->numCoordinates);
    TEST_ASSERT_NULL(node->coordinateNode);

    temporalNode_free(node);
    free(node);
}

void test_temporalNode_findPersonsContatcs(void) {
    tDate date = {1, 1, 2023};
    tTime time = {0, 0};
    tDateTime dateTime = {date, time};
    
    tTemporalNode *node = (tTemporalNode*) malloc(sizeof(tTemporalNode));
    temporalNode_init(node);
    node->date = dateTime;

    tCoordinate coord1 = {1.234, 5.678};
    tCoordinate coord2 = {3.234, 6.678};

    temporalNode_addPersonCoordinate(node, coord1, "Alice");
    temporalNode_addPersonCoordinate(node, coord1, "Bob");
    temporalNode_addPersonCoordinate(node, coord2, "Jane");

    tListDocuments contacts = {NULL, 0};

    temporalNode_findPersonsContatcs(node, "Alice", &contacts);
    TEST_ASSERT_EQUAL_INT(1, contacts.numDocuments);
    TEST_ASSERT_EQUAL_STRING("Bob", contacts.documents[0]);
    free(contacts.documents[0]);
    free(contacts.documents);
    contacts.documents = NULL;
    contacts.numDocuments = 0;

    temporalNode_findPersonsContatcs(node, "Bob", &contacts);
    TEST_ASSERT_EQUAL_INT(1, contacts.numDocuments);
    TEST_ASSERT_EQUAL_STRING("Alice", contacts.documents[0]);
    free(contacts.documents[0]);
    free(contacts.documents);
    contacts.documents = NULL;
    contacts.numDocuments = 0;

    temporalNode_findPersonsContatcs(node, "Jane", &contacts);
    TEST_ASSERT_EQUAL_INT(0, contacts.numDocuments);

    temporalNode_free(node);
    free(node);
}

void test_temporalNode_countPersons(void) {
    tDate date = {1, 1, 2023};
    tTime time = {0, 0};
    tDateTime dateTime = {date, time};
    
    tTemporalNode *node = (tTemporalNode*) malloc(sizeof(tTemporalNode));
    temporalNode_init(node);
    node->date = dateTime;

    tCoordinate coord1 = {1.234, 5.678};
    tCoordinate coord2 = {3.234, 6.678};

    temporalNode_addPersonCoordinate(node, coord1, "Alice");
    TEST_ASSERT_EQUAL_INT(1, temporalNode_countPersons(node));

    temporalNode_addPersonCoordinate(node, coord2, "Alice");
    TEST_ASSERT_EQUAL_INT(1, temporalNode_countPersons(node));

    temporalNode_addPersonCoordinate(node, coord1, "Bob");
    temporalNode_addPersonCoordinate(node, coord2, "Jane");
    TEST_ASSERT_EQUAL_INT(3, temporalNode_countPersons(node));

    temporalNode_removePerson(node, "Bob");
    TEST_ASSERT_EQUAL_INT(2, temporalNode_countPersons(node));

    temporalNode_removePerson(node, "Alice");
    TEST_ASSERT_EQUAL_INT(1, temporalNode_countPersons(node));

    temporalNode_removePerson(node, "Jane");
    TEST_ASSERT_EQUAL_INT(0, temporalNode_countPersons(node));

    temporalNode_free(node);
    free(node);
}

void test_temporalNode_cmpDate(void) {
    tDate date1 = {1, 1, 2023};
    tTime time = {0, 0};
    tDateTime dateTime1 = {date1, time};

    tDate date2 = {2, 1, 2023};
    tDateTime dateTime2 = {date2, time};

    tTemporalNode *node = (tTemporalNode*) malloc(sizeof(tTemporalNode));
    temporalNode_init(node);
    node->date = dateTime1;
    TEST_ASSERT_EQUAL_INT(0, temporalNode_cmpDate(node, dateTime1));
    TEST_ASSERT_EQUAL_INT(-1, temporalNode_cmpDate(node, dateTime2));
    temporalNode_free(node);
    free(node);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_temporalNode_init);
    RUN_TEST(test_temporalNode_addPersonCoordinate_same_coordinates);
    RUN_TEST(test_temporalNode_addPersonCoordinate_different_coordinates);
    RUN_TEST(test_temporalNode_removePerson_single_node);
    RUN_TEST(test_temporalNode_removePerson_several_nodes);
    RUN_TEST(test_temporalNode_removePerson_with_more_persons_same_node);
    RUN_TEST(test_temporalNode_removePerson_not_exists);
    RUN_TEST(test_temporalNode_removePerson_empty_list_one_by_one);
    RUN_TEST(test_temporalNode_findPersonsContatcs);
    RUN_TEST(test_temporalNode_countPersons);
    RUN_TEST(test_temporalNode_cmpDate);
    return UNITY_END();
}