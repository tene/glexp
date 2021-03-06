CFLAGS    = -ggdb -Wall -std=gnu99
CBLASARGS = -L/usr/lib64/atlas -lcblas -lm
GLLINKARGS= -lGL -lGLU -lGLEW -lglut 
LINKARGS  = $(LDFLAGS) $(LD_LOAD_FLAGS) $(GLLINKARGS) $(CBLASARGS)
VALGRIND  = valgrind --gen-suppressions=all --suppressions=support/opengl.supp --track-origins=yes --leak-check=full --show-reachable=yes

all: tri box

tri: Makefile tri.c utils.c
	gcc $(CFLAGS) $(LINKARGS) tri.c -o tri

box: Makefile box.c utils.c glmath.h
	gcc $(CFLAGS) $(LINKARGS) box.c -o box

%.valgrind: %
	$(VALGRIND) ./$*
