rem Build options using local SFML
g++ -c ./src/main.cpp -ISFML-2.6.0/include -Iinclude
g++ main.o -o smfl-app -LSFML-2.6.0/lib -lsfml-graphics -lsfml-window -lsfml-system

rem Recreate build
if exist "build" rmdir /s /q "build"
if not exist "build" mkdir "build"

rem Move exe to parent directory
for %%F in ("%~dp0\*.exe") do move "%%F" "%~dp0build\"

rem Copy relevant DLLs from SFML
setlocal
set "fileNames=sfml-window-2.dll sfml-graphics-2.dll sfml-system-2.dll"
for %%A in ("%~dp0.") do set "batchDir=%%~fA"
set "sourceDir=SFML-2.6.0\bin"
set "buildDir=build"
for %%F in (%fileNames%) do (
    copy /Y "%sourceDir%\%%F" "%buildDir%\"
)

rem Remove object code
del main.o

rem Run the exe in the build path
set "folder_path=build"

for %%i in ("%folder_path%\*.exe") do (
    echo Running "%%~nxi"
    call "%%i"
)