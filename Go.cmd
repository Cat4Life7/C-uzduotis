if .%USERDOMAIN% == .NOJUS goto :savasPC
path C:\PROGRA~2\Dev-Cpp\MinGW64\bin\;%path%
mingw32-make.exe
goto :toliau

:savasPC
mingw32-make

:toliau
test.exe > test.txt
pause
