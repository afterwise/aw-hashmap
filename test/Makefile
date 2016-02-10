
export CFLAGS += -std=c99 -Wall -Wextra -Itest/aw-time

test: test.o aw-time/libaw-time.a
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c aw-time/aw-time.h
	$(CC) $(CFLAGS) -I.. -Iaw-time -c $< -o $@

../libaw-hashmap.a:
	$(MAKE) -C..

aw-time/libaw-time.a:
	$(MAKE) -Caw-time

aw-time/aw-time.h:
	git clone git@github.com:afterwise/aw-time.git && \
	touch -c $@

.PHONY: clean
clean:
	rm -f test test.o
	rm -rf aw-time

