rem Build options using local SFML
g++ -c ../main.cpp -I../SFML-2.6.0/include
g++ main.o -o smfl-app -L../SFML-2.6.0/lib -lsfml-graphics -lsfml-window -lsfml-system

rem Bin directory creation if necessary
if not exist "..\bin" mkdir "..\bin"

rem Move exe to parent directory
for %%F in ("%~dp0\*.exe") do move "%%F" "%~dp0..\bin\"

rem Copy relevant DLLs from SFML
setlocal
set "fileNames=sfml-window-2.dll sfml-graphics-2.dll sfml-system-2.dll"
for %%A in ("%~dp0.") do set "batchDir=%%~fA"
set "sourceDir=..\SFML-2.6.0\bin"
set "binDir=..\bin"
for %%F in (%fileNames%) do (
    copy /Y "%sourceDir%\%%F" "%binDir%\"
)