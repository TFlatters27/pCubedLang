# Variables
exec = p3
sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)
CFLAGS = -g
CC ?= gcc

# Default installation directory
PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin

# Targets
$(BINDIR)/$(exec): $(objects)
	@echo "Compiling and installing $(exec)..."
	@$(CC) $(objects) $(CFLAGS) -o $(BINDIR)/$(exec)
	@echo "Done."

%.o: %.c
	@echo "Compiling $<..."
	@$(CC) -c $(CFLAGS) $< -o $@

clean:
	@echo "Cleaning up..."
	@rm -f $(objects)
	@rm -f $(BINDIR)/$(exec)
	@echo "Clean complete."

# Install target to handle installation
install: $(BINDIR)/$(exec)
	@echo "Installing $(exec) to $(BINDIR)..."

# Uninstall target to remove installed binaries
uninstall:
	@echo "Removing $(BINDIR)/$(exec)..."
	@rm -f $(BINDIR)/$(exec)
	@echo "Uninstall complete."

.PHONY: clean install uninstall
