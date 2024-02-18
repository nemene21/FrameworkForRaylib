all: Build.exe

object_files/main.o: ../src/main.cpp
	g++ -O1 -Wall -Wno-missing-braces -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -c ../src/main.cpp -o $(@)

object_files/entity.o: ../src/framework/entity.cpp
	g++ -O1 -Wall -Wno-missing-braces -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -c ../src/framework/entity.cpp -o $(@)

object_files/component.o: ../src/framework/component.cpp
	g++ -O1 -Wall -Wno-missing-braces -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -c ../src/framework/component.cpp -o $(@)

object_files/misc.o: ../src/framework/misc.cpp
	g++ -O1 -Wall -Wno-missing-braces -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -c ../src/framework/misc.cpp -o $(@)

object_files/health_component.o: ../src/framework/components/health_component.cpp
	g++ -O1 -Wall -Wno-missing-braces -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -c ../src/framework/components/health_component.cpp -o $(@)

object_files/trails.o: ../src/framework/objects/trails.cpp
	g++ -O1 -Wall -Wno-missing-braces -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -c ../src/framework/objects/trails.cpp -o $(@)

object_files/sprites.o: ../src/framework/objects/sprites.cpp
	g++ -O1 -Wall -Wno-missing-braces -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -c ../src/framework/objects/sprites.cpp -o $(@)

object_files/particles.o: ../src/framework/objects/particles.cpp
	g++ -O1 -Wall -Wno-missing-braces -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -c ../src/framework/objects/particles.cpp -o $(@)

Build.exe: object_files/main.o object_files/entity.o object_files/component.o object_files/misc.o object_files/health_component.o object_files/trails.o object_files/sprites.o object_files/particles.o 
	g++ -o Build.exe object_files/main.o object_files/entity.o object_files/component.o object_files/misc.o object_files/health_component.o object_files/trails.o object_files/sprites.o object_files/particles.o -L../lib/ -lraylib -lopengl32 -lgdi32 -lwinmm

.PHONY: clean
clean:
	del object_files/main.o 
	del object_files/entity.o 
	del object_files/component.o 
	del object_files/misc.o 
	del object_files/health_component.o 
	del object_files/trails.o 
	del object_files/sprites.o 
	del object_files/particles.o 
