TARGET = os
CC = gcc
CFLAGS = -g -Wall
LIBS = -lm

# Diretórios
SRCDIR = src
HEADERDIR = headers
BUILDDIR = build

# Arquivos
SOURCES = $(wildcard $(SRCDIR)/*.c)
HEADERS = $(wildcard $(HEADERDIR)/*.h)
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))

.PHONY: all clean default

default: $(TARGET)
all: default

# Regra para compilar cada .c em .o dentro de build/
$(BUILDDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -I$(HEADERDIR) -c $< -o $@

# Regra de link final
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	@rm -rf $(BUILDDIR) $(TARGET)
