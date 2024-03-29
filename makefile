CC := gcc
CFLAGS := -Wall -Wextra -Werror -std=c99 -O3 -lm

SRCDIR := src
BUILDDIR := build

TARGET := ordena
SOURCES := $(wildcard $(SRCDIR)/*.c $(SRCDIR)/*/*.c)
OBJECTS := $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SOURCES))

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@$(CC) -o $@ $^ $(CFLAGS)
	@echo "Compilation successful. Executable: $(TARGET)"

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D)
	@mkdir -p tmp
	@$(CC) -c $< -o $@ $(CFLAGS)

clean: 
	@$(RM) -r $(BUILDDIR) $(TARGET)
	@echo "Cleanup complete"

run: $(TARGET)
	@make clean
	@./$(TARGET) 1 1000 3