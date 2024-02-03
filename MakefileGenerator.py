CFLAGS   = "-O1 -Wall -Wno-missing-braces -I../include/"
LDFLAGS  = "-L../lib/"
LDLIBS   = "-lraylib -lopengl32 -lgdi32 -lwinmm"
OBJ_DIR  = "object_files"
SRC_DIR  = "../src"
COMPILER = "g++"

makefile = ""

source_files = [
    "main",
    "entity",
    "component",
    "test/test"
]

def next(n=1):
    global makefile
    for i in range(n):
        makefile += "\n"

def tab():
    global makefile
    makefile += "\t"

def add(cont):
    global makefile
    makefile += cont

add("all: Build.exe")
next(2)

for source in source_files:
    add(f"{OBJ_DIR}/{source}.o: {SRC_DIR}/{source}.cpp")
    next(); tab()

    add(f"{COMPILER} {CFLAGS} -c {SRC_DIR}/{source}.cpp -o $(@)")
    next(2)

add("Build.exe: ")
for source in source_files:
    add(OBJ_DIR + "/" + source + ".o ")

next(); tab()
add(f"{COMPILER} -o Build.exe ")
for source in source_files:
    add(OBJ_DIR + "/" + source + ".o ")

add(LDFLAGS + " " + LDLIBS)

file = open("build/Makefile", "w")
file.write(makefile)
file.close()