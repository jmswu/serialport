# This make file is written for Windows only
# good reference about patten matching: 
# https://stackoverflow.com/questions/40621451/makefile-automatically-compile-all-c-files-keeping-o-files-in-separate-folde


# define build folder here
FOLDER_BUILD 	:= build
# define source folder here
FOLDER_SOURCE_1 := src
FOLDER_SOURCE_2 := ${FOLDER_SOURCE_1}/Unity
# define application name here
TARGET_NAME 	:= unit_test

#----------------------------------
# Don't change anything below here
#----------------------------------

SHELL	:= cmd
CC 		:= gcc

CFLAGS  = -std=c99
CFLAGS += -Werror
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wpointer-arith
CFLAGS += -Wcast-align
CFLAGS += -Wwrite-strings
CFLAGS += -Wswitch-default
CFLAGS += -Wunreachable-code
CFLAGS += -Winit-self
CFLAGS += -Wmissing-field-initializers
CFLAGS += -Wno-unknown-pragmas
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wundef
CFLAGS += -Wold-style-definition
CFLAGS += -Wno-misleading-indentation

IF_NOT_EXIT			:=@if not exist
IF_EXIT				:=@if exist
RMDIR				:=@rmdir /S /Q
MKDIR				:=@mkdir
MAKE_BUILD_FOLDER	:=$(IF_NOT_EXIT) $(FOLDER_BUILD)/NUL $(MKDIR) $(FOLDER_BUILD)
REMOVE_BUILD_FOLDER	:=$(IF_EXIT) $(FOLDER_BUILD) $(RMDIR) $(FOLDER_BUILD)


TARGET			:= $(FOLDER_BUILD)/$(TARGET_NAME)
RUN_TARGET		:= $(TARGET)

FILES_SOURCE_1		:= $(wildcard $(FOLDER_SOURCE_1)/*.c)
FILES_SOURCE_2		:= $(wildcard $(FOLDER_SOURCE_2)/*.c)
FILES_SOURCE 		 = $(FILES_SOURCE_1) $(FILES_SOURCE_2)

FILES_OBJECT_1		 = $(patsubst $(FOLDER_SOURCE_1)/%.c,$(FOLDER_BUILD)/%.o,$(FILES_SOURCE_1))
FILES_OBJECT_2		+= $(patsubst $(FOLDER_SOURCE_2)/%.c,$(FOLDER_BUILD)/%.o,$(FILES_SOURCE_2))
FILES_OBJECT		 = $(FILES_OBJECT_1) $(FILES_OBJECT_2)

# build application
$(TARGET): $(FILES_OBJECT)
	@$(MAKE_BUILD_FOLDER)
	@$(CC) $(CFLAGS) -o $@ $^
	@$(RUN_TARGET)

# build object files
$(FOLDER_BUILD)/%.o: $(FOLDER_SOURCE_1)/%.c
	@$(MAKE_BUILD_FOLDER)
	@$(CC) -I$(FOLDER_SOURCE_1) -I$(FOLDER_SOURCE_2) -I$(FO) $(CFLAGS) -c $< -o $@

# build object files
$(FOLDER_BUILD)/%.o: $(FOLDER_SOURCE_2)/%.c
	@$(MAKE_BUILD_FOLDER)
	@$(CC) -I$(FOLDER_SOURCE) -I$(FO) $(CFLAGS) -c $< -o $@

# clean build folder
.PHONY: clean
clean:
	$(REMOVE_BUILD_FOLDER)

.PHONY: test1
test1:
	@echo $(FILES_SOURCE)
	@echo $(FILES_OBJECT)

.DELETE_ON_ERROR:
	$(REMOVE_BUILD_FOLDER)