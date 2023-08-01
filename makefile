CC := g++
CFLAGS := -Wall -O3

SRCDIR := src
BUILDDIR := build

TARGET := exe
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
	@./$(TARGET) 1 1000 3