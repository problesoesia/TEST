# ifndef __COORDINATE_NODE_H__
# define __COORDINATE_NODE_H__

typedef struct _tCoordinate {
    float latitude;
    float longitude;    
} tCoordinate;

typedef struct _tCoordinateNode {    
    tCoordinate coordinate;
    int numPersons;
    char** persons;
    struct _tCoordinateNode* next;
} tCoordinateNode;

void coordinateNode_init(tCoordinateNode* node);
void coordinateNode_free(tCoordinateNode* node);

void coordinateNode_addPerson(tCoordinateNode* node, const char* person);
void coordinateNode_removePerson(tCoordinateNode* node, const char* person);
int coordinateNode_findPerson(tCoordinateNode* node, const char* person);
int coordinateNode_countPersons(tCoordinateNode* node);
int coordinateNode_hasCoordinate(tCoordinateNode* node, tCoordinate coordinate);
void coordinateNode_print(const tCoordinateNode* node);

# endif