CFLAGS=-Wall -O3 -g
INCLUDES=-I/opt/X11/include
LDFLAGS=-L/opt/X11/lib -lX11 -lm

galsim: galsim.o graphics.o file_operations.o
	gcc -o galsim galsim.o graphics.o file_operations.o $(LDFLAGS)

all: galsim compare

compare: compare_gal_files/compare_gal_files.c file_operations.o
	gcc -o compare compare_gal_files/compare_gal_files.c file_operations.o $(LDFLAGS)

perf: perf_test.o galsim.o
	gcc -o perf perf_test.o galsim.o graphics.o file_operations.o $(LDFLAGS)

galsim.o: galsim.c galsim.h graphics/graphics.h
	gcc $(CFLAGS) $(INCLUDES) -c galsim.c

graphics.o: graphics/graphics.c graphics/graphics.h
	gcc $(CFLAGS) $(INCLUDES) -c graphics/graphics.c

file_operations.o: file_operations/file_operations.c file_operations/file_operations.h
	gcc $(CFLAGS) $(INCLUDES) -c file_operations/file_operations.c

perf_test.o: perf_test.c galsim.h
	gcc $(CFLAGS) $(INCLUDES) -c perf_test.c 

clean:
	rm -f ./galsim ./compare ./perf *.o *.gal 
