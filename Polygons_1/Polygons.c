#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "Polygons.h"

struct polygonsList polygons;
void(*functionPointerArray[AMOUNT_OF_FUNCTIONS])(LLU);

void parseInput(LLU input) {
	LLU polygon = getPolygonFromInput(input);

	BOOL isInsert = input & 2; // 2nd bit

	if (isInsert) {
		add_polygon(polygon);
	}

	LLU whoToMakeOnMask = (input & 0x60) >> 6;

	if (whoToMakeOnMask == CURRENT_POLYGON) {
		do_current(input);
	}
	else {
		do_all(input);
	}
	//area(input);
}


LLU getPolygonFromInput(LLU input) {
	//get vertices
	LLU polygon = input >> ACTION_BITS;

	// get and add type
	polygon <<= 1;

	polygon += ((input & 4) >> 2);

	return polygon;
}

ACTIONS_LIST getActionsList(LLU input) {
	ACTIONS_LIST result;
	//printf("input on getActionsList = %llx", input);
	result.printAction = (input & 8) != 0;
	result.perimeterAction = (input & 16) != 0;
	result.areaAction = (input & 32) != 0;

	return result;
}

void do_current(long long unsigned input)
{
	ACTIONS_LIST actionsList = getActionsList(input);

	doActionsOnPolygon(polygons.tail, actionsList);
}
void do_all(long long unsigned input)
{
	ACTIONS_LIST actionsList = getActionsList(input);

	LLU whoToMakeOnMask = (input & 0x60) >> 6;

	LIST_NODE* currentPolygon = polygons.head;

	while (currentPolygon != NULL) {
		//doActionsOnPolygon(currentPolygon, isPrintAction, isPerimeterAction, isAreaAction);
		if (isRightPolygon(currentPolygon->polygon, whoToMakeOnMask)) {
			doActionsOnPolygon(currentPolygon, actionsList);
		}

		currentPolygon = currentPolygon->next;
	}
	
}

BOOL isRightPolygon(LLU polygon, LLU whoToMakeOnMask) {
	BOOL result = FALSE;

	if (whoToMakeOnMask == ALL_POLYGONS) {
		result = TRUE;
	}
	else if (whoToMakeOnMask == ALL_SQUARES && isSquarePolygon(polygon)) {
		result = TRUE;
	}
	else if (whoToMakeOnMask == ALL_TRIANGLES && isTrianglePolygon(polygon)) {
		result = TRUE;
	}

	return result;
}


void doActionsOnPolygon(LIST_NODE* polygon, ACTIONS_LIST actionsList) {
	if (actionsList.printAction) {
		print_polygon(polygon->polygon);
	}
	if (actionsList.perimeterAction) {
		perimeter(polygon->polygon);
	}
	if (actionsList.perimeterAction) {
		area(polygon->polygon);
	}
}

LIST_NODE* createListNode(LLU polygon) {
	LIST_NODE *result = (LIST_NODE *)malloc(sizeof(LIST_NODE));
	//TODO debug
	printf("input: %llx\n", polygon);

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

float getEdgeLengthFromVertexes(VERTEX v1, VERTEX v2) {
	int xDist = v1.x - v2.x;
	int yDist = v1.y - v2.y;
	return (float)sqrt(xDist * xDist + yDist * yDist); // sqrt((x1-x2)^2 + (y1-y2)^2)
}

float getEdgeLength(int vertexFirstIndex, LLU polygon) { 
	VERTEX v1 = getVertexFromPolygon(vertexFirstIndex, polygon);
	VERTEX v2 = getVertexFromPolygon(vertexFirstIndex + 12 ,polygon);

	return getEdgeLengthFromVertexes(v1, v2);
}

float getPerimeter(LLU polygon) {
	int i;
	float result = 0;
	int numberOfVertices = getNumberOfVertices(polygon);

	VERTEX* vertices = (VERTEX*)malloc(numberOfVertices * sizeof(VERTEX));
	
	// get vertices
	for (i = 0; i < numberOfVertices; i++) {
		vertices[i] = getVertexFromPolygon(1 + i * 12, polygon);
	}

	// get distance for all vertices and sum
	// round the shape
	for (i = 0; i < numberOfVertices - 1; i++) {
		result += getEdgeLengthFromVertexes(vertices[i], vertices[i+1]);
	}
	// first and last
	result += getEdgeLengthFromVertexes(vertices[0], vertices[numberOfVertices - 1]);

	free(vertices);

	return result;
}

void perimeter(long long unsigned polygon)
{
	printf("perimeter: %.1f\n", getPerimeter(polygon));
}

// using Heron's formula https://en.wikipedia.org/wiki/Heron's_formula
float getTriangleArea(LLU triangle) {
	int i;
	VERTEX vertices[3];
	for (i = 0; i < 3; i++) {
		vertices[i] = getVertexFromPolygon(1 + 12 * i, triangle);
	}

	float a = getEdgeLengthFromVertexes(vertices[0], vertices[1]);
	float b = getEdgeLengthFromVertexes(vertices[1], vertices[2]);
	float c = getEdgeLengthFromVertexes(vertices[2], vertices[0]);
	float s = (a + b + c) / 2;

	return (float)sqrt(s*(s - a)*(s - b)*(s - c));
}

float getSquareArea(LLU square) {
	return getTriangleArea(square) * 2;
}

void area(long long unsigned polygon)
{
	printf("area: ");

	if (isSquarePolygon(polygon)) {
		printf("%.1f", getSquareArea(polygon)); // TODO check
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

VERTEX getVertexFromPolygon(int vertexFirstBit, LLU polygon) {
	VERTEX result;
	LLU mask = 0x3F;					// we need 6 bits of the input i.e. 0011 1111
									   // create the vertex value and move it to start
	polygon >>= vertexFirstBit;
	result.x = (char) mask & polygon;
	result.x <<= 2;
	result.x /= 4;
	polygon >>= 6;
	result.y = (char) mask & polygon;
	result.y <<= 2;
	result.y /= 4;

	return result;
}

void printVertex(int vertexFirstBit, LLU polygon) {
	VERTEX vertex = getVertexFromPolygon(vertexFirstBit, polygon);

	printf("{%d, %d} ", vertex.x, vertex.y);
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

void print_polygon(long long unsigned polygon)
{
	int numberOfVertices = getNumberOfVertices(polygon);
	int i;

	isSquarePolygon(polygon) ? printf("Square ") : printf("Triangle ");

	for (i = 0; i < numberOfVertices; i++) {
		printVertex(1 + i * 12, polygon); // 
	}

	printf("\n");
}

int getBitAt(LLU input, int bitIndex) {
	int result = input & ((LLU)1 << bitIndex); // a mask for getting the k'th bit
	result >>= bitIndex;
	return result;
}

void initFunctionPointerArray() {
	functionPointerArray[ADD_POLY_FUNC] = add_polygon;
	functionPointerArray[PERIMETER_FUNC] = perimeter;
	functionPointerArray[AREA_FUNC] = area;
	functionPointerArray[PRINT_POLY_FUNC] = print_polygon;
	functionPointerArray[DO_CURRENT_FUNC] = do_current;
	functionPointerArray[DO_ALL_FUNC_FUNC] = do_all;
}

void main() {
	BOOL stopInput = FALSE;
	LLU input;

	initFunctionPointerArray();

	while (!stopInput) {
		scanf("%llx", &input);
		//printf("the number is: %llx\n", input);

		stopInput = (BOOL)getBitAt(input, 0);
		parseInput(input);
	}
}