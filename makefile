CC = gcc
CFLAGS = -Wall -Wextra -pedantic -Iinclude
LDFLAGS = -lm
TARGET = simulator
SRCDIR = src
OBJDIR = obj
BINDIR = bin
RESULT_FOLDER = results
PLOT_FOLDER = plots
RESULT_TYPE_FOLDER_1 = generic
RESULT_TYPE_FOLDER_2 = problem

FOLDER_GENERIC = \
    $(RESULT_FOLDER)/$(RESULT_TYPE_FOLDER_1)/same_arrival_time_rate \
    $(RESULT_FOLDER)/$(RESULT_TYPE_FOLDER_1)/low_to_high_arrival_time_rate \
    $(RESULT_FOLDER)/$(RESULT_TYPE_FOLDER_1)/high_arrival_time_rate

FOLDER_PROBLEM = \
    $(RESULT_FOLDER)/$(RESULT_TYPE_FOLDER_2)/avarage_lateness \
    $(RESULT_FOLDER)/$(RESULT_TYPE_FOLDER_2)/longer_Wait \
    $(RESULT_FOLDER)/$(RESULT_TYPE_FOLDER_2)/round_robin

FOLDERS = $(FOLDER_GENERIC) $(FOLDER_PROBLEM)

SRCS := $(wildcard $(SRCDIR)/*.c)
OBJS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

all: $(FOLDERS) $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJS)
	@mkdir -p $(BINDIR)
	@$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(FOLDERS):
	@mkdir -p $@

run: clean all 
	@./$(BINDIR)/$(TARGET)

clean:
	@rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all run clean complete $(FOLDERS)