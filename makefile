OPT = -g3 -O0
LIB_SOURCES1 = main.c main_linux.c distfield.c

LIB_SOURCES = $(addprefix src/, $(LIB_SOURCES1))
CC = gcc
TARGET = run.exe
LIB_OBJECTS =$(LIB_SOURCES:.c=.o)
LDFLAGS= -L. $(OPT) # -Wl,--gc-sections -Wl,-s -fdata-sections -ffunction-sections
LIBS= -l:iron/libiron.a -lglfw -lGL -lGLEW -lm  -lopenal -licydb -lpthread -lX11 -lglfw 
ALL= $(TARGET)
CFLAGS = -Isrc/ -Iinclude/ -std=gnu11 -c $(OPT) -Werror=implicit-function-declaration -Wformat=0 -D_GNU_SOURCE -fdiagnostics-color  -Wwrite-strings -msse4.2 -Werror=uninitialized -DUSE_VALGRIND -DDEBUG -Wall

$(TARGET): $(LIB_OBJECTS) iron/libiron.a 
	$(CC) $(LDFLAGS) $(LIB_OBJECTS) $(LIBS) -o $@

all: $(ALL)

src/distfield.c: src/dist.shader.c

iron/libiron.a:
	make -C iron

.c.o: $(HEADERS) $(LEVEL_CS)
	$(CC) $(CFLAGS) $< -o $@ -MMD -MF $@.depends

src/ttf_font.c: 
	xxd -i /usr/share/fonts/truetype/dejavu/DejaVuSans.ttf > src/ttf_font.c
src/dist.shader.c: src/dist.shader.fs src/dist.shader.vs src/dist.shader.cs
	xxd -i src/dist.shader.vs > src/dist.shader.c
	xxd -i src/dist.shader.fs >> src/dist.shader.c
	xxd -i src/dist.shader.cs >> src/dist.shader.c


main.o: ttf_font.c

depend: h-depend
clean:
	make -C iron clean
	rm -f $(LIB_OBJECTS) $(ALL) src/*.o.depends src/*.o src/level*.c src/*.shader.c

.PHONY: test
.PHONY: iron/libiron.a
test: $(TARGET)
	make -f makefile.compiler
	make -f makefile.test test

-include $(LIB_OBJECTS:.o=.o.depends)


