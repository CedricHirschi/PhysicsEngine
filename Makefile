MAIN_NAME = main
BUILD_DIR = build
GPU_NAME = NVIDIA GeForce RTX 3050 Ti Laptop GPU

CXX = g++
OPT = -Ofast -fopenmp
CFLAGS = -I.\SFML\include -DSFML_STATIC -static-libstdc++ $(OPT)
LFLAGS = -L.\SFML\lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lsfml-audio-s -lsfml-network-s -lopenal32 -lopengl32 -lwinmm -lgdi32 -lfreetype -static-libstdc++ $(OPT)

.SILENT:

all: build run

build: clean compile link

.PHONY: run clean pack build

compile:
	echo Compiling $(MAIN_NAME).cpp into .\$(BUILD_DIR)\$(MAIN_NAME).o... 
	mkdir $(BUILD_DIR)
	$(CXX) -c $(MAIN_NAME).cpp -o .\$(BUILD_DIR)\$(MAIN_NAME).o $(CFLAGS)
	echo Done

link:
	echo Linking .\$(BUILD_DIR)\$(MAIN_NAME).o into .\$(BUILD_DIR)\$(MAIN_NAME).exe
	$(CXX) .\$(BUILD_DIR)\$(MAIN_NAME).o -o .\$(BUILD_DIR)\$(MAIN_NAME) $(LFLAGS)
	echo Done

run:
	if not exist .\$(BUILD_DIR)\$(MAIN_NAME).exe (echo .\$(BUILD_DIR)\$(MAIN_NAME) not found, run 'make [build]' first!) \
	else \
	echo Running .\$(BUILD_DIR)\$(MAIN_NAME).exe... && \
	.\$(BUILD_DIR)\$(MAIN_NAME).exe && \
	echo Done \

clean:
	echo Cleaning up... 
	if exist $(BUILD_DIR) (echo removing .\$(BUILD_DIR)\ && rmdir /s /q $(BUILD_DIR))
	if exist $(MAIN_NAME).zip (echo removing .\$(MAIN_NAME).zip && del $(MAIN_NAME).zip)
	if exist data.csv (echo removing data.csv && del data.csv)
	echo Done

pack:
	echo Packing $(MAIN_NAME).zip...
	zip -r $(MAIN_NAME).zip . -x *.git* *.zip* *.o *.exe *.dll *.lib *.a *.so *.dylib *.dSYM* *.obj *.pdb *.ilk *.exp *.lib *.idb *.manifest *.sdf *.suo *.ncb *.user *.opensdf *.opendb *.sln.docstates *.sln.ide *.sln.vspscc *.sln.vssscc *.sln.vspscc *.sln.vssscc *.sln.docstates *.sln.ide *.sln.vspscc *.sln.vssscc *.sln.vspscc *.sln.vssscc
	echo Done

help:
	echo make build - Compiles and links the program
	echo make run - Runs the program
	echo make clean - Removes the build directory
	echo make pack - Packs the program into a zip file
	echo make help - Shows this help
	echo make all - Runs build and run
