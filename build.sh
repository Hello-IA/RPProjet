#!/bin/bash

echo "===== Script de compilation ====="

CXX=g++
CXXFLAGS="-w -Wall -Wextra -std=c++17 -mmacosx-version-min=14.0"
GRAPHVIZ_INCLUDE="/usr/local/include/graphviz"
GRAPHVIZ_LIB="/usr/local/lib"
LIBS="-lgvc -lcgraph -lcdt"
LDFLAGS=""
TARGET="graphapp"

echo
echo "Compilation des fichiers source..."
for file in $(find . -name "*.cpp"); do
    echo "Compilation de $file"
    $CXX -c $CXXFLAGS -I"$GRAPHVIZ_INCLUDE" "$file"
    if [ $? -ne 0 ]; then
        echo "Erreur lors de la compilation de $file"
        exit 1
    fi
done

echo
echo "Liaison des fichiers objets..."
$CXX -o "$TARGET" *.o -L"$GRAPHVIZ_LIB" $LIBS $LDFLAGS
if [ $? -ne 0 ]; then
    echo "Erreur lors de la liaison"
    exit 1
fi

echo
echo "Construction terminée avec succès !"
echo "L'exécutable $TARGET a été créé."
