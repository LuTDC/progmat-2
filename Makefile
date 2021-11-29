archflags = -march=native -mtune=native
optimizationflags = -O3
libraries = -lgurobi


all:	compile

compile:
	gcc src/main.c -o bin/Main $(libraries) $(archflags) $(optimizationflags)

run:
	./bin/Main

clean:
	rm Main
