all: Build.exe

object_files/main.o: ../src/main.cpp
	g++ -O1 -Wall -Wno-missing-braces -I../include/ -c ../src/main.cpp -o $(@)

object_files/entity.o: ../src/entity.cpp
	g++ -O1 -Wall -Wno-missing-braces -I../include/ -c ../src/entity.cpp -o $(@)

object_files/component.o: ../src/component.cpp
	g++ -O1 -Wall -Wno-missing-braces -I../include/ -c ../src/component.cpp -o $(@)

Build.exe: object_files/main.o object_files/entity.o object_files/component.o 
	g++ -o Build.exe object_files/main.o object_files/entity.o object_files/component.o -L../lib/ -lraylib -lopengl32 -lgdi32 -lwinmm