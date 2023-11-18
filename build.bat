rem Filename processing, remove extension
for %%f in (%1) do (
    set "filename=%%~nf"
)

rem Build using local sfml
g++ -c ./src/%filename%.cpp -ISFML-2.6.0/include -Iinclude
g++ %filename%.o -o %filename% -LSFML-2.6.0/lib -lsfml-graphics -lsfml-window -lsfml-system -lopengl32

if "%filename%" == "main" (
    set "a=build"
) else (
    set "a=debug"
)

del "./%a%/%filename%.exe"
move "%filename%.exe" "./%a%/"
setlocal
del "%filename%.o"
cd "%a%"
call "%filename%.exe"