#ifndef DEF_H
#define DEF_H


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "math.h"
#ifndef M_PI
#define M_PI 3.1415926535
#endif


typedef struct ListEntryStruct {
	void* value;
	struct ListEntryStruct* next;
	struct ListEntryStruct* prev;
} ListEntry;

typedef struct {
	ListEntry* first;
	int size;
} List;

/** Variable length string structure */
typedef struct {
	char* string;
	size_t len;
	size_t alloc;
} String;

void* ListGet(List* l, int index);
int ListFind(List* l, void* value);
int ListSize(List* l);
void* ListAdd(List* l, void* value);
ListEntry* ListInsert(List* l, void* value, int index);
void ListRemove(List* l, int index);
void ListBubbleSort(List* l, int (*comparator)(void* a, void* b));
String StringNew();
String StringNewl(size_t size);
String StringNews(const char* src);
String StringNewsl(const char* src, size_t len);
void StringAppends(String* str, const char* s);
void StringAppendsl(String* str, const char* s, size_t len);
void StringAppendc(String* str, const char c);
void StringFree(String* str);


#endif // DEF_H
