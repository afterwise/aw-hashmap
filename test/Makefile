
export CFLAGS += -std=c99 -D_DEFAULT_SOURCE -D_POSIX_C_SOURCE=200809L -Wall -Wextra -Itest/aw-time

ifeq ($(shell uname -s),Linux)
export LDLIBS += -lrt
endif

test: test.o aw-time/libaw-time.a
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c aw-time/aw-time.h
	$(CC) $(CFLAGS) -I.. -Iaw-time -c $< -o $@

../libaw-hashmap.a:
	$(MAKE) -C..

aw-time/libaw-time.a:
	$(MAKE) -Caw-time

aw-time/aw-time.h:
	git clone https://github.com/afterwise/aw-time.git && \
	touch -c $@

.PHONY: clean
clean:
	rm -f test test.o
	rm -rf aw-time

