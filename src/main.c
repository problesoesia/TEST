#include <stdio.h>
#include <stdlib.h>
#include "api.h"
#include "person.h"
#include "error.h"

tApiError parse_add(const char* text, tApiData* apiData) {
    int year, month, day, hour, minute;
    char document[8];
    float latitude, longitude;
    
    int result = sscanf(text, "%d/%d/%d %d:%d %s %f;%f", &day, &month, &year, &hour, &minute, document, &latitude, &longitude);
    if (result != 8) {
        return E_INVALID_ENTRY_FORMAT;
    }
    
    tDate date = {year, month, day};
    tTime time = {hour, minute};
    tDateTime dateTime = {date, time};
    tCoordinate coordinate = {latitude, longitude};
    return api_add_person_geolocation(apiData, dateTime, document, coordinate);
}

tApiError parse_contact(const char* text, tApiData* apiData, tPopulation* population) {
    int year1, month1, day1, hour1, minute1;
    int year2, month2, day2, hour2, minute2;
    char document[8];
    
    int result = sscanf(text, "%d/%d/%d %d:%d %d/%d/%d %d:%d %s", &day1, &month1, &year1, &hour1, &minute1, &day2, &month2, &year2, &hour2, &minute2, document);
    if (result != 11) {
        return E_INVALID_ENTRY_FORMAT;
    }
    
    tDate date1 = {year1, month1, day1};
    tTime time1 = {hour1, minute1};
    tDateTime dateTime1 = {date1, time1};
    
    tDate date2 = {year2, month2, day2};
    tTime time2 = {hour2, minute2};
    tDateTime dateTime2 = {date2, time2};
    
    return api_persons_in_contact(apiData, dateTime1, dateTime2, document, population);
}

const char* parse_error(tApiError error) {
    switch (error) {
        case E_INVALID_ENTRY_FORMAT:
            return "Invalid entry format\n";
        case E_MEMORY_ERROR:
            return "Memory error\n";
        case E_DUPLICATED_ENTRY:
            return "Duplicated entry\n";
        case E_INVALID_ENTRY_TYPE:
            return "Invalid entry type\n";
        case E_PERSON_NOT_FOUND:
            return "Person not found\n";
        case E_FILE_NOT_FOUND:
            return "File not found\n";
        case E_NOT_IMPLEMENTED:
            return "Not implemented\n";
        case E_DATE_NOT_FOUND:
            return "Date not found\n";
        default:
            return "Unknown error\n";
    }
}

int main(int argc, char **argv) {

    // check that the number of arguments is 1
    if (argc != 2) {
        printf("Usage: %s <csv_file>\n", argv[0]);
        return -1;
    }

    // read the csv file name from the arguments
    const char* filename = argv[1];

    printf("Initializing API\n");

    tApiData data;
    api_init(&data);

    tApiError result = api_loadData(&data, filename, false);
    if (result != E_SUCCESS) {
        perror(parse_error(result));
        return -1;
    }

    population_print(data.population);

    bool exit = false;
    while (!exit)  {
        char text[200];

        printf("Enter a command (add, dates, contact, exit): ");
        fgets(text, sizeof(text), stdin);
        text[strcspn(text, "\n")] = '\0';

        if (strcmp(text, "exit") == 0) {
            exit = true;
        } else if (strcmp(text, "add") == 0) {
            printf("Enter a date (dd/mm/yyyy hh:mm document lat;lon): ");
            fgets(text, sizeof(text), stdin);
            text[strcspn(text, "\n")] = '\0'; 

            result = parse_add(text, &data);
            if (result != E_SUCCESS) {
                perror(parse_error(result));
            } else {
                printf("Data added successfully\n");
            }
        } else if (strcmp(text, "contact") == 0) {
            printf("Enter start_date end_date and document (dd/mm/yyyy hh:mm dd/mm/yyyy hh:mm document):");
            fgets(text, sizeof(text), stdin);
            text[strcspn(text, "\n")] = '\0';

            tPopulation population;
            population_init(&population);

            result = parse_contact(text, &data, &population);
            if (result != E_SUCCESS) {
                perror(parse_error(result));
            } else {
                printf("Persons in contact:\n");
                population_print(population);
            }
        } else if (strcmp(text, "dates") == 0) {
            tDateTime min, max;
            result = api_get_dates(&data, &min, &max);
            if (result != E_SUCCESS) {
                perror(parse_error(result));
            } else {
                printf("Dates: %d/%d/%d %d:%d - %d/%d/%d %d:%d\n", min.date.day, min.date.month, min.date.year, min.time.hour, min.time.minutes, max.date.day, max.date.month, max.date.year, max.time.hour, max.time.minutes);
            } 
        } else {
            printf("Unknown command\n");
        }
    }

    api_free(&data);
    
    return 0;
}
