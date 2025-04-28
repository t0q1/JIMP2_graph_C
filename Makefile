all:
	gcc src/*.c -o bin/graph

test: all
	./bin/graph ./tests/graf.csrrg 3
