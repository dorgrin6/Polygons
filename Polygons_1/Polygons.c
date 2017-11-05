#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "Polygons.h"

struct polygonsList polygons;
void(*functionPointerArray[AMOUNT_OF_FUNCTIONS])(LLU);


void main() {
	BOOL continueInput = TRUE;
	LLU input;

	initFucntionPointerArray();

	while (continueInput) {
		scanf("%llx", &input);
		printf("the number is: %llx\n", input);
		
		continueInput = createPolygonFromInput(input);
	}
}

void initFucntionPointerArray() {
	functionPointerArray[ADD_POLY_FUNC] = add_polygon;
	functionPointerArray[PERIMETER_FUNC] = perimeter;
	functionPointerArray[AREA_FUNC] = area;
	functionPointerArray[PRINT_POLY_FUNC] = print_polygon;
	functionPointerArray[DO_CURRENT_FUNC] = do_current;
	functionPointerArray[DO_ALL_FUNC_FUNC] = do_all;
}

int getBitAt(LLU input, int bitIndex) {
	int result = input & ((LLU)1 << bitIndex); // a mask for getting the k'th bit
	result >>= bitIndex;
	return result;
}

int getAndRemoveNextBit(LLU* input) {
	int result = *input & 1;
	*input <<= 1;
	return result;
}

LIST_NODE* createListNode(LLU input) {
	LIST_NODE *result = (LIST_NODE *)malloc(sizeof(LIST_NODE *));
	//TODO debug
	printf("input: %llx", input);

	//get vertices
	LLU polygon = input >> ACTION_BITS;
	printf("1.polygon: %llx", input);

	// get and add type
	polygon <<= 1;
	printf("2.polygon: %llx", input);

	polygon |= getBitAt(input, 2);
	
	printf("3.polygon: %llx", input);

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

void perimeter(long long unsigned input)
{
}

void area(long long unsigned input)
{
}

void print_polygon(long long unsigned input)
{
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
	int bitCounter = AMOUNT_OF_DIGITS_IN_INPUT - 1; // start bitcounter on 63rd bit

	int nextBit = getBitAt(input, 1);

	if (getBitAt(input, 1) != 0) {
		functionPointerArray[ADD_POLY_FUNC](input);
	}

	return isLastCommand;
}

