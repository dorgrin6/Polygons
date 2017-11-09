#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "Polygon.h"
#include <string.h>
#include <math.h>

struct polygonsList polygons;
void(*functionPointerArray[AMOUNT_OF_FUNCTIONS])(LLU);


void main() {
	BOOL continueInput = TRUE;
	LLU input;

	initFunctionPointerArray();

	while (continueInput) {
		scanf("%llx", &input);
		printf("the number is: %llx\n", input);

		continueInput = createPolygonFromInput(input);
		doPolygonActions(input);
		print_polygon(input);
		//perimeter(input);
		area(input);
	}
}

LLU getPolygonFromInput(LLU input) {
	//get vertices
	LLU polygon = input >> ACTION_BITS;
	//printf("1.polygon: %llx\n", polygon);

	// get and add type
	polygon <<= 1;
	//printf("2.polygon: %llx\n", polygon);

	polygon += ((input & 4) >> 2);

	//printf("3.polygon: %llx\n", polygon);

	return polygon;
}

void doPolygonActions(LLU input) {
	BOOL isPrintAction = (input & 0x4) != 0;
	BOOL isPerimeterAction = (input & 0x8) != 0;
	BOOL isAreaAction = (input & 0x10) != 0;

	LLU whoToMakeOnMask = (input & 0x60) >> 6;

	if (whoToMakeOnMask == CURRENT_POLYGON) {
		do_all(input);
	}
	else if (whoToMakeOnMask == ALL_TRIANGLES) {
		do_all(input);
	}
	else if (whoToMakeOnMask == ALL_SQUARES) {
		do_all(input);
	}
	else if (whoToMakeOnMask == ALL_POLYGONS) {
		do_all(input);
	}

	LIST_NODE* currentPolygon = polygons.head;
	while (currentPolygon != NULL) {
		if (isActionPoly(currentPolygon)) {
			if (isPrintAction) {
				print_polygon(currentPolygon->polygon);
			}
			if (isPerimeterAction) {
				perimeter(currentPolygon->polygon);
			}
			if (isAreaAction) {
				area(currentPolygon->polygon);
			}

		}
	}
}

LIST_NODE* createListNode(LLU input) {
	LIST_NODE *result = (LIST_NODE *)malloc(sizeof(LIST_NODE *));
	//TODO debug
	printf("input: %llx\n", input);

	LLU polygon = getPolygonFromInput(input);
	result->polygon = polygon;
	result->next = NULL;

	return result;
}



void add_polygon(long long unsigned input) {
	// list is empty
	if (polygons.head == NULL) {
		polygons.head = createListNode(input);
		polygons.tail = polygons.head;
	}
	// list isnt empty
	else {
		polygons.tail->next = createListNode(input);
		polygons.tail = polygons.tail->next;
	}
}

float getEdgeLengthFromVertexes(VERTEX *v1, VERTEX *v2) {
	int xDist = v1->x - v2->x;
	int yDist = v1->y - v2->y;
	return (float)sqrt(xDist * xDist + yDist * yDist); // sqrt((x1-x2)^2 + (y1-y2)^2)
}

float getEdgeLength(int firstBitIndex, LLU polygon) {
	VERTEX* v1 = getVertexFromPolygon(firstBitIndex, polygon);
	VERTEX* v2 = getVertexFromPolygon(firstBitIndex + 12, polygon);

	float result = getEdgeLengthFromVertexes(v1, v2);

	//free(v1);
	//free(v2);

	return result;
}


float getPerimeter(LLU input) {
	LLU polygon = getPolygonFromInput(input);
	int i;
	float result = 0;

	int edgeAmount = getNumberOfVertices(polygon);

	for (i = 0; i < edgeAmount; i++) {
		result += getEdgeLength(1 + i * 6 * 4, polygon);
	}

	return result;
}

void perimeter(long long unsigned input)
{
	printf("perimeter: %.1f\n", getPerimeter(input));
}

// using Heron's formula https://en.wikipedia.org/wiki/Heron's_formula
float getTriangleArea(LLU triangle) {
	float s = getPerimeter(triangle) / 2;
	float a = getEdgeLength(1, triangle);
	float b = getEdgeLength(1 + 6 * 2, triangle);
	float c = getEdgeLength(1 + 6 * 4, triangle);

	return (float)sqrt(s*(s - a)*(s - b)*(s - c));
}

void area(long long unsigned input)
{
	LLU polygon = getPolygonFromInput(input);

	printf("area: ");

	if (isSquarePolygon(polygon)) {
		printf("%.1f", getEdgeLength(1, polygon) * 4); // TODO check
	}
	else if (isTrianglePolygon(polygon)) {
		printf("%.1f", getTriangleArea(polygon));
	}

	printf("\n");
}

BOOL isSquarePolygon(LLU polygon) {
	return (polygon & SQUARE) != 0;
}

BOOL isTrianglePolygon(LLU polygon) {
	return !isSquarePolygon(polygon);
}

VERTEX* getVertexFromPolygon(int vertexFirstBit, LLU polygon) {
	VERTEX* result = (VERTEX*)malloc(sizeof(VERTEX*));
	LLU mask = 0x3F << vertexFirstBit; // we need 6 bits of the input i.e. 0011 1111
								   // create the vertex value and move it to start
	LLU maskedX = polygon & mask;
	maskedX >>= vertexFirstBit;

	LLU maskedY = polygon & (mask << VERTEX_AMOUNT_OF_BITS);
	maskedY >>= (vertexFirstBit + VERTEX_AMOUNT_OF_BITS);

	result->x = (int)maskedX;
	result->y = (int)maskedY;

	return result;
}

void printVertex(int vertexFirstBit, LLU polygon) {

	VERTEX* vertex = getVertexFromPolygon(vertexFirstBit, polygon);
	
	printf("{%d, %d} ", vertex->x, vertex->y);

	//free(vertex);
}

int getNumberOfVertices(LLU polygon) {
	if (isSquarePolygon(polygon)) {
		return 4;
	}
	else if (isTrianglePolygon(polygon)) {
		return 3;
	}
	else {
		return -1;
	}
}

void print_polygon(long long unsigned input)
{
	LLU polygon = getPolygonFromInput(input);
	int numberOfVertices = getNumberOfVertices(polygon);
	int i;

	if (isSquarePolygon(polygon)) {
		printf("Square ");
	}
	else if (isTrianglePolygon(polygon)) {
		printf("Triangle ");
	}
	else {
		printf("incompatible type in print_polygon");
	}

	for (i = 0; i < numberOfVertices; i++) {
		printVertex(1 + i * VERTEX_AMOUNT_OF_BITS * 2, polygon); // skip 2 vertices
	}

	printf("\n");
}

void do_current(long long unsigned input)
{
	
}

void do_all(long long unsigned input)
{

}

BOOL createPolygonFromInput(LLU input)
{
	BOOL isLastCommand = (BOOL)getBitAt(input, 0);
	//int bitCounter = AMOUNT_OF_DIGITS_IN_INPUT - 1; // start bitcounter on 63rd bit

	int nextBit = getBitAt(input, 1);

	if (getBitAt(input, 1) != 0) {
		functionPointerArray[ADD_POLY_FUNC](input);
	}

	return isLastCommand;
}


int getBitAt(LLU input, int bitIndex) {
	int result = input & ((LLU)1 << bitIndex); // a mask for getting the k'th bit
	result >>= bitIndex;
	return result;
}

/*
int getAndRemoveNextBit(LLU* input) {
	int result = *input & 1;
	*input <<= 1;
	return result;
}*/



void initFunctionPointerArray() {
	functionPointerArray[ADD_POLY_FUNC] = add_polygon;
	functionPointerArray[PERIMETER_FUNC] = perimeter;
	functionPointerArray[AREA_FUNC] = area;
	functionPointerArray[PRINT_POLY_FUNC] = print_polygon;
	functionPointerArray[DO_CURRENT_FUNC] = do_current;
	functionPointerArray[DO_ALL_FUNC_FUNC] = do_all;
}