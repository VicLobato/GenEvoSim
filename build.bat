rem Build options using local SFML
g++ -c ./src/main.cpp -ISFML-2.6.0/include -Iinclude
g++ main.o -o program -LSFML-2.6.0/lib -lsfml-graphics -lsfml-window -lsfml-system

rem Remove current exe
del .\build\program.exe

rem Move exe to parent directory
for %%F in ("%~dp0\*.exe") do move "%%F" "%~dp0build\"

rem Copy assets to build
setlocal

rem Remove object code
del main.o

rem Run the exe in the build path
set "folder_path=build"

for %%i in ("%folder_path%\*.exe") do (
    echo Running "%%~nxi"
    call "%%i"
)