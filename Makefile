#OBJS specifies which files to compile as part of the project
OBJ_TWO = ui.c shellgui.c
CC = gcc
CFLAGS = -w
LINKER_FLAGS_ONE = -lSDL2 -lSDL2_mixer_ext
LINKER_FLAGS_TWO = -lSDL2 -lSDL2_mixer_ext -lncurses -lpthread

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME_ONE = main
OBJ_NAME_TWO = shell

#This is the target that compiles our executable
all : shellgui 

# main:
# 	$(CC) $(OBJ_ONE) $(CFLAGS) $(LINKER_FLAGS_ONE) -o $(OBJ_NAME_ONE)
shellgui:
	$(CC) $(OBJ_TWO) $(LINKER_FLAGS_TWO) -o $(OBJ_NAME_TWO)
