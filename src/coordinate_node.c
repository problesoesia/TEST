# include "coordinate_node.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

void coordinateNode_init(tCoordinateNode* node) {
    if (node) {
        node->coordinate.latitude = 0;
        node->coordinate.longitude = 0;
        node->numPersons = 0;
        node->persons = NULL;
        node->next = NULL;
    }
}

void coordinateNode_free(tCoordinateNode* node) {
    if (node) {
        if (node->persons) {
            for (int i = 0; i < node->numPersons; i++) {
                free(node->persons[i]);
            }
            free(node->persons);
        }
        node->persons = NULL;
        node->numPersons = 0;
    }
}

int coordinateNode_findPerson(tCoordinateNode* node, const char* person) {
    if (node && person) {
        for (int i = 0; i < node->numPersons; i++) {
            if (strcmp(node->persons[i], person) == 0) {
                return i;
            }
        }
        return -1;
    } else {
        return -1;
    }
}

void coordinateNode_addPerson(tCoordinateNode* node, const char* person) {
    if (node && person) {
        int pos = coordinateNode_findPerson(node, person);
        if (pos == -1) {
            
            if (node->numPersons == 0) {
                node->persons = (char**)malloc(sizeof(char*));
            } else {
                node->persons = (char**)realloc(node->persons, (node->numPersons + 1) * sizeof(char*));
            }
            
            node->numPersons ++;
            node->persons[node->numPersons - 1] = (char*)malloc(strlen(person) + 1);
            strcpy(node->persons[node->numPersons - 1], person);
        }
    }
}

void coordinateNode_removePerson(tCoordinateNode* node, const char* person) {
    if (node && person) {
        int pos = coordinateNode_findPerson(node, person);
        if (pos != -1) {
            free(node->persons[pos]);
            for (int i = pos; i < node->numPersons - 1; i++) {
                node->persons[i] = node->persons[i + 1];
            }
            node->numPersons--;
            if (node->numPersons == 0) {
                free(node->persons);
                node->persons = NULL;
            } else {
                node->persons = (char**)realloc(node->persons, node->numPersons * sizeof(char*));
            }
        }
    }
}

int coordinateNode_countPersons(tCoordinateNode* node) {
    if (node) {
        return node->numPersons;
    } else {
        return 0;
    }
}

int coordinateNode_hasCoordinate(tCoordinateNode* node, tCoordinate coordinate) {
    if (node) {
        return node->coordinate.latitude == coordinate.latitude && node->coordinate.longitude == coordinate.longitude;
    } else {
        return 0;
    }
}

void coordinateNode_print(const tCoordinateNode* node) {
    if (node) {
        printf("Coordinate: %f, %f\n", node->coordinate.latitude, node->coordinate.longitude);
        printf("Persons: %d\n", node->numPersons);
        for (int i = 0; i < node->numPersons; i++) {
            printf("  %s\n", node->persons[i]);
        }
    }
}