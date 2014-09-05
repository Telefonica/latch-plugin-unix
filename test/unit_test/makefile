CFLAGS +=  -Wall

CC = gcc 
OBJECTS = ../../lib/util.o ../../lib/charset.o UtilTest.o CharsetTest.o CuTest.o

test : AllTests.o $(OBJECTS)
	$(CC) $(CFLAGS) -o test AllTests.o $(OBJECTS) 


AllTests.o: CuTest.h
../../lib/util.o: ../../lib/util.h
../../lib/charset.o: ../../lib/charset.h
UtilTest.o: CuTest.h ../../lib/util.h
CharsetTest.o: CuTest.h ../../lib/charset.h
CuTest.o: CuTest.h


.PHONY : clean
    clean :
	-rm test AllTests.o $(OBJECTS) 
    all:
	-sudo chown root:root test
	-sudo chmod 4755 test
