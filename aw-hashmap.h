
#ifndef AW_HASHMAP_H
#define AW_HASHMAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

void hashmap_init(unsigned *keys, size_t cnt);
int hashmap_add(unsigned *keys, size_t cnt, unsigned key);
int hashmap_remove(unsigned *keys, size_t cnt, unsigned key);
int hashmap_find(unsigned *keys, size_t cnt, unsigned key);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* AW_HASHMAP_H */

