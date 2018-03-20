build:
	gcc -c -I../include ../src/elise.c ../src/networking.c

dylib:
	gcc -o libelise.dylib -dynamiclib elise.o networking.o

clean:
	rm ./libelise.dylib ./*.o
