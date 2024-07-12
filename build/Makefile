all: Build.exe

web_object_files/component.o: ../src/framework/component.cpp
	emcc -g -O1 -Wall -Wno-missing-braces -I../src/game/ -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/component.cpp -o $(@)

web_object_files/animation_component.o: ../src/framework/components/animation_component.cpp
	emcc -g -O1 -Wall -Wno-missing-braces -I../src/game/ -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/components/animation_component.cpp -o $(@)

web_object_files/area_component.o: ../src/framework/components/area_component.cpp
	emcc -g -O1 -Wall -Wno-missing-braces -I../src/game/ -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/components/area_component.cpp -o $(@)

web_object_files/camera_component.o: ../src/framework/components/camera_component.cpp
	emcc -g -O1 -Wall -Wno-missing-braces -I../src/game/ -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/components/camera_component.cpp -o $(@)

web_object_files/collider_component.o: ../src/framework/components/collider_component.cpp
	emcc -g -O1 -Wall -Wno-missing-braces -I../src/game/ -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/components/collider_component.cpp -o $(@)

web_object_files/health_component.o: ../src/framework/components/health_component.cpp
	emcc -g -O1 -Wall -Wno-missing-braces -I../src/game/ -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/components/health_component.cpp -o $(@)

web_object_files/timer_component.o: ../src/framework/components/timer_component.cpp
	emcc -g -O1 -Wall -Wno-missing-braces -I../src/game/ -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/components/timer_component.cpp -o $(@)

web_object_files/transform_component.o: ../src/framework/components/transform_component.cpp
	emcc -g -O1 -Wall -Wno-missing-braces -I../src/game/ -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/components/transform_component.cpp -o $(@)

web_object_files/tilemap_entity.o: ../src/framework/entities/tilemap_entity.cpp
	emcc -g -O1 -Wall -Wno-missing-braces -I../src/game/ -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/entities/tilemap_entity.cpp -o $(@)

web_object_files/tilemap_renderer.o: ../src/framework/entities/tilemap_renderer.cpp
	emcc -g -O1 -Wall -Wno-missing-braces -I../src/game/ -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/entities/tilemap_renderer.cpp -o $(@)

web_object_files/entity.o: ../src/framework/entity.cpp
	emcc -g -O1 -Wall -Wno-missing-braces -I../src/game/ -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/entity.cpp -o $(@)

web_object_files/framework.o: ../src/framework/framework.cpp
	emcc -g -O1 -Wall -Wno-missing-braces -I../src/game/ -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/framework.cpp -o $(@)

web_object_files/imgui.o: ../src/framework/imgui/imgui.cpp
	emcc -g -O1 -Wall -Wno-missing-braces -I../src/game/ -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/imgui/imgui.cpp -o $(@)

web_object_files/imgui_demo.o: ../src/framework/imgui/imgui_demo.cpp
	emcc -g -O1 -Wall -Wno-missing-braces -I../src/game/ -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/imgui/imgui_demo.cpp -o $(@)

web_object_files/imgui_draw.o: ../src/framework/imgui/imgui_draw.cpp
	emcc -g -O1 -Wall -Wno-missing-braces -I../src/game/ -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/imgui/imgui_draw.cpp -o $(@)

web_object_files/imgui_tables.o: ../src/framework/imgui/imgui_tables.cpp
	emcc -g -O1 -Wall -Wno-missing-braces -I../src/game/ -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/imgui/imgui_tables.cpp -o $(@)

web_object_files/imgui_widgets.o: ../src/framework/imgui/imgui_widgets.cpp
	emcc -g -O1 -Wall -Wno-missing-braces -I../src/game/ -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/imgui/imgui_widgets.cpp -o $(@)

web_object_files/rlImGui.o: ../src/framework/imgui/rlImGui.cpp
	emcc -g -O1 -Wall -Wno-missing-braces -I../src/game/ -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/imgui/rlImGui.cpp -o $(@)

web_object_files/input.o: ../src/framework/input.cpp
	emcc -g -O1 -Wall -Wno-missing-braces -I../src/game/ -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/input.cpp -o $(@)

web_object_files/misc.o: ../src/framework/misc.cpp
	emcc -g -O1 -Wall -Wno-missing-braces -I../src/game/ -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/misc.cpp -o $(@)

web_object_files/audio.o: ../src/framework/objects/audio.cpp
	emcc -g -O1 -Wall -Wno-missing-braces -I../src/game/ -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/objects/audio.cpp -o $(@)

web_object_files/drawables.o: ../src/framework/objects/drawables.cpp
	emcc -g -O1 -Wall -Wno-missing-braces -I../src/game/ -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/objects/drawables.cpp -o $(@)

web_object_files/particles.o: ../src/framework/objects/particles.cpp
	emcc -g -O1 -Wall -Wno-missing-braces -I../src/game/ -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/objects/particles.cpp -o $(@)

web_object_files/progress_bar.o: ../src/framework/objects/progress_bar.cpp
	emcc -g -O1 -Wall -Wno-missing-braces -I../src/game/ -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/objects/progress_bar.cpp -o $(@)

web_object_files/sprites.o: ../src/framework/objects/sprites.cpp
	emcc -g -O1 -Wall -Wno-missing-braces -I../src/game/ -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/objects/sprites.cpp -o $(@)

web_object_files/trails.o: ../src/framework/objects/trails.cpp
	emcc -g -O1 -Wall -Wno-missing-braces -I../src/game/ -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/objects/trails.cpp -o $(@)

web_object_files/ui_drawables.o: ../src/framework/objects/ui_drawables.cpp
	emcc -g -O1 -Wall -Wno-missing-braces -I../src/game/ -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/objects/ui_drawables.cpp -o $(@)

web_object_files/rich_presence.o: ../src/framework/rich_presence.cpp
	emcc -g -O1 -Wall -Wno-missing-braces -I../src/game/ -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/rich_presence.cpp -o $(@)

web_object_files/scene.o: ../src/framework/scene.cpp
	emcc -g -O1 -Wall -Wno-missing-braces -I../src/game/ -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/framework/scene.cpp -o $(@)

web_object_files/player.o: ../src/game/entities/player.cpp
	emcc -g -O1 -Wall -Wno-missing-braces -I../src/game/ -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/game/entities/player.cpp -o $(@)

web_object_files/game.o: ../src/game/scenes/game.cpp
	emcc -g -O1 -Wall -Wno-missing-braces -I../src/game/ -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/game/scenes/game.cpp -o $(@)

web_object_files/main.o: ../src/main.cpp
	emcc -g -O1 -Wall -Wno-missing-braces -I../src/game/ -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/ -c ../src/main.cpp -o $(@)

Build.exe: web_object_files/component.o web_object_files/animation_component.o web_object_files/area_component.o web_object_files/camera_component.o web_object_files/collider_component.o web_object_files/health_component.o web_object_files/timer_component.o web_object_files/transform_component.o web_object_files/tilemap_entity.o web_object_files/tilemap_renderer.o web_object_files/entity.o web_object_files/framework.o web_object_files/imgui.o web_object_files/imgui_demo.o web_object_files/imgui_draw.o web_object_files/imgui_tables.o web_object_files/imgui_widgets.o web_object_files/rlImGui.o web_object_files/input.o web_object_files/misc.o web_object_files/audio.o web_object_files/drawables.o web_object_files/particles.o web_object_files/progress_bar.o web_object_files/sprites.o web_object_files/trails.o web_object_files/ui_drawables.o web_object_files/rich_presence.o web_object_files/scene.o web_object_files/player.o web_object_files/game.o web_object_files/main.o 
	emcc -g -O1 -Wall -Wno-missing-braces -I../src/game/ -I../src/framework/components/ -I../include/ -I../src/ -I../src/framework/ -I../src/framework/objects/ -I../src/framework/entities/../src/main.cpp web_object_files/component.o web_object_files/animation_component.o web_object_files/area_component.o web_object_files/camera_component.o web_object_files/collider_component.o web_object_files/health_component.o web_object_files/timer_component.o web_object_files/transform_component.o web_object_files/tilemap_entity.o web_object_files/tilemap_renderer.o web_object_files/entity.o web_object_files/framework.o web_object_files/imgui.o web_object_files/imgui_demo.o web_object_files/imgui_draw.o web_object_files/imgui_tables.o web_object_files/imgui_widgets.o web_object_files/rlImGui.o web_object_files/input.o web_object_files/misc.o web_object_files/audio.o web_object_files/drawables.o web_object_files/particles.o web_object_files/progress_bar.o web_object_files/sprites.o web_object_files/trails.o web_object_files/ui_drawables.o web_object_files/rich_presence.o web_object_files/scene.o web_object_files/player.o web_object_files/game.o web_object_files/main.o -o web_build/index.html -s ASSERTIONS=1 -s SAFE_HEAP=1 -s USE_GLFW=3 -s ASYNCIFY -s WASM=1 -s TOTAL_MEMORY=2048MB --shell-file shell.html --preload-file assets -DWEB -L../libweb/ -lraylib

.PHONY: clean
clean:
	del /S /Q web_object_files\component.o
	del /S /Q web_object_files\animation_component.o
	del /S /Q web_object_files\area_component.o
	del /S /Q web_object_files\camera_component.o
	del /S /Q web_object_files\collider_component.o
	del /S /Q web_object_files\health_component.o
	del /S /Q web_object_files\timer_component.o
	del /S /Q web_object_files\transform_component.o
	del /S /Q web_object_files\tilemap_entity.o
	del /S /Q web_object_files\tilemap_renderer.o
	del /S /Q web_object_files\entity.o
	del /S /Q web_object_files\framework.o
	del /S /Q web_object_files\imgui.o
	del /S /Q web_object_files\imgui_demo.o
	del /S /Q web_object_files\imgui_draw.o
	del /S /Q web_object_files\imgui_tables.o
	del /S /Q web_object_files\imgui_widgets.o
	del /S /Q web_object_files\rlImGui.o
	del /S /Q web_object_files\input.o
	del /S /Q web_object_files\misc.o
	del /S /Q web_object_files\audio.o
	del /S /Q web_object_files\drawables.o
	del /S /Q web_object_files\particles.o
	del /S /Q web_object_files\progress_bar.o
	del /S /Q web_object_files\sprites.o
	del /S /Q web_object_files\trails.o
	del /S /Q web_object_files\ui_drawables.o
	del /S /Q web_object_files\rich_presence.o
	del /S /Q web_object_files\scene.o
	del /S /Q web_object_files\player.o
	del /S /Q web_object_files\game.o
	del /S /Q web_object_files\main.o
