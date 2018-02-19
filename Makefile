CFLAGS = -g -Wall
LDLIBS = -lhyphen

hyphenate:	hyphenate.c

clean:
	rm hyphenate *.o 2>/dev/null || true
