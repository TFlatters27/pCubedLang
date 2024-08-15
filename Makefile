exec = p3.out
sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)
flags = -g


$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec)

%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

install:
	make
	cp ./p3.out /usr/local/bin/p3

clean:
	-rm *.out
	-rm *.o
	-rm src/*.o