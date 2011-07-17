CFLAGS    = -ggdb -Wall -std=gnu99
LINKARGS  = $(LDFLAGS) $(LD_LOAD_FLAGS) -lGL -lGLU -lGLEW -lglut

tri: Makefile tri.c utils.c
	gcc $(CFLAGS) $(LINKARGS) tri.c -o tri
