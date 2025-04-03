@echo off
echo ===== Script de compilation =====

set CXX=g++
set CXXFLAGS=-Wall -Wextra -std=c++11
set GRAPHVIZ_INCLUDE="C:\Program Files (x86)\Graphviz\include"
set GRAPHVIZ_LIB="C:\Program Files (x86)\Graphviz\lib"
set LIBS=-lgvc -lcgraph -lcdt
set LDFLAGS=-Wl,--allow-multiple-definition
set TARGET=graphapp.exe

echo.
echo Compilation des fichiers source...
for %%F in (*.cpp) do (
    echo Compilation de %%F
    %CXX% -c %CXXFLAGS% -I%GRAPHVIZ_INCLUDE% "%%F"
    if errorlevel 1 (
        echo Erreur lors de la compilation de %%F
        exit /b 1
    )
)

echo.
echo Liaison des fichiers objets...
%CXX% -o %TARGET% *.o -L%GRAPHVIZ_LIB% %LIBS% %LDFLAGS%
if errorlevel 1 (
    echo Erreur lors de la liaison
    exit /b 1
)

echo.
echo Construction terminée avec succès !
echo L'exécutable %TARGET% a été créé.