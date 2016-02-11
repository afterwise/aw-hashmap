
/*
   Copyright (c) 2014-2016 Malte Hildingsson, malte (at) afterwi.se

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
 */

#ifndef AW_HASHMAP_H
#define AW_HASHMAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <string.h>

#if __GNUC__
# define _hashmap_alwaysinline inline __attribute__((always_inline))
# define _hashmap_unused __attribute__((unused))
#elif _MSC_VER
# define _hashmap_alwaysinline __forceinline
# define _hashmap_unused
#endif

#if __GNUC__
# define _hashmap_likely(x) __builtin_expect(!!(x), 1)
# define _hashmap_unlikely(x) __builtin_expect(!!(x), 0)
#else
# define _hashmap_likely(x) (x)
# define _hashmap_unlikely(x) (x)
#endif

typedef signed char hashmap_s8_t;
typedef unsigned char hashmap_u8_t;
typedef signed short hashmap_s16_t;
typedef unsigned short hashmap_u16_t;
typedef signed int hashmap_s32_t;
typedef unsigned int hashmap_u32_t;
#if _MSC_VER
typedef signed __int64 hashmap_s64_t;
typedef unsigned __int64 hashmap_u64_t;
#else
typedef signed long long hashmap_s64_t;
typedef unsigned long long hashmap_u64_t;
#endif
typedef float hashmap_f32_t;
typedef double hashmap_f64_t;

static _hashmap_alwaysinline hashmap_u32_t hashmap_lsr31(hashmap_u32_t a) { return a >> 31; }
static _hashmap_alwaysinline hashmap_s32_t hashmap_asr31(hashmap_s32_t a) { return a >> 31; }

static _hashmap_alwaysinline hashmap_s32_t hashmap_abs_s32(hashmap_s32_t a) {
	return (a ^ hashmap_asr31(a)) - hashmap_asr31(a);
}

static _hashmap_alwaysinline hashmap_s32_t hashmap_sel_s32(hashmap_s32_t x, hashmap_s32_t a, hashmap_s32_t b) {
	return a + ((b - a) & hashmap_asr31(x));
}
static _hashmap_alwaysinline hashmap_u32_t hashmap_sel_u32(hashmap_u32_t x, hashmap_u32_t a, hashmap_u32_t b) {
	return a + ((b - a) & hashmap_asr31(x));
}

/*
	open addressing fanning linear probing hash map algorithm with little branching.
	type-safe and memory management agnostic in that it only cares about the keys,
	the user will then allocate another array of the value type and use the returned
	index from hashmap_add() and hashmap_find() to access data in the value array.

	keys are 32 bit in size, and the values 0x00000000 and 0xffffffff are magic in
	that they mean empty and removed slot respectively, and are treated as such when
	probing.
*/

_hashmap_unused
static void hashmap_init(hashmap_u32_t *key, size_t n) {
	memset(key, 0, sizeof (hashmap_u32_t) * n);
}

_hashmap_unused
static ssize_t hashmap_add(hashmap_u32_t *keys, size_t n, hashmap_u32_t key) {
	hashmap_u32_t h = key % n, i = h, k = 1, a = 1;
	hashmap_s32_t x;
	while (a < n) {
		if (keys[i] == (hashmap_u32_t) hashmap_asr31(keys[i]) || keys[i] == key) {
			keys[i] = key;
			return i;
		}
		i = h + a * k;
		x = hashmap_sel_u32(i, i, -i) % n * hashmap_sel_s32(i, 1, -1);
		i = hashmap_sel_u32(x, x, n + x);
		i = hashmap_sel_u32(i - n, i - n, i);
		k = (hashmap_u32_t) (-(int) k + hashmap_lsr31(k));
		a = (hashmap_u32_t) hashmap_abs_s32((int) k);
	}

	return -1;
}

_hashmap_unused
static ssize_t hashmap_find(hashmap_u32_t *keys, size_t n, hashmap_u32_t key) {
	hashmap_u32_t h = key % n, i = h, k = 1, a = 1;
	hashmap_s32_t x;
	while (a < n) {
		if (_hashmap_unlikely(keys[i] == 0))
			break;
		if (keys[i] == key)
			return i;
		i = h + a * k;
		x = hashmap_sel_u32(i, i, -i) % n * hashmap_sel_s32(i, 1, -1);
		i = hashmap_sel_u32(x, x, n + x);
		i = hashmap_sel_u32(i - n, i - n, i);
		k = (hashmap_u32_t) (-(int) k + hashmap_lsr31(k));
		a = (hashmap_u32_t) hashmap_abs_s32((int) k);
	}
	return -1;
}

_hashmap_unused
static ssize_t hashmap_remove(hashmap_u32_t *keys, size_t n, hashmap_u32_t key) {
	ssize_t i;
	if ((i = hashmap_find(keys, n, key)) >= 0) {
		keys[i] = ~0u;
		return i;
	}
	return -1;
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* AW_HASHMAP_H */

