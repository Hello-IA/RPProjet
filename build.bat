@echo off
setlocal enabledelayedexpansion

echo ===== Script de compilation intelligente avec suivi des dépendances =====
set PATH=%PATH%;C:\msys64\mingw64\bin

set CXX=g++
set CXXFLAGS=-Wall -Wextra -std=c++11
set GRAPHVIZ_INCLUDE="C:\Program Files\Graphviz\include"
set GRAPHVIZ_LIB="C:\Program Files\Graphviz\lib"
set LIBS=-lstdc++ -lm -lgvc -lcgraph -lcdt
set LDFLAGS=-Wl,--allow-multiple-definition
set TARGET=graphapp.exe
set NEED_LINK=0
set DEPDIR=.deps

:: Créer les répertoires nécessaires
if not exist obj mkdir obj
if not exist %DEPDIR% mkdir %DEPDIR%

echo.
echo Vérification et compilation des fichiers source modifiés...

:: Liste tous les fichiers .cpp dans le répertoire actuel et tous les sous-répertoires
for /r %%F in (*.cpp) do (
    set "cppfile=%%F"
    set "objfile=obj\%%~nF.o"
    set "depfile=%DEPDIR%\%%~nF.d"
    set "NEED_COMPILE=0"
    
    :: Vérifier si le fichier objet existe
    if not exist "!objfile!" (
        set NEED_COMPILE=1
        echo Le fichier objet !objfile! n'existe pas, compilation requise.
    ) else (
        :: Vérifier si le fichier .cpp est plus récent que le .o
        for %%A in ("!cppfile!") do set CPP_DATE=%%~tA
        for %%B in ("!objfile!") do set OBJ_DATE=%%~tB
        
        if "!CPP_DATE!" gtr "!OBJ_DATE!" (
            set NEED_COMPILE=1
            echo Le fichier !cppfile! a été modifié, compilation requise.
        )

        :: Vérifier aussi les fichiers d'en-tête inclus
        if exist "!depfile!" (
            for /f "tokens=*" %%H in (!depfile!) do (
                if exist "%%H" (
                    for %%I in ("%%H") do set HEADER_DATE=%%~tI
                    if "!HEADER_DATE!" gtr "!OBJ_DATE!" (
                        set NEED_COMPILE=1
                        echo Le fichier d'en-tête %%H a été modifié, recompilation de !cppfile!.
                    )
                )
            )
        ) else (
            :: Fichier de dépendances n'existe pas, on doit compiler
            set NEED_COMPILE=1
            echo Fichier de dépendances manquant pour !cppfile!, compilation requise.
        )
    )
    
    :: Compiler si nécessaire
    if !NEED_COMPILE! equ 1 (
        echo Compilation de !cppfile! ...
        
        :: Extraire et sauvegarder les dépendances
        %CXX% -MM -I%GRAPHVIZ_INCLUDE% -I. "!cppfile!" > "!depfile!"
        
        :: Compiler le fichier
        %CXX% -c %CXXFLAGS% -I%GRAPHVIZ_INCLUDE% -I. "!cppfile!" -o "!objfile!"
        if errorlevel 1 (
            echo Erreur lors de la compilation de !cppfile!
            exit /b 1
        )
        set NEED_LINK=1
    )
)

:: Liaison si nécessaire
if %NEED_LINK% equ 1 (
    echo.
    echo Liaison des fichiers objets...
    %CXX% -o %TARGET% obj\*.o -L%GRAPHVIZ_LIB% %LIBS% %LDFLAGS%
    if errorlevel 1 (
        echo Erreur lors de la liaison
        exit /b 1
    )
    echo.
    echo Construction terminée avec succès !
    echo L'exécutable %TARGET% a été créé.
) else (
    echo.
    echo Aucun fichier n'a été modifié, la liaison n'est pas nécessaire.
    echo Le programme %TARGET% est à jour.
)

endlocal
