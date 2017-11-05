
#ifndef _POLYGONS_H
#define _POLYGONS_H


#define FALSE 0
#define TRUE 1
#define AMOUNT_OF_DIGITS_IN_INPUT 64

#define TRIANGLE 0
#define SQUARE 1

#define ACTION_BITS 7
// functions
#define AMOUNT_OF_FUNCTIONS 6
#define ADD_POLY_FUNC 0
#define PERIMETER_FUNC 1
#define AREA_FUNC 2
#define PRINT_POLY_FUNC 3
#define DO_CURRENT_FUNC 4
#define DO_ALL_FUNC_FUNC 5

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


void initFucntionPointerArray();
BOOL createPolygonFromInput(LLU input);
int getAndRemoveNextBit(LLU* input);
int getBitAt(LLU input, int bitIndex);

void add_polygon(long long unsigned);
// add new polygon to the list

void perimeter(long long unsigned);
// calculate and print the perimeter

void area(long long unsigned);
// calculate and print the area
void print_polygon(long long unsigned);
// print the type of polygon and its vertices
void do_current(long long unsigned);
// do the operations on the current polygon.
void do_all(long long unsigned);
// do the operations in the parameter on the list



#endif // !_POLYGONS_H


