
export CFLAGS += -Wall -Wextra

test: test.o ../libaw-hashmap.a
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c aw-bytes/aw-types.h
	$(CC) $(CFLAGS) -I.. -Iaw-utf8 -c $< -o $@

../libaw-hashmap.a: CFLAGS += -Itest/aw-bytes

../libaw-hashmap.a: aw-bytes/aw-types.h
	$(MAKE) -C..

aw-bytes/aw-types.h:
	git clone ../../aw-bytes && \
	touch -c $@
#	git clone git@github.com:afterwise/aw-fs.git && \
#	touch -c $@

.PHONY: clean
clean:
	rm -f test test.o
	rm -rf aw-bytes

