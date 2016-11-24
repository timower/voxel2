#ifndef CONTAINER_H
#define CONTAINER_H

#include <cassert>
#include <cstddef>

#include "handle.h"

// TODO: use generation to assert!

template<typename T, unsigned int N, unsigned int S>
struct Container {
	uint16_t size;
	T data[N];
	Handle handles[N];

 	uint16_t nextFree;

	void init();
	Handle add();
	void remove(Handle handle);
	T& get(Handle handle);
};

template<typename T, unsigned int N, unsigned int S>
void Container<T, N, S>::init() {
	this->size = 0;
	this->nextFree = static_cast<uint16_t>(-1);
}

template<typename T, unsigned int N, unsigned int S>
Handle Container<T, N, S>::add() {
	assert(this->size < N);
	uint16_t idx = this->size++;
	Handle ret;
	ret.type = S;

	if (this->nextFree == static_cast<uint16_t>(-1)) {
		ret.index = idx;
	} else {
		ret.index = this->nextFree;
		this->nextFree = this->handles[ret.index].index;
	}

	this->handles[ret.index].index = idx;
	this->data[idx].handle = ret;
	return ret;
}

template<typename T, unsigned int N, unsigned int S>
void Container<T, N, S>::remove(Handle handle) {
	assert(handle.type == S);
	assert(handle.index < N);
	uint16_t idx = this->handles[handle.index].index;
	assert(idx < this->size);

	this->size -= 1;
	this->data[idx] = this->data[this->size];
	this->handles[this->data[idx].handle.index].index = idx; // TODO: increase generation.

	this->handles[handle.index].index = this->nextFree;
	this->nextFree = handle.index;
}

template<typename T, unsigned int N, unsigned int S>
T& Container<T, N, S>::get(Handle handle) {
	assert(handle.type == S);
	assert(handle.index < N);
	uint16_t idx = this->handles[handle.index].index;
	assert(idx < this->size); // TODO: what if fail???
	return this->data[idx];
}

#endif
