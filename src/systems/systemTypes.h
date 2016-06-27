#ifndef SYSTEM_TYPES_H
#define SYSTEM_TYPES_H

#define INVALID_HNDL (Handle){0, INVALID, 0}

enum SystemTypes {
	ENTITY = 0,
	TRANSFORM,
	GRAPHICS,
	CAMERA,
	PHYSICS,
	INPUT,
	CHUNK,

	INVALID
};

#endif
