#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "person.h"


// Initialize the population data
void population_init(tPopulation* data) {
    // Check input/output data
    assert(data != NULL);
    
    data->elems = NULL;
    data->count = 0;
}

// Remove the data from a person
void person_free(tPerson* data) {
    // Check input data
    assert(data != NULL);
    
    // Release document data
    if(data->document != NULL) free(data->document);
    data->document = NULL;
    
    // Release name data
    if(data->name != NULL) free(data->name);
    data->name = NULL;
    
    // Release surname data
    if(data->surname != NULL) free(data->surname);
    data->surname = NULL;
    
    // Release email data
    if(data->email != NULL) free(data->email);
    data->email = NULL;
    
    // Release address data
    if(data->address != NULL) free(data->address);
    data->address = NULL;
    
    // Release cp data
    if(data->cp != NULL) free(data->cp);
    data->cp = NULL;
}

// Remove the data from all persons
void population_free(tPopulation* data) {
    int i;
    
    // Check input data
    assert(data != NULL);
    
    // Remove contents
    for(i = 0; i < data->count; i++) {
        person_free(&(data->elems[i]));
    }    
    
    // Release memory
    if (data->count > 0) {
        free(data->elems);
        data->elems = NULL;
        data->count = 0;
    }
}

// Add a new person
void population_add(tPopulation* data, tPerson person) {
    // Check input data
    assert(data != NULL);
    
    // If person does not exist add it
    if(population_find(*data, person.document) < 0) {   
        // Allocate memory for new element
        if (data->count == 0) {
            // Request new memory space
            data->elems = (tPerson*) malloc(sizeof(tPerson));            
        } else {
            // Modify currently allocated memory
            data->elems = (tPerson*) realloc(data->elems, (data->count + 1) * sizeof(tPerson));            
        }
        assert(data->elems != NULL);
                
        // Copy the data to the new position
        person_cpy(&(data->elems[data->count]), person);
        
        // Increase the number of elements
        data->count ++;
    }
}

// Remove a person
void population_del(tPopulation* data, const char *document) {
    int i;
    int pos;
    
    // Check input data
    assert(data != NULL);
    
    // Find if it exists
    pos = population_find(*data, document);
    
    if (pos >= 0) {
        // Remove current position memory
        person_free(&(data->elems[pos]));
        // Shift elements 
        for(i = pos; i < data->count-1; i++) {
            // Copy address of element on position i+1 to position i
            data->elems[i] = data->elems[i+1];
        }
        // Update the number of elements
        data->count--;
        // Resize the used memory
        if (data->count == 0) {
            // No element remaining
            free(data->elems);
            data->elems = NULL;
        } else {
            // Still some elements are remaining
            data->elems = (tPerson*)realloc(data->elems, data->count * sizeof(tPerson));
        }
    }
}

// Return the position of a person with provided document. -1 if it does not exist
int population_find(tPopulation data, const char* document) {
    int i;
    
    for(i = 0; i < data.count; i++) {
        if(strcmp(data.elems[i].document, document) == 0 ) {
            return i;
        }
    }
    
    return -1;
}

// Print the person data
void population_print(tPopulation data) {
    int i;
    
    for(i = 0; i < data.count; i++) {
        // Print position and document
        printf("%d;%s;", i, data.elems[i].document);
        // Print name and surname
        printf("%s;%s;", data.elems[i].name, data.elems[i].surname);        
        // Print email
        printf("%s;", data.elems[i].email);
        // Print address and CP
        printf("%s;%s;", data.elems[i].address, data.elems[i].cp);
        // Print birthday date
        printf("%02d/%02d/%04d\n", data.elems[i].birthday.day, data.elems[i].birthday.month, data.elems[i].birthday.year);
    }
}

// Copy the data from the source to destination
void person_cpy(tPerson* destination, tPerson source) {
    // Copy identity document data
    destination->document = (char*) malloc((strlen(source.document) + 1) * sizeof(char));
    assert(destination->document != NULL);
    strcpy(destination->document, source.document);
    
    // Copy name data
    destination->name = (char*) malloc((strlen(source.name) + 1) * sizeof(char));
    assert(destination->name != NULL);
    strcpy(destination->name, source.name);
    
    // Copy surname data
    destination->surname = (char*) malloc((strlen(source.surname) + 1) * sizeof(char));
    assert(destination->surname != NULL);
    strcpy(destination->surname, source.surname);
    
    // Copy email data
    destination->email = (char*) malloc((strlen(source.email) + 1) * sizeof(char));
    assert(destination->email != NULL);
    strcpy(destination->email, source.email);
    
    // Copy address data
    destination->address = (char*) malloc((strlen(source.address) + 1) * sizeof(char));
    assert(destination->address != NULL);
    strcpy(destination->address, source.address);
    
    // Copy cp data
    destination->cp = (char*) malloc((strlen(source.cp) + 1) * sizeof(char));
    assert(destination->cp != NULL);
    strcpy(destination->cp, source.cp);
    
    // Copy the birthday date
    destination->birthday = source.birthday;
}