LANG := C
OUTPUT := bass
TYPE := static-lib
LIBS :=
FLAGS := -g -Wall -Werror -fPIC

INCLUDE_DIR:= -I deps/*/include -I include
LIB_DIR:= -L deps/*/lib

ifeq "$(LANG)" "C++"
	EXT:=cpp
	STD:=c++14
	CC:=$(CXX)
else ifeq "$(LANG)" "C"
	EXT:=c
	STD:=c99
	CC:=$(CC)
else
	$(error Invalid language specified)
endif

AR ?= ar

SRC:=$(shell find src -name *.${EXT})
OBJ:=$(SRC:src/%.${EXT}=obj/%.o)
DEP:=$(OBJ:%.o=%.d)

CFLAGS:= -std=$(STD) $(FLAGS) $(INCLUDE_DIR)
SHELL := /bin/bash
INSTALL_DIR := /usr/local/bin

build : $(OUTPUT) remove_unused_objects

rebuild : clean build

run : build
	@./$(OUTPUT)

install : build
	@install ./$(OUTPUT) $(INSTALL_DIR)

uninstall :
	-@rm $(INSTALL_DIR)/$(OUTPUT)

$(OUTPUT): $(OBJ)
ifeq "$(TYPE)" "executable"
	@$(CC) $^ -o $(OUTPUT) $(CFLAGS) $(LIB_DIR) $(LIBS)
else ifeq "$(TYPE)" "static-lib"
	@$(AR) -rs lib$(OUTPUT).a $^ 2> /dev/null
endif

obj/%.o : src/%.$(EXT)
	@mkdir -p $(@D) # $(@D) <- Gets directory part of target path
	@$(CC) $< -o $@ $(CFLAGS) -c -MMD -MP

-include $(DEP)

-FILES_IN_OBJ = $(shell find obj -name *.o)

remove_unused_objects :
ifneq '' '$(filter-out $(OBJ), $(FILES_IN_OBJ))' # finds out which object files no longer have an associated source file
	@rm -r $(filter-out $(OBJ), $(FILES_IN_OBJ))
	-@rm -r $(filter-out $(DEP), $(FILES_IN_OBJ:%.o=%.d))
endif

clean :
	@rm -r obj/*

ifeq "$(TYPE)" "executable"
	@rm $(shell find . -name $(OUTPUT)*)
else ifeq "$(TYPE)" "static-lib"
	@rm $(shell find . -name lib$(OUTPUT).a)
endif

debug :
	@echo Language Selected: $(LANG)
	@echo Compiler: $(CC)
	@echo Standard Library: $(STD)
	@echo Libraries: $(LIBS)
	@echo
	@echo Binary Name: $(OUTPUT)
	@echo Source Files: $(SRC)
	@echo Object Files: $(OBJ)
	@echo Dependencies: $(DEP)
	@echo All files in Object folder: $(FILES_IN_OBJ)
	@echo

include TestMakefile

clean-all : clean test-clean
