exec = /usr/local/bin/p3
sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)
flags = -g

$(exec): $(objects)
	@echo "Compiling and installing p3..."
	@$(CC) $(objects) $(flags) -o $(exec)
	@echo "Done."

%.o: %.c include/%.h
	@$(CC) -c $(flags) $< -o $@

clean:
	@echo "Cleaning up..."
	@rm -f $(objects)
	@rm -f $(exec)
	@echo "Clean complete."
