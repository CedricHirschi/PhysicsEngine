MAIN_NAME = main
BUILD_DIR = build

SHELL = cmd.exe

CXX = g++
OPT = -Ofast -fopenmp
CFLAGS = -I.\SFML\include -DSFML_STATIC -static-libstdc++ $(OPT)
LFLAGS = -L.\SFML\lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lsfml-audio-s -lsfml-network-s -lopenal32 -lopengl32 -lwinmm -lgdi32 -lfreetype -static-libstdc++ $(OPT)

RED=[0;31m
BLUE=[0;34m
GREEN=[0;32m
NC=[0m
DONE = @echo $(GREEN)Done!$(NC)
ERROR = @echo $(RED)Error!$(NC)

.SILENT:

all: build run

build: clean compile link

.PHONY: run clean pack build

compile:
	echo Compiling $(BLUE)$(MAIN_NAME).cpp$(NC) into $(BLUE).\$(BUILD_DIR)\$(MAIN_NAME).o$(NC)...
	mkdir $(BUILD_DIR)
	$(CXX) -c $(MAIN_NAME).cpp -o .\$(BUILD_DIR)\$(MAIN_NAME).o $(CFLAGS)
	$(DONE)

link:
	echo Linking $(BLUE).\$(BUILD_DIR)\$(MAIN_NAME).o$(NC) into $(BLUE).\$(BUILD_DIR)\$(MAIN_NAME).exe$(NC)...
	$(CXX) .\$(BUILD_DIR)\$(MAIN_NAME).o -o .\$(BUILD_DIR)\$(MAIN_NAME) $(LFLAGS)
	$(DONE)

run:
	if not exist .\$(BUILD_DIR)\$(MAIN_NAME).exe (echo $(BLUE).\$(BUILD_DIR)\$(MAIN_NAME)$(NC) not found, run '$(RED)make [build]$(NC)' first! && exit 1) \
	else \
	echo Running $(BLUE).\$(BUILD_DIR)\$(MAIN_NAME).exe$(NC)... && \
	.\$(BUILD_DIR)\$(MAIN_NAME).exe && \
	$(DONE) \

clean:
	echo Cleaning up... 
	if exist $(BUILD_DIR) (echo   removing $(BLUE).\$(BUILD_DIR)\$(NC) && rmdir /s /q $(BUILD_DIR))
	if exist $(MAIN_NAME).zip (echo   removing $(BLUE).\$(MAIN_NAME).zip$(NC) && del $(MAIN_NAME).zip)
	if exist data.csv (echo  > removing $(BLUE)data.csv$(NC) && del data.csv)
	$(DONE)

pack:
	echo Packing $(BLUE)$(MAIN_NAME).zip$(NC)...
	zip -r $(MAIN_NAME).zip . -x *.git* *.zip* *.o *.exe *.dll *.lib *.a *.so *.dylib *.dSYM* *.obj *.pdb *.ilk *.exp *.lib *.idb *.manifest *.sdf *.suo *.ncb *.user *.opensdf *.opendb *.sln.docstates *.sln.ide *.sln.vspscc *.sln.vssscc *.sln.vspscc *.sln.vssscc *.sln.docstates *.sln.ide *.sln.vspscc *.sln.vssscc *.sln.vspscc *.sln.vssscc
	$(DONE)

help:
	echo make build - Compiles and links the program
	echo make run - Runs the program
	echo make clean - Removes the build directory
	echo make pack - Packs the program into a zip file
	echo make help - Shows this help
	echo make all - Runs build and run
