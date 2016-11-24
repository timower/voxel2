#ifndef HANDLE_H
#define HANDLE_H

#include <cstdint>

struct Handle {
	uint32_t index : 16;
	uint32_t type : 6;
	uint32_t generation : 10;
};

const Handle INVALID_HNDL = { 0, INVALID, 0 };


#endif
