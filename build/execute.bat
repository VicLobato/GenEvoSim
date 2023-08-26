set "folder_path=../bin"

for %%i in ("%folder_path%\*.exe") do (
    echo Running "%%~nxi"
    call "%%i"
)