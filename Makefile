CFLAGS = -g -Wall
LDLIBS = -lhyphen

hyphenate:	hyphenate.c

test:	hyphenate
	./hyphenate Götterdämmerung humuhumunukunukuāpuaʻa
	./hyphenate shelfful bookkeeper preindustrialized
	./hyphenate kindergarten kindergärten
	./hyphenate zoology zoölogy
clean:
	rm hyphenate *.o 2>/dev/null || true
