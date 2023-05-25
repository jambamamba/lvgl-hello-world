#!/bin/bash -xe
set -xe

export SCREEN_HOR_RES=800 
export SCREEN_VER_RES=480

mkdir -p x86-build
pushd x86-build
cmake -DUSE_WAYLAND=1 -G Ninja ..
ninja
popd

