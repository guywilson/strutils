###############################################################################
#                                                                             #
# MAKEFILE for strutils                                                       #
#                                                                             #
# (c) Guy Wilson 2020                                                         #
#                                                                             #
###############################################################################

# Directories
SOURCE = src
BUILD = build
DEP = dep
LIB = lib

# What is our target
TARGET = su
LIBTARGET = lib$(TARGET).so

# Tools
C = gcc
LINKER = gcc

# postcompile step
PRECOMPILE = @ mkdir -p $(BUILD) $(DEP)
# postcompile step
POSTCOMPILE = @ mv -f $(DEP)/$*.Td $(DEP)/$*.d

CFLAGS = -c -O2 -Wall -pedantic -fPIC
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEP)/$*.Td

COMPILE.c = $(C) $(CFLAGS) $(DEPFLAGS) $(MGFLAGS) -o $@
LINK.o = $(LINKER) -o $@

CSRCFILES = $(wildcard $(SOURCE)/*.c)
OBJFILES := $(patsubst $(SOURCE)/%.c, $(BUILD)/%.o, $(CSRCFILES))
OBJFILES := $(filter-out $(BUILD)/test.o, $(OBJFILES))
DEPFILES = $(patsubst $(SOURCE)/%.c, $(DEP)/%.d, $(CSRCFILES))

all: $(TARGET) $(LIBTARGET)

# Compile C/C++ source files
#
$(TARGET): $(OBJFILES) $(BUILD)/test.o
	$(LINK.o) $^

$(LIBTARGET): $(OBJFILES)
	$(LINKER) -shared -o $(LIB)/$(LIBTARGET) $^

$(BUILD)/%.o: $(SOURCE)/%.c
$(BUILD)/%.o: $(SOURCE)/%.c $(DEP)/%.d
	$(PRECOMPILE)
	$(COMPILE.c) $<
	$(POSTCOMPILE)

.PRECIOUS = $(DEP)/%.d
$(DEP)/%.d: ;

-include $(DEPFILES)

install: $(TARGET)
	cp $(TARGET) /usr/local/bin
	cp $(LIB)/$(LIBTARGET) /usr/local/lib
	cp $(SOURCE)/strutils.h /usr/local/include
	
clean:
	rm -r $(BUILD)
	rm -r $(DEP)
	rm $(TARGET)
