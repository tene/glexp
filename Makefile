CFLAGS    = -ggdb -Wall -std=gnu99
LINKARGS  = $(LDFLAGS) $(LD_LOAD_FLAGS) -lGL -lGLU -lGLEW -lglut
VALGRIND  = valgrind --gen-suppressions=all --suppressions=support/opengl.supp --track-origins=yes --leak-check=full --show-reachable=yes

tri: Makefile tri.c utils.c
	gcc $(CFLAGS) $(LINKARGS) tri.c -o tri

%.valgrind: %
	$(VALGRIND) ./$*
