import tkinter, subprocess

CFLAGS   = "-O1 -Wall -Wno-missing-braces -I../include/"
LDFLAGS  = "-L../lib/"
LDLIBS   = "-lraylib -lopengl32 -lgdi32 -lwinmm"
OBJ_DIR  = "object_files"
SRC_DIR  = "../src"
COMPILER = "g++"

makefile = ""

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

def generate_makefile():
    global makefile

    print("Starting...")
    source_list_file = open("SourceList", "r")
    source_files = []

    for source in source_list_file.read().split("\n"):
        if source != "":
            source_files.append(source)

    source_list_file.close()
    print("Opened Source List...")

    makefile = ""

    add("all: Build.exe")
    next(2)

    for source in source_files:
        add(f"{OBJ_DIR}/{get_filename(source)}.o: {SRC_DIR}/{source}.cpp")
        next(); tab()

        add(f"{COMPILER} {CFLAGS} -c {SRC_DIR}/{source}.cpp -o $(@)")
        next(2)

    add("Build.exe: ")
    for source in source_files:
        add(OBJ_DIR + "/" + get_filename(source) + ".o ")

    next(); tab()
    add(f"{COMPILER} -o Build.exe ")
    for source in source_files:
        add(OBJ_DIR + "/" + get_filename(source) + ".o ")

    add(LDFLAGS + " " + LDLIBS)

    print("Generated...")

    file = open("build/Makefile", "w")
    file.write(makefile)
    file.close()

    print("Saved...\n")

def make():
    result = subprocess.run("make", shell=True, cwd="build")
    print("Make done?")
    print("")

def get_filename(string):
    return string.split("/")[-1]

def run():
    result = subprocess.run("Build.exe", shell=True, cwd="build")

    if result:
        print("Game Sucessfully Ran!")
    else:
        print("Game didnt' Run :(")
    print("")

window = tkinter.Tk()
window.configure(bg='#333333')
window.geometry("512x512")
window.title("Auto Compiling and Running!")

font = ("Helvetica", 24)

generate_button = tkinter.Button(window, text="Generate Makefile", command=generate_makefile, font=font)
generate_button.place(x = 16, y = 16)

make_button = tkinter.Button(window, text="Make", command=make, font=font)
make_button.place(x = 16, y = 16 + 80)

run_button = tkinter.Button(window, text="Run", command=run, font=font)
run_button.place(x = 16, y = 16 + 160)

window.mainloop()