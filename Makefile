all:
	gcc -O0 src/main.c src/set1/setone.c src/testing.c src/set2/set2_chal1.c -o run.o
	./run.o

clean:
	rm run.o

