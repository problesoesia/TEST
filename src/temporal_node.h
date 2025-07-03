# ifndef __TEMPORAL_NODE_H__
# define __TEMPORAL_NODE_H__

#include "coordinate_node.h"
#include "date.h"

typedef struct _tTemporalNode {
    tCoordinateNode* coordinateNode;
    tDateTime date;
    int numCoordinates;
    struct _tTemporalNode* next;
} tTemporalNode;

typedef struct _tListDocuments {
    char** documents;
    int numDocuments;
} tListDocuments;

typedef struct _tListCoordinates
{
    tCoordinate* coordinates;
    int numCoordinates;
} tListCoordinates;
 
void temporalNode_init(tTemporalNode* node);
void temporalNode_free(tTemporalNode* node);

void temporalNode_addPersonCoordinate(tTemporalNode* node, tCoordinate coordinate, const char* person);
void temporalNode_removePerson(tTemporalNode* node, const char* person);
void temporalNode_findPersonsContatcs(tTemporalNode* node, const char* person, tListDocuments* list);
int temporalNode_countPersons(tTemporalNode* node);
int temporalNode_cmpDate(tTemporalNode* node, tDateTime date);
int temporalNode_count_nodesCoordinate(const tTemporalNode* node);
void temporalNode_getCoordinates(tTemporalNode* node, tListCoordinates* list);
void temporalNode_getPersons(tTemporalNode* node, tListDocuments* list);

# endif