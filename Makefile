MAIN_NAME = main
BUILD_DIR = build
SOURCE_DIR = source
INCLUDE_DIR = $(SOURCE_DIR)/include
SFML_DIR = SFML

USER = $(wildcard *.cpp)
SRC = $(wildcard $(SOURCE_DIR)/*.cpp)
OBJ = $(patsubst $(SOURCE_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC)) $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(USER))

CXX = g++
OPT = -O3 -fopenmp
CFLAGS = -I$(INCLUDE_DIR) -I$(SFML_DIR)/include -DSFML_STATIC -static-libstdc++ -lstdc++fs $(OPT)
LFLAGS = -L$(SFML_DIR)/lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lsfml-audio-s -lsfml-network-s -lopengl32 -lwinmm -lgdi32 -lfreetype -static-libstdc++ -lstdc++fs $(OPT)

SHELL = cmd.exe
RED=[0;31m
BLUE=[0;34m
GREEN=[0;32m
NC=[0m
DONE = # @echo $(GREEN)Done!$(NC)
ERROR = @echo $(RED)Error!$(NC)

export MAKEFLAGS=-j

.SECONDARY:

.SILENT:

all: run
build: $(BUILD_DIR)/$(MAIN_NAME).exe

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	if not exist $(BUILD_DIR) echo Creating $(BLUE)$(BUILD_DIR)$(NC) directory... && mkdir $(BUILD_DIR)
	echo Compiling $(BLUE)$<$(NC) into $(BLUE)$@$(NC)...
	$(CXX) -c $< -o $@ $(CFLAGS)
	$(DONE)

$(BUILD_DIR)/%.o: %.cpp
	echo Compiling $(BLUE)$<$(NC) into $(BLUE)$@$(NC)...
	$(CXX) -c $< -o $@ $(CFLAGS)
	$(DONE)

$(BUILD_DIR)/%.exe: $(OBJ)
	echo Linking $(BLUE)$@$(NC) from $(BLUE)$^$(NC)...
	$(CXX) $^ -o $@ $(LFLAGS)
	$(DONE)

run: build
	echo Running $(BLUE)$(MAIN_NAME).exe$(NC)...
	$(BUILD_DIR)/$(MAIN_NAME).exe
	$(DONE)

clean:
	echo Cleaning...
	if exist $(BUILD_DIR) (echo Deleting $(BLUE)$(BUILD_DIR)$(NC) directory... && rmdir /S /Q $(BUILD_DIR))
	if exist $(MAIN_NAME).zip (echo Deleting $(BLUE)$(MAIN_NAME).zip$(NC)... && del /S /Q *.zip)
	$(DONE)

pack:
	echo Packing source, include and SFML directories into $(BLUE)$(MAIN_NAME).zip$(NC)...
	zip -r $(MAIN_NAME).zip $(SOURCE_DIR) $(INCLUDE_DIR) $(SFML_DIR) Makefile
	$(DONE)

find:
	echo from $(SRC) $(USER)
	echo      vvvvv
	echo to   $(OBJ)

help:
	echo Available commands:
	echo   $(GREEN)all$(NC)     mapped to run
	echo   $(BLUE)build$(NC)   builds executable
	echo   $(BLUE)run$(NC)     runs executable                     (runs $(BLUE)build$(NC) if needed)
	echo   $(BLUE)clean$(NC)   deletes build directory and zip
	echo   $(BLUE)pack$(NC)    pack executable into a zip          (runs $(BLUE)build$(NC) if needed)
	echo   $(BLUE)find$(NC)    list files which will be compiled
	echo   $(RED)help$(NC)    show this message

.PHONY: all run clean build pack find