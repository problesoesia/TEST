# include "api.h"

const char* api_version() {
    return "1.0.0";
}

tApiError api_init(tApiData* data) {
    if (!data) {
        return E_MEMORY_ERROR;
    }

    population_init(&data->population);
    data->temporal_node = NULL;
    data->numNodes = 0;
    return E_SUCCESS;
}

void api_free(tApiData* data) {
    if (data) {
        population_free(&data->population);
        
        tTemporalNode* aux = data->temporal_node;
        while (aux) {
            tTemporalNode* next = aux->next;
            temporalNode_free(aux);
            free(aux);
            aux = next;
        }
        data->temporal_node = NULL;
        data->numNodes = 0;
    }
}

tApiError api_loadData(tApiData* data, const char* filename, bool reset) {
    if (!data) {
        return E_MEMORY_ERROR;
    }

    if (reset) {
        api_free(data);
        api_init(data);
    }

    tCSVFile* csv = (tCSVFile*) malloc(sizeof(tCSVFile));
    tApiError error = csv_read(csv, filename, ',');
    
    if (error == E_SUCCESS) {
    
        if (csv->header_count != 7) {
            perror("Error getting header\n");
            csv_free(csv);
            free(csv);
            return E_INVALID_ENTRY_FORMAT;
        }

        for (int i = 0; i < csv->row_count; i++) {
            
            tPerson person;
            person.document = csv_get_item_by_row_and_column_name(csv, i, "document");
            if (!person.document) {
                perror("Error getting document\n");
                csv_free(csv);
                free(csv);
                return E_INVALID_ENTRY_FORMAT;
            }

            person.name = csv_get_item_by_row_and_column_name(csv, i, "name");
            if (!person.name) {
                perror("Error getting name\n");
                csv_free(csv);
                free(csv);
                return E_INVALID_ENTRY_FORMAT;
            }

            person.surname = csv_get_item_by_row_and_column_name(csv, i, "surname");
            if (!person.surname) {
                perror("Error getting surname\n");
                csv_free(csv);
                free(csv);
                return E_INVALID_ENTRY_FORMAT;
            }

            person.cp = csv_get_item_by_row_and_column_name(csv, i, "cp");
            if (!person.cp) {
                perror("Error getting cp\n");
                csv_free(csv);
                free(csv);
                return E_INVALID_ENTRY_FORMAT;
            }

            person.email = csv_get_item_by_row_and_column_name(csv, i, "email");
            if (!person.email) {
                perror("Error getting email\n");
                csv_free(csv);
                free(csv);
                return E_INVALID_ENTRY_FORMAT;
            }

            person.address = csv_get_item_by_row_and_column_name(csv, i, "address");
            if (!person.address) {
                perror("Error getting address\n");
                csv_free(csv);
                free(csv);
                return E_INVALID_ENTRY_FORMAT;
            }

            char* birthday = csv_get_item_by_row_and_column_name(csv, i, "birthday");
            if (!birthday) {
                perror("Error getting birthday\n");
                csv_free(csv);
                free(csv);
                return E_INVALID_ENTRY_FORMAT;
            }
            sscanf(birthday, "%d/%d/%d", &(person.birthday.day), &(person.birthday.month), &(person.birthday.year));
        
            population_add(&data->population, person);
        }

        csv_free(csv);
        free(csv);
        return E_SUCCESS;
    } else {
        return error;
    }
}

tApiError api_add_person_geolocation(tApiData* data, tDateTime date, const char* document, tCoordinate coordinate) {

    if (data) {
        
        if (population_find(data->population, document) != -1) {

            // If the list is empty, create a new temporal node and add it to the linked list
            if (!data->temporal_node) {
                tTemporalNode* temporal_node = (tTemporalNode*) malloc(sizeof(tTemporalNode));
                if (temporal_node) {
                    temporalNode_init(temporal_node);
                    temporal_node->date = date;
                    temporalNode_addPersonCoordinate(temporal_node, coordinate, document);
                    data->temporal_node = temporal_node;
                    data->numNodes++;
                    return E_SUCCESS;
                } else {
                    return E_MEMORY_ERROR;
                }
            } else {
                // If the list is not empty, search for the date in the linked list
                tTemporalNode* aux = data->temporal_node;
                tTemporalNode* prev = NULL;
                while (aux && temporalNode_cmpDate(aux, date) <= 0) {
                    if (temporalNode_cmpDate(aux, date) == 0) {
                        temporalNode_addPersonCoordinate(aux, coordinate, document);
                        return E_SUCCESS;
                    }
                    prev = aux;
                    aux = aux->next;
                }

                // If the date is not found, create a new temporal node and add it between the previous and the next node
                tTemporalNode* temporal_node = (tTemporalNode*) malloc(sizeof(tTemporalNode));
                if (temporal_node) {
                    temporalNode_init(temporal_node);
                    temporal_node->date = date;
                    temporalNode_addPersonCoordinate(temporal_node, coordinate, document);
                    
                    data->numNodes++;
                    //check that the new node is the last one
                    if (!aux) {
                        prev->next = temporal_node;
                        return E_SUCCESS;
                    } else {
                        //check that the new node is the first one
                        if (!prev) {
                            temporal_node->next = data->temporal_node;
                            data->temporal_node = temporal_node;
                            return E_SUCCESS;
                        } else {
                            prev->next = temporal_node;
                            temporal_node->next = aux;
                            return E_SUCCESS;
                        }
                    }
                } else {
                    return E_MEMORY_ERROR;
                }
            }
        } else {
            return E_PERSON_NOT_FOUND;
        }
    } else {
        return E_MEMORY_ERROR;
    }
}

tApiError api_remove_person(tApiData* data, const char* document) {
    if (data) {
        tTemporalNode* aux = data->temporal_node;
        tTemporalNode* prev = NULL;

        while(aux) {
            temporalNode_removePerson(aux, document);
            if (aux->numCoordinates == 0) {
                data->numNodes--;

                if (data->numNodes == 0) {
                    temporalNode_free(aux);
                    free(aux);
                    data->temporal_node = NULL;
                    aux = NULL;
                } else {
                    if (prev) {
                        prev->next = aux->next;
                        temporalNode_free(aux);
                        free(aux);
                        aux = prev->next;
                    } else {
                        data->temporal_node = aux->next;
                        temporalNode_free(aux);
                        free(aux);
                        aux = data->temporal_node;
                    }
                }
            } else {
                prev = aux;
                aux = aux->next;
            }

        }
        return E_SUCCESS;
    } else {
        return E_MEMORY_ERROR;
    }
}

tApiError api_remove_geodata(tApiData* data, tDateTime ini, tDateTime end) {
    if (data) {
        tTemporalNode* aux = data->temporal_node;
        tTemporalNode* prev = NULL;
        while (aux)
        {
            if (dateTime_cmp(aux->date, ini) >= 0 && dateTime_cmp(aux->date, end) <= 0) {
                data->numNodes--;
                if (data->numNodes == 0) {
                    temporalNode_free(aux);
                    free(aux);
                    data->temporal_node = NULL;
                    aux = NULL;
                } else {
                    if (prev) {
                        prev->next = aux->next;
                        temporalNode_free(aux);
                        free(aux);
                        aux = prev->next;
                    } else {
                        data->temporal_node = aux->next;
                        temporalNode_free(aux);
                        free(aux);
                        aux = data->temporal_node;
                    }
                }
            } else {
                prev = aux;
                aux = aux->next;
            }
        }
        return E_SUCCESS;
    } else {
        return E_MEMORY_ERROR;
    }
}

/*tApiError api_count_persons(const tApiData* data, tDateTime init, tDateTime end, int* count) {
    if (data) {
        tTemporalNode* aux = data->temporal_node;
        *count = 0;
        while (aux) {
            if (temporalNode_cmpDate(aux, init) >= 0 && temporalNode_cmpDate(aux, end) <= 0) {
                *count += temporalNode_countPersons(aux);
            }
            aux = aux->next;
        }
        return E_SUCCESS;
    } else {
        return E_MEMORY_ERROR;
    }
}*/

tApiError api_persons_in_contact(const tApiData* data, tDateTime ini, tDateTime end, const char* document, tPopulation* population) {
    if (data && document && population) {
        
        if (population_find(data->population, document) == -1) {
            return E_PERSON_NOT_FOUND;
        }

        tTemporalNode* aux = data->temporal_node;
        while (aux) {
            if (temporalNode_cmpDate(aux, ini) >= 0 && temporalNode_cmpDate(aux, end) <= 0) {
                tListDocuments list = {NULL, 0};
                temporalNode_findPersonsContatcs(aux, document, &list);
                for (int i = 0; i < list.numDocuments; i++) {
                    if (population_find(*population, list.documents[i]) == -1) {
                        int pos = population_find(data->population, list.documents[i]);
                        if (pos != -1) {
                            population_add(population, data->population.elems[pos]);
                        } else {
                            population_free(population);
                            return E_PERSON_NOT_FOUND;
                        }
                    }
                    free(list.documents[i]);
                }
                free(list.documents);
            }
            aux = aux->next;
        }
        return E_SUCCESS;
    } else {
        return E_MEMORY_ERROR;
    }
}

tApiError api_get_dates(const tApiData* data, tDateTime* min, tDateTime* max) {
    if (data && min && max) {
        tTemporalNode* aux = data->temporal_node;
        
        tDate min_date = {31, 12, 9999};
        tTime min_time = {23, 59};
        tDateTime min_aux = {min_date, min_time};
        
        tDate max_date = {1, 1, 0};
        tTime max_time = {0, 0};
        tDateTime max_aux = {max_date, max_time};
        
        while (aux) {
            if (dateTime_cmp(aux->date, min_aux) < 0) {
                min_aux = aux->date;
            }
            if (dateTime_cmp(aux->date, max_aux) > 0) {
                max_aux = aux->date;
            }
            aux = aux->next;
        }

        min->date = min_aux.date;
        min->time = min_aux.time;
        max->date = max_aux.date;
        max->time = max_aux.time;
        
        return E_SUCCESS;
    } else {
        return E_MEMORY_ERROR;
    }
}

tApiError api_get_coordinates_in_date(const tApiData* data, tDateTime date, tCoordinateArray* coordinates) {
    if (data) {
        // Find the node with the date
        tTemporalNode* aux = data->temporal_node;
        while (aux && dateTime_cmp(aux->date, date) != 0) {
            aux = aux->next;
        }

        // If the node is found, get the coordinates
        if (aux) {
            coordinates->count = 0;
            coordinates->coordinates = NULL;
            tCoordinateNode* coord_aux = aux->coordinateNode;
            while (coord_aux) {
                if (coordinates->count == 0) {
                    coordinates->count ++;
                    coordinates->coordinates = (struct CoordinateElem*) malloc(sizeof(struct CoordinateElem));
                    coordinates->coordinates[0].latitude = coord_aux->coordinate.latitude;
                    coordinates->coordinates[0].longitude = coord_aux->coordinate.longitude; 

                } else {
                    coordinates->count ++;
                    coordinates->coordinates = (struct CoordinateElem*) realloc(coordinates->coordinates, sizeof(struct CoordinateElem) * coordinates->count);
                    coordinates->coordinates[coordinates->count - 1].latitude = coord_aux->coordinate.latitude;
                    coordinates->coordinates[coordinates->count - 1].longitude = coord_aux->coordinate.longitude;
                }
                coord_aux = coord_aux->next;
            }
            return E_SUCCESS;
        } else {
            return E_DATE_NOT_FOUND;
        }
    } else {
        return E_MEMORY_ERROR;
    }
}

tApiError api_get_located_persons(const tApiData* data, tDateTime ini, tDateTime end, tPopulation* population) {
    if (data) {
        population_init(population);
        tTemporalNode* aux = data->temporal_node;
        while (aux) {
            if (temporalNode_cmpDate(aux, ini) >= 0 && temporalNode_cmpDate(aux, end) <= 0) {
                tCoordinateNode* coord_aux = aux->coordinateNode;
                while (coord_aux) {
                    for (int i = 0; i < coord_aux->numPersons; i++) {
                        if (population_find(*population, coord_aux->persons[i]) == -1) {
                            int pos = population_find(data->population, coord_aux->persons[i]);
                            if (pos != -1) {
                                population_add(population, data->population.elems[pos]);
                            } else {
                                population_free(population);
                                return E_PERSON_NOT_FOUND;
                            }
                        }
                    }
                    coord_aux = coord_aux->next;
                }
            }
            aux = aux->next;
        } 
    } else {
        return E_MEMORY_ERROR;
    }
}