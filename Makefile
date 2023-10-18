all: ./test/random_test

./test/random_test: ./uwp/uwp.o ./test/random_test.o
	g++ ./uwp/uwp.o ./test/random_test.o -o ./test/random_test

./uwp/uwp.o: ./uwp/uwp.cpp ./uwp/uwp.h
	g++ -c ./uwp/uwp.cpp -o ./uwp/uwp.o

./test/random_test.o: ./test/random_test.cpp
	g++ -c ./test/random_test.cpp -o ./test/random_test.o

clean:
	rm *.o