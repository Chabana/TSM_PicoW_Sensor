#!/bin/env bash
set -ex

# build test binary
cmake --preset test
cmake --build --preset app-test

# fill-in memory address of `program_arg` variable
PROGRAM_ARG_ADDR="0x$(nm build/test/TSM_PicoW_Sensor.elf | grep program_arg | awk '{ print $1 }')"
find src/tests/ -name '*.JLinkScript' -exec sed --regexp-extended -i "s/0x([0-9]+)/${PROGRAM_ARG_ADDR}/g" {} \;

# run tests
ctest -v --test-dir build/test --timeout 120 --output-on-failure
