MAIN_NAME = main
BUILD_DIR = ..\build
INCLUDE_DIR = include

SRC = $(wildcard *.cpp)

CXX = g++
OPT = -Ofast -fopenmp
CFLAGS = -I$(INCLUDE_DIR) -I..\SFML\include -DSFML_STATIC -static-libstdc++ $(OPT)
LFLAGS = -L..\SFML\lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lsfml-audio-s -lsfml-network-s -lopenal32 -lopengl32 -lwinmm -lgdi32 -lfreetype -static-libstdc++ $(OPT)

SHELL = cmd.exe
RED=[0;31m
BLUE=[0;34m
GREEN=[0;32m
NC=[0m
DONE = @echo $(GREEN)Done!$(NC)
ERROR = @echo $(RED)Error!$(NC)

.SECONDARY:

.SILENT:

all: run

$(BUILD_DIR)/%.o: %.cpp
	echo Compiling $(BLUE)$<$(NC)...
	$(CXX) -c $< -o $@ $(CFLAGS)
	$(DONE)

$(BUILD_DIR)/%.exe: $(SRC:%.cpp=$(BUILD_DIR)/%.o)
	echo Linking $(BLUE)$@$(NC)...
	$(CXX) $^ -o $@ $(LFLAGS)
	$(DONE)

compile_and_link: $(BUILD_DIR)/$(MAIN_NAME).exe

run: compile_and_link
	echo Running $(BLUE)$(MAIN_NAME).exe$(NC)...
	$(BUILD_DIR)/$(MAIN_NAME).exe
	$(DONE)

clean:
	echo Cleaning...
	echo Deleting object files...
	del /Q /S $(BUILD_DIR)\*.o
	echo Deleting executables...
	del /Q /S $(BUILD_DIR)\*.exe
	$(DONE)

.PHONY: all build run clean