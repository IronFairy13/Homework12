#!/bin/sh
BINARY_HOME=${BINARY_HOME:-./build/bin}
INPUT_HOME=./input
INPUT=${INPUT_HOME}/AB_NYC_2019.csv

BINARY_FALLBACK=./bin

if [ ! -x "${BINARY_HOME}/mapper" ] || [ ! -x "${BINARY_HOME}/reducer" ]; then
    if [ -x "${BINARY_FALLBACK}/mapper" ] && [ -x "${BINARY_FALLBACK}/reducer" ]; then
        BINARY_HOME=${BINARY_FALLBACK}
    else
        echo "Mapper and reducer binaries not found in ${BINARY_HOME} or fallback ${BINARY_FALLBACK}." >&2
        echo "Build the project with CMake, e.g.:" >&2
        echo "  cmake -S . -B build && cmake --build build" >&2
        exit 1
    fi
fi

cat ${INPUT} | ${BINARY_HOME}/mapper | sort -k1 | ${BINARY_HOME}/reducer > output
cat output
