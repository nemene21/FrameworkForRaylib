all: Build.exe

object_files/main.o: ../src/main.cpp
	g++ -O1 -Wall -Wno-missing-braces -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/main.cpp -o $(@)

object_files/entity.o: ../src/framework/entity.cpp
	g++ -O1 -Wall -Wno-missing-braces -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/entity.cpp -o $(@)

object_files/component.o: ../src/framework/component.cpp
	g++ -O1 -Wall -Wno-missing-braces -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/component.cpp -o $(@)

object_files/scene.o: ../src/framework/scene.cpp
	g++ -O1 -Wall -Wno-missing-braces -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/scene.cpp -o $(@)

object_files/misc.o: ../src/framework/misc.cpp
	g++ -O1 -Wall -Wno-missing-braces -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/misc.cpp -o $(@)

object_files/health_component.o: ../src/framework/components/health_component.cpp
	g++ -O1 -Wall -Wno-missing-braces -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/components/health_component.cpp -o $(@)

object_files/transform_component.o: ../src/framework/components/transform_component.cpp
	g++ -O1 -Wall -Wno-missing-braces -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/components/transform_component.cpp -o $(@)

object_files/trails.o: ../src/framework/objects/trails.cpp
	g++ -O1 -Wall -Wno-missing-braces -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/objects/trails.cpp -o $(@)

object_files/sprites.o: ../src/framework/objects/sprites.cpp
	g++ -O1 -Wall -Wno-missing-braces -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/objects/sprites.cpp -o $(@)

object_files/particles.o: ../src/framework/objects/particles.cpp
	g++ -O1 -Wall -Wno-missing-braces -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/objects/particles.cpp -o $(@)

object_files/test_entity.o: ../src/framework/entities/test_entity.cpp
	g++ -O1 -Wall -Wno-missing-braces -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/entities/test_entity.cpp -o $(@)

Build.exe: object_files/main.o object_files/entity.o object_files/component.o object_files/scene.o object_files/misc.o object_files/health_component.o object_files/transform_component.o object_files/trails.o object_files/sprites.o object_files/particles.o object_files/test_entity.o 
	g++ -o Build.exe object_files/main.o object_files/entity.o object_files/component.o object_files/scene.o object_files/misc.o object_files/health_component.o object_files/transform_component.o object_files/trails.o object_files/sprites.o object_files/particles.o object_files/test_entity.o -L../lib/ -lraylib -lopengl32 -lgdi32 -lwinmm

.PHONY: clean
clean:
	del object_files/main.o 
	del object_files/entity.o 
	del object_files/component.o 
	del object_files/scene.o 
	del object_files/misc.o 
	del object_files/health_component.o 
	del object_files/transform_component.o 
	del object_files/trails.o 
	del object_files/sprites.o 
	del object_files/particles.o 
	del object_files/test_entity.o 
