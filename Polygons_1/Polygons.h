
#ifndef _POLYGONS_H
#define _POLYGONS_H
#define FALSE 0
#define TRUE 1
#define AMOUNT_OF_DIGITS_IN_INPUT 64

#define TRIANGLE 0
#define SQUARE 1

#define ACTION_BITS 8
// functions
#define AMOUNT_OF_FUNCTIONS 6
#define ADD_POLY_FUNC 0
#define PERIMETER_FUNC 1
#define AREA_FUNC 2
#define PRINT_POLY_FUNC 3
#define DO_CURRENT_FUNC 4
#define DO_ALL_FUNC_FUNC 5

// polygon bits
#define CURRENT_POLYGON 0
#define ALL_TRIANGLES 1
#define ALL_SQUARES 2
#define ALL_POLYGONS 3
#define VERTEX_AMOUNT_OF_BITS 6
#define TYPE_BIT 1

typedef unsigned char BYTE;
typedef unsigned char BOOL;
typedef long long unsigned LLU;

typedef struct listNode {
	long long unsigned polygon;
	struct listNode* next;
}LIST_NODE;

typedef struct polygonsList {
	struct listNode* head;
	struct listNode* tail;
}POLYGONS_LIST;

typedef struct actionsList {
	BOOL printAction;
	BOOL perimeterAction;
	BOOL areaAction;
}ACTIONS_LIST;

typedef struct vertex {
	char x;
	char y;
}VERTEX;



void add_polygon(long long unsigned);
float getEdgeLengthFromVertexes(VERTEX v1, VERTEX v2);
float getPerimeter(LLU polygon);
// add new polygon to the list
void perimeter(long long unsigned);
float getTriangleArea(LLU triangle);
float getSquareArea(LLU square);
// calculate and print the perimete
void area(long long unsigned);
BOOL isSquarePolygon(LLU polygon);
BOOL isTrianglePolygon(LLU polygon);
VERTEX getVertexFromPolygon(int vertexFirstBit, LLU polygon);
void printVertex(int vertexFirstBit, LLU polygon);
int getNumberOfVertices(LLU polygon);
// calculate and print the area
void print_polygon(long long unsigned);
int getBitAt(LLU input, int bitIndex);
void initFunctionPointerArray();
void parseInput(LLU input);
LLU getPolygonFromInput(LLU input);
ACTIONS_LIST getActionsList(LLU input);
// print the type of polygon and its vertices
void do_current(long long unsigned);
// do the operations on the current polygon.
void do_all(long long unsigned);
BOOL isRightPolygon(LLU polygon, LLU whoToMakeOnMask);
void doActionsOnPolygon(LIST_NODE * polygon, ACTIONS_LIST actionsList);
LIST_NODE * createListNode(LLU input);
// do the operations in the parameter on the list


#endif // !_POLYGONS_H
