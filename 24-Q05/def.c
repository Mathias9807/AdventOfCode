#include "def.h"


void* ListGet(List* l, int index) {
	ListEntry* cur = l->first;
	int i = 0;
	
	if (index < 0) return NULL;
	
	while (cur) {
		if (i == index) return cur->value;
		
		i++;
		cur = cur->next;
	}
	
	return NULL;
}

int ListFind(List* l, void* value) {
	ListEntry* cur = l->first;
	
	int index = 0;

	while (cur) {
		if (cur->value == value) return index;
		
		index++;
		cur = cur->next;
	}

	return -1;
}

int ListSize(List* l) {
	ListEntry* cur = l->first;
	int i = 0;
	
	while (cur) {
		cur = cur->next;
		i++;
	}
	
	l->size = i;
	
	return i;
}

void* ListAdd(List* l, void* value) {
	ListEntry* cur = l->first;
	if (cur) {
		while (cur->next) 
			cur = cur->next;
		cur->next = malloc(sizeof(ListEntry));
		cur->next->value = value;
		cur->next->prev = cur;
		cur->next->next = NULL;
	}else {
		l->first = malloc(sizeof(ListEntry));
		l->first->value = value;
		l->first->prev = l->first->next = NULL;
	}
	
	l->size++;

	return value;
}

ListEntry* ListInsert(List* l, void* value, int index) {
	ListEntry* cur = l->first;
	ListEntry* newEntry = NULL;
	if (cur) {
		int curIndex = 0;
		while (curIndex++ != index && cur->next) 
			cur = cur->next;
		ListEntry* next = cur->next;
		cur->next = malloc(sizeof(ListEntry));
		cur->next->value = value;
		cur->next->prev = cur;
		cur->next->next = next;
		if (next) next->prev = cur->next;
		newEntry = cur->next;
	}else {
		l->first = malloc(sizeof(ListEntry));
		l->first->value = value;
		l->first->prev = l->first->next = NULL;
		newEntry = l->first;
	}
	
	l->size++;

	return newEntry;
}

void ListRemove(List* l, int index) {
	ListEntry* cur = l->first;
	int i = 0;
	
	if (index < 0) return;
	
	while (cur && i < index) {
		cur = cur->next;
		i++;
	}
	
	if (!cur) return;
	
	if (i == 0) {
		l->first = cur->next;
		l->first->prev = NULL;
	}
	if (cur->prev)	cur->prev->next = cur->next;
	if (cur->next)	cur->next->prev = cur->prev;
	free(cur);
	
	l->size--;
}

void ListBubbleSort(List* l, int (*comparator)(void* a, void* b)) {
	for (int i = 1; i < l->size; i++) {
		ListEntry* prev = l->first;
		ListEntry* cur = prev->next;
		bool changed = false;
		
		for (int j = i; j < l->size; j++) {
			if (!cur) break;
			
			int comp = comparator(prev->value, cur->value);
			if (comp < 0) {
				
				// Byt cur och prev
				prev->next = cur->next;
				if (prev->next)
					prev->next->prev = prev;
				if (l->first == prev) {
					l->first = cur;
					cur->prev = NULL;
				}else {
					prev->prev->next = cur;
					cur->prev = prev->prev;
				}
				cur->next = prev;
				prev->prev = cur;
				
				ListEntry* tmp = cur;
				cur = prev;
				prev = tmp;
				
				changed = true;
			}
			
			prev = prev->next;
			cur = cur->next;
		}
		
		if (!changed) break;
	}
}

String StringNew(const char* src) {
	String str;
	str.string = calloc(1, 64);
	str.len = 0;
	str.alloc = 64;
	return str;
}
String StringNewl(size_t size) {
	size_t nextPower = pow(2, ceil(log(size+1)/log(2)));
	String str;
	str.string = calloc(1, nextPower);
	str.len = 0;
	str.alloc = nextPower;
	return str;
}
String StringNews(const char* src) {
	size_t len = 0;
	while (src[len]) len++;
	return StringNewsl(src, len);
}
String StringNewsl(const char* src, size_t len) {
	size_t nextPower = pow(2, ceil(log(len+1)/log(2)));

	String str;
	str.string = calloc(1, nextPower);
	memcpy(str.string, src, len);
	str.len = len;
	str.alloc = nextPower;
	return str;
}

void StringFree(String* str) {
	free(str->string);
	str->string = NULL;
	str->alloc = str->len = 0;
}

void StringIncreaseSize(String* str, size_t newSize) {
	if (newSize + 1 <= str->alloc) return;

	size_t nextPower = pow(2, ceil(log(newSize+1)/log(2)));
	char* newStr = calloc(1, nextPower);
	memcpy(newStr, str->string, str->len);
	free(str->string);
	str->string = newStr;
	str->alloc = nextPower;
}

void StringAppends(String* str, const char* src) {
	size_t len = 0;
	while (src[len]) len++;
	return StringAppendsl(str, src, len);
}
void StringAppendsl(String* str, const char* src, size_t len) {
	StringIncreaseSize(str, str->len + len);

	memcpy(str->string + str->len, src, len);
	str->len += len;
}

void StringAppendc(String* str, const char c) {
	StringIncreaseSize(str, str->len + 1);

	str->string[str->len++] = c;
}

