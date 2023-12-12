rem Filename processing, remove extension and store core filename to variable "filename"
rem Uses iterator over the 1st argument "%1"
for %%f in (%1) do (
    set "filename=%%~nf"
)

rem Perform g++ build
rem build filename.cpp into filename.o, create object code including libraries and c++ standard libraries (static) 
g++ -c ./src/%filename%.cpp -ISFML-2.6.0/include -Iinclude -static-libgcc -static-libstdc++
rem link object code, build into executable (still with c++ standard libraries static)
g++ %filename%.o -o %filename% -LSFML-2.6.0/lib -lsfml-graphics -lsfml-window -lsfml-system -lopengl32 -static-libgcc -static-libstdc++

rem custom check, moves main.cpp into the build folder, every other cpp builds into debug
rem useful to just ensure that we separate actual builds from any test files
if "%filename%" == "main" (
    set "a=build"   
) else (
    set "a=debug"
)

rem Move the executable into the build folder
move "%filename%.exe" "./%a%/"
rem Set our working directory to main folder
setlocal
rem Remove the object code (cleanup)
del "%filename%.o"
rem Set our working directory to the build folder
cd "%a%"
rem Call our executable
call "%filename%.exe"