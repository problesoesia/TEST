#ifndef __CDEVOPS_ERROR__H
#define __CDEVOPS_ERROR__H

// Define error codes
enum _tApiError
{
    E_SUCCESS = 0, // No error
    E_NOT_IMPLEMENTED = -1, // Called method is not implemented
    E_FILE_NOT_FOUND = -2, // File not found
    E_PERSON_NOT_FOUND = -3, // Person not found
    E_DATE_NOT_FOUND = -8, // Person not found
    E_INVALID_ENTRY_TYPE = -4, // Invalid entry type
    E_INVALID_ENTRY_FORMAT = -5, // Invalid entry format
    E_DUPLICATED_ENTRY = -6, // Duplicated entry
    E_MEMORY_ERROR = -7, // Memory error
};

// Define an error type
typedef enum _tApiError tApiError;

#endif 