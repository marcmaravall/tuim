#ifndef TUIM_ID_H
#define TUIM_ID_H

#include <stdint.h>

typedef uint64_t TuimID;

#ifndef TUIM_STACK_ID_SIZE
#define TUIM_STACK_ID_SIZE 64
#endif // TUIM_STACK_ID_SIZE

typedef struct {
	TuimID stack[TUIM_STACK_ID_SIZE];
} TuimStackID;

TuimID tuim_id_hash (const char* seed);
TuimID tuim_fnv1a_64(const char* seed);

#endif // TUIM_ID_H