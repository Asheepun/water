const int ARRAY_DEFAULT_LENGTH = 16;
const int ARRAY_DEFAULT_LENGTH_INCREMENT = 16;

template<typename T>
struct Array{

	T *elements;
	int length;
	int maxLength;

	T& operator[] (int i){
		return elements[i];
	}

	void init(){
		this->length = 0;
		this->maxLength = ARRAY_DEFAULT_LENGTH;
		this->elements = (T *)malloc(this->maxLength * sizeof(T));
	}

	void freeMemory(){
		free(this->elements);
	}

	void push(T element){

		if(this->length == this->maxLength){
			this->maxLength += ARRAY_DEFAULT_LENGTH_INCREMENT;
			this->elements = (T *)realloc(this->elements, this->maxLength * sizeof(T));
		}

		this->elements[this->length] = element;
		this->length++;


	}

	void remove(int index){
		if(this->length > 0){
			memcpy(this->elements + index, this->elements + index + 1, (this->length - index - 1) * sizeof(T));
			this->length--;
		}
	}

	void clear(){
		this->length = 0;
	}

};
