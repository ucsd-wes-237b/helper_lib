@ECHO OFF

set COMPILER=clang

set C_INCLUDE_PATH=C:\OpenCL-SDK\include
set CPLUS_INCLUDE_PATH=C:\OpenCL-SDK\include
set LIBRARY_PATH=C:\OpenCL-SDK\lib

%COMPILER% *.c -c -L%LIBRARY_PATH% -lOpenCL