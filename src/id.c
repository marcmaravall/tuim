#include "id.h"

TuimID tuim_id_hash(const char* seed) {
    return tuim_fnv1a_64(seed);
}

TuimID tuim_fnv1a_64(const char* seed) {
    uint64_t hash = 1469598103934665603ULL;

    while (*seed) {
        hash ^= (unsigned char)*seed++;
        hash *= 1099511628211ULL;
    }

    return hash;
}
