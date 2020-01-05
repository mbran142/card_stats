stat: main.o sequence.o specification.o simulation.o
	-gcc -g -Wall -ggdb3 -o stat main.o sequence.o specification.o simulation.o

main.o: main.c
	-gcc -g -Wall -ggdb3 -c main.c -o main.o

sequence.o: sequence.c
	-gcc -g -Wall -ggdb3 -c sequence.c -o sequence.o

specification.o: specification.c
	-gcc -g -Wall -ggdb3 -c specification.c -o specification.o

simulation.o: simulation.c
	-gcc -g -Wall -ggdb3 -c simulation.c -o simulation.o

clean:
	rm stat main.o sequence.o specification.o simulation.o