CFLAGS = -g -Wall
LDLIBS = -lhyphen

hyphenate:	hyphenate.c

test:	hyphenate
	./hyphenate Götterdämmerung humuhumunukunukuāpuaʻa
	./hyphenate kindergarten kindergärten
	./hyphenate zoology zoölogy
clean:
	rm hyphenate *.o 2>/dev/null || true
