
#include "aw-hashmap.h"
#include "aw-arith.h"
#include <string.h>

#if __GNUC__
# define _hashmap_likely(x) __builtin_expect(!!(x), 1)
# define _hashmap_unlikely(x) __builtin_expect(!!(x), 0)
#else
# define _hashmap_likely(x) (x)
# define _hashmap_unlikely(x) (x)
#endif

void hashmap_init(unsigned *key, size_t n) {
	memset(key, 0, sizeof (unsigned) * n);
}

int hashmap_add(unsigned *keys, size_t n, unsigned key) {
	unsigned h, i, k, a;
	int x;

	for (h = key % n, i = h, k = 1, a = 1; a < n;) {
		if (keys[i] == (unsigned) asr31(keys[i]) || keys[i] == key) {
			keys[i] = key;
			return (int) i;
		}

		i = h + a * k;
		x = sel_u32(i, i, -i) % n * sel_s32(i, 1, -1);
		i = sel_u32(x, x, n + x);
		i = sel_u32(i - n, i - n, i);
		k = (unsigned) (-(int) k + sel_u32(k, 0, 1));
		a = (unsigned) abs_s32((int) k);
	}

	return -1;
}

int hashmap_remove(unsigned *keys, size_t n, unsigned key) {
	int i;

	if ((i = hashmap_find(keys, n, key)) >= 0) {
		keys[i] = ~0u;
		return i;
	}

	return -1;
}

int hashmap_find(unsigned *keys, size_t n, unsigned key) {
	unsigned h, i, k, a;
	int x;

	for (h = key % n, i = h, k = 1, a = 1; a < n;) {
		if (_hashmap_unlikely(keys[i] == 0))
			break;

		if (keys[i] == key)
			return i;

		i = h + a * k;
		x = sel_u32(i, i, -i) % n * sel_s32(i, 1, -1);
		i = sel_u32(x, x, n + x);
		i = sel_u32(i - n, i - n, i);
		k = (unsigned) (-(int) k + sel_u32(k, 0, 1));
		a = (unsigned) abs_s32((int) k);
	}

	return -1;
}

