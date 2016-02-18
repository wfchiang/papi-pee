
CFLAGS = -Wno-write-strings
INCDIR = -I/home/wfchiang/lib/papi-5.4.3/include
LIBDIR = -L/home/wfchiang/lib/papi-5.4.3/lib 


all: rapl_utils.o rapl_test.o rapl_loader.o rapl_test rapl_loader 

rapl_utils.o: rapl_utils.c rapl_utils.h
	gcc $(CFLAGS) $(INCDIR) $(LIBDIR) -c rapl_utils.c -lpapi -lpfm

rapl_test.o: rapl_test.c
	gcc $(CFLAGS) $(INCDIR) $(LIBDIR) -c rapl_test.c -lpapi -lpfm

rapl_loader.o: rapl_loader.c
	gcc $(CFLAGS) $(INCDIR) $(LIBDIR) -c rapl_loader.c -lpapi -lpfm

rapl_test: rapl_utils.o rapl_test.o
	gcc $(CFLAGS) $(INCDIR) $(LIBDIR) -o rapl_test rapl_utils.o rapl_test.o -lpapi -lpfm

rapl_loader: rapl_utils.o rapl_loader.o
	gcc $(CFLAGS) $(INCDIR) $(LIBDIR) -o rapl_loader rapl_utils.o rapl_loader.o -lpapi -lpfm

clean:
	-rm *~
	-rm *.o
	-rm rapl_test
	-rm rapl_loader 
