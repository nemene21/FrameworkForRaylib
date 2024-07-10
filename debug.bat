@echo off
cd /d "build"
start cmd /k "color 3 && gdb build.exe && set cwd"