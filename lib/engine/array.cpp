#include "engine/array.h"
/*
#include "engine/array.h"

template<typename T>
void Array_init(Array<T> *array_p){
	array_p->length = 0;
	array_p->maxLength = ARRAY_DEFAULT_LENGTH;
	array_p->elements = (T *)malloc(array_p->maxLength * sizeof(T));
}

template<typename T>
void Array_free(Array<T> *array_p){
	free(array_p->elements);
}

template<typename T>
void Array_push(Array<T> *array_p, T element){

	if(array_p->length == array_p->maxLength){
		array_p->maxLength += ARRAY_DEFAULT_LENGTH_INCREMENT;
		array_p->elements = (T *)realloc(array_p->elements, array_p->maxLength * sizeof(T));
	}

	array_p->elements[array_p->length] = element;
	array_p->length++;

}

template<typename T>
void Array_remove(Array<T> *array_p, int index){
	if(array_p->length > 0){
		memcpy(array_p->elements + index, array_p->elements + index + 1, array_p->length - index - 1);
		array_p->length--;
	}
}

template<typename T>
void Array_clear(Array<T> *array_p){
	array_p->length = 0;
}

*/
