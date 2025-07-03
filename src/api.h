# ifndef __DEVOPS_API_H__
# define __DEVOPS_API_H__

# include <string.h>
# include "csv.h"
# include "error.h"
# include "person.h"
# include "temporal_node.h"
# include "date.h"

typedef struct _ApiData {
    tPopulation population;
    tTemporalNode* temporal_node;
    int numNodes;    
} tApiData;

typedef struct _CoordinateArray {

    struct CoordinateElem {
        float latitude;
        float longitude;    
    };
    
    struct CoordinateElem* coordinates;
    int count; 

} tCoordinateArray;

const char* api_version();

tApiError api_init(tApiData* data);
tApiError api_loadData(tApiData* data, const char* filename, bool reset);
tApiError api_add_person_geolocation(tApiData* data, tDateTime date, const char* document, tCoordinate coordinate);
tApiError api_remove_person(tApiData* data, const char* document);
tApiError api_remove_geodata(tApiData* data, tDateTime ini, tDateTime end);
//tApiError api_count_persons(const tApiData* data, tDateTime init, tDateTime end, int* count);
tApiError api_persons_in_contact(const tApiData* data, tDateTime ini, tDateTime end, const char* document, tPopulation* population);
tApiError api_get_dates(const tApiData* data, tDateTime* min, tDateTime* max);
tApiError api_get_coordinates_in_date(const tApiData* data, tDateTime date, tCoordinateArray* coordinates);
tApiError api_get_located_persons(const tApiData* data, tDateTime ini, tDateTime end, tPopulation* population);

void api_free(tApiData* data);

# endif

