
#include "aw-hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void testmap(unsigned n, unsigned m, unsigned of, unsigned fp) {
	unsigned *knownkeys = malloc(sizeof (unsigned) * (m + of));
	unsigned *mapkeys = malloc(sizeof (unsigned) * (m + of));
	char *mapvals = malloc(sizeof (char *) * (m + of));
	unsigned i, j, k;
	int err;

	memset(knownkeys, 0, sizeof (unsigned) * (m + of));
	memset(mapkeys, 0, sizeof (unsigned) * (m + of));
	memset(mapvals, 0, sizeof (char *) * (m + of));

	srand(0x3a2b);

	printf("adding %d\n", n);

	for (i = 0; i < n + of; ++i) {
add_again:
		k = rand() % 0xffff;

		for (j = 0; j < i; ++j)
			if (k == knownkeys[j])
				goto add_again;

		knownkeys[i] = k;

		if ((err = hashmap_add(mapkeys, m, k)) < 0) {
			fprintf(stderr, "hashmap_add failed: i=%d/%d k=%04x\n", i, n, k);
			exit(1);
		}
	}

	printf("finding %d\n", n);

	for (i = 0; i < n; ++i) {
		k = knownkeys[i];

		if (hashmap_find(mapkeys, m, k) < 0) {
			fprintf(stderr, "hashmap_find failed: i=%d/%d k=%04x\n", i, n, k);
			exit(1);
		}
	}

	printf("finding %d invalid\n", fp);

	for (i = 0; i < fp; ++i) {
fp_again:
		k = rand() % 0xffff;

		for (j = 0; j < n; ++j)
			if (k == knownkeys[j])
				goto fp_again;

		if (hashmap_find(mapkeys, m, k) >= 0) {
			fprintf(stderr, "hashmap_find false positive: i=%d/%d k=%04x\n", i, n, k);
			exit(1);
		}
	}

	printf("removing %d + verifying\n", n);

	for (i = 0; i < n; ++i) {
		for (j = i; j < n; ++j) {
			k = knownkeys[j];

			if (hashmap_find(mapkeys, m, k) < 0) {
				fprintf(stderr, "hashmap_find failed during removal: j=%d/%d k=%04x\n", j, n, k);
				exit(1);
			}
		}

		k = knownkeys[i];

		if (hashmap_remove(mapkeys, m, k) < 0) {
			fprintf(stderr, "hashmap_find failed: i=%d/%d k=%04x\n", i, n, k);
			exit(1);
		}
	}

	free(knownkeys);
	free(mapkeys);
	free(mapvals);

	printf("OK\n");
}

int main(int argc, char *argv[]) {
	(void) argc;
	(void) argv;

	testmap(9999, 9999 + 3333, 0, 7777);

	return 0;
}

