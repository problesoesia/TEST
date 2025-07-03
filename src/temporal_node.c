# include "temporal_node.h"
# include "coordinate_node.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

void temporalNode_init(tTemporalNode* node) {
    if (node) {
        node->coordinateNode = NULL;
        node->date.date.year = 0;
        node->date.date.month = 0;
        node->date.date.day = 0;
        node->date.time.hour = 0;
        node->date.time.minutes = 0;
        node->next = NULL;
        node->numCoordinates = 0;
    }
}

void temporalNode_free(tTemporalNode* node) {
    if (node) {
        tCoordinateNode* aux = node->coordinateNode;
        while (aux) {
            tCoordinateNode* next = aux->next;
            coordinateNode_free(aux);
            free(aux);
            aux = next;
        }
        node->coordinateNode = NULL;
        node->numCoordinates = 0;
    }
}

void temporalNode_addPersonCoordinate(tTemporalNode* node, tCoordinate coordinate, const char* person) {
    if (node && person) {
        if (!node->coordinateNode) {
            node->coordinateNode = (tCoordinateNode*)malloc(sizeof(tCoordinateNode));
            coordinateNode_init(node->coordinateNode);
            node->coordinateNode->coordinate = coordinate;
            node->numCoordinates++;
            
            coordinateNode_addPerson(node->coordinateNode, person);
        }
        else {
            tCoordinateNode* aux = node->coordinateNode;
            if (aux->coordinate.latitude == coordinate.latitude && aux->coordinate.longitude == coordinate.longitude) {
                coordinateNode_addPerson(aux, person);
                return;
            }

            while (aux->next) {
                aux = aux->next;
                if (aux->coordinate.latitude == coordinate.latitude && aux->coordinate.longitude == coordinate.longitude) {
                    coordinateNode_addPerson(aux, person);
                    return;
                }
            }

            aux->next = (tCoordinateNode*)malloc(sizeof(tCoordinateNode));
            coordinateNode_init(aux->next);
            aux->next->coordinate = coordinate;
            node->numCoordinates++;

            coordinateNode_addPerson(aux->next, person);
        }
    }        
}

void temporalNode_removePerson(tTemporalNode* node, const char* person) {
    if (node && person) {
        tCoordinateNode* aux = node->coordinateNode;
        tCoordinateNode* prev = NULL;
        while (aux) {
            coordinateNode_removePerson(aux, person);
            if (aux->numPersons == 0) {
                node->numCoordinates--;

                if (node->numCoordinates == 0) {
                    coordinateNode_free(aux);
                    free(aux);
                    node->coordinateNode = NULL;
                    aux = NULL;
                } else {
                    
                    if (prev) {
                        prev->next = aux->next;
                        coordinateNode_free(aux);
                        free(aux);
                        aux = prev->next;
                    } else {
                        node->coordinateNode = aux->next;
                        coordinateNode_free(aux);
                        free(aux);
                        aux = node->coordinateNode;
                    }
                }
            } else {
                prev = aux;
                aux = aux->next;
            }
        }
    }
}

void temporalNode_findPersonsContatcs(tTemporalNode* node, const char* person, tListDocuments* list) {
    if (node && person) {
        tCoordinateNode* aux = node->coordinateNode;
        while (aux) {
            if (coordinateNode_findPerson(aux, person) != -1) {
                char** persons = aux->persons;
                for (int i = 0; i < aux->numPersons; i++) {
                    if (strcmp(persons[i], person) != 0) {
                        if (list->numDocuments == 0) {
                            list->documents = (char**)malloc(sizeof(char*));
                        } else {
                            list->documents = (char**)realloc(list->documents, (list->numDocuments + 1) * sizeof(char*));
                        }
                        list->documents[list->numDocuments] = (char*)malloc((strlen(persons[i]) + 1) * sizeof(char));
                        strcpy(list->documents[list->numDocuments], persons[i]);
                        list->numDocuments++;
                    }
                }
            }
            aux = aux->next;
        }

        //remove repeated documents
        for (int i = 0; i < list->numDocuments; i++) {
            for (int j = i + 1; j < list->numDocuments; j++) {
                if (strcmp(list->documents[i], list->documents[j]) == 0) {
                    free(list->documents[j]);
                    for (int k = j; k < list->numDocuments - 1; k++) {
                        list->documents[k] = list->documents[k + 1];
                    }
                    list->numDocuments--;
                    list->documents = (char**)realloc(list->documents, list->numDocuments * sizeof(char*));
                    j--;
                }
            }
        }
    }
}

int temporalNode_countPersons(tTemporalNode* node) {
    int count = 0;
    tListDocuments list = {NULL, 0};
    if (node) {
        tCoordinateNode* aux = node->coordinateNode;
        while (aux) {
            char** persons = aux->persons;
            for (int i = 0; i < aux->numPersons; i++) {
                //check if the person is already in the list
                int found = 0;
                for (int j = 0; j < list.numDocuments; j++) {
                    if (strcmp(list.documents[j], persons[i]) == 0) {
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    if (list.numDocuments == 0) {
                        list.documents = (char**)malloc(sizeof(char*));
                    } else {
                        list.documents = (char**)realloc(list.documents, (list.numDocuments + 1) * sizeof(char*));
                    }
                    list.documents[list.numDocuments] = (char*)malloc((strlen(persons[i]) + 1) * sizeof(char));
                    strcpy(list.documents[list.numDocuments], persons[i]);
                    list.numDocuments++;
                }
            }
            aux = aux->next;   
        }
    }
    count = list.numDocuments;
    for (int i = 0; i < list.numDocuments; i++) {
        free(list.documents[i]);
    }
    free(list.documents);
    return count;
}

int temporalNode_cmpDate(tTemporalNode* node, tDateTime date) {
    if (node->date.date.year < date.date.year) {
        return -1;
    }
    if (node->date.date.year > date.date.year) {
        return 1;
    }
    // Check month
    if (node->date.date.month < date.date.month) {
        return -1;
    }
    if (node->date.date.month > date.date.month) {
        return 1;
    }  
    // Check day
    if (node->date.date.day < date.date.day) {
        return -1;
    }
    if (node->date.date.day > date.date.day) {
        return 1;
    }
    // Check hour
    if (node->date.time.hour < date.time.hour) {
        return -1;
    }
    if (node->date.time.hour > date.time.hour) {
        return 1;
    }
    // Check minutes
    if (node->date.time.minutes < date.time.minutes) {
        return -1;
    }
    if (node->date.time.minutes > date.time.minutes) {
        return 1;
    }
    
    return 0;
}

int temporalNode_count_nodesCoordinate(const tTemporalNode* node) {
    int count = 0;
    if (node) {
        tCoordinateNode* aux = node->coordinateNode;
        while (aux) {
            count++;
            aux = aux->next;
        }
    }
    return count;
}

void temporalNode_getCoordinates(tTemporalNode* node, tListCoordinates* list) {
    if (node) {
        tCoordinateNode* aux = node->coordinateNode;
        while (aux) {
            if (list->numCoordinates == 0) {
                list->coordinates = (tCoordinate*)malloc(sizeof(tCoordinate));
            } else {
                list->coordinates = (tCoordinate*)realloc(list->coordinates, (list->numCoordinates + 1) * sizeof(tCoordinate));
            }
            list->coordinates[list->numCoordinates] = aux->coordinate;
            list->numCoordinates++;
            aux = aux->next;
        }
    }
}

void temporalNode_getPersons(tTemporalNode* node, tListDocuments* list) {
    if (node) {
        tCoordinateNode* aux = node->coordinateNode;
        while (aux) {
            char** persons = aux->persons;
            for (int i = 0; i < aux->numPersons; i++) {
                if (list->numDocuments == 0) {
                    list->documents = (char**)malloc(sizeof(char*));
                } else {
                    list->documents = (char**)realloc(list->documents, (list->numDocuments + 1) * sizeof(char*));
                }
                list->documents[list->numDocuments] = (char*)malloc((strlen(persons[i]) + 1) * sizeof(char));
                strcpy(list->documents[list->numDocuments], persons[i]);
                list->numDocuments++;
            }
            aux = aux->next;
        }
    }
}
