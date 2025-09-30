CC = gcc
CFLAGS = -Wall -Wextra -pedantic -Iinclude
LDFLAGS = -lm
TARGET = simulator
SRCDIR = src
OBJDIR = obj
BINDIR = bin

SRCS := $(wildcard $(SRCDIR)/*.c)
OBJS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

all: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJS)
	@mkdir -p $(BINDIR)
	@$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -c $< -o $@

run: clean all 
	@./$(BINDIR)/$(TARGET)

clean:
	@rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all run clean complete