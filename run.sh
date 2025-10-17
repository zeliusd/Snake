#!/bin/bash
set -e

BUILD_DIR=build

# Configurar y compilar
cmake -S . -B "$BUILD_DIR" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build "$BUILD_DIR"

if [ $# -eq 0 ]; then
  # Ejecutar el juego por defecto
  echo "Ejecutando juego principal..."
  "$BUILD_DIR/juego"
  exit 0
fi

while getopts "t" opt; do
  case $opt in
  t)
    echo "Ejecutando tests..."
    "$BUILD_DIR/tests"
    ;;
  *)
    usage
    ;;
  esac
done
