@echo off
cls
g++ "src\%1" -o "bin\%~n1.exe"
if errorlevel 1 (
    echo Compilation failed.
) else (
    "bin\%~n1.exe"
    echo.
    echo.
)
pause
exit