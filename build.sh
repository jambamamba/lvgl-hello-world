#!/bin/bash -xe
set -xe

function parseArgs(){
   for change in "$@"; do
      local name="${change%%=*}"
      local value="${change#*=}"
      eval $name="$value"
   done
}

function setupVscode(){
mkdir -p .vscode
echo '[
    {
        "name": "x86-build",
        "compilers": {
            "C": "/usr/bin/clang",
            "CXX": "/usr/bin/clang++",
        },
        "environmentVariables":{
            "CMAKE_PREFIX_PATH":"${workspaceRoot}/share/cmake-modules",
            "CMAKE_MODULE_PATH":"${workspaceRoot}/share/cmake-modules",
            "SCREEN_HOR_RES":"800",
            "SCREEN_VER_RES":"480"
        }
    }
]
' >  .vscode/cmake-kits.json

echo '{
    "version":"0.2.0",
    "configurations": [
        {
            "name":"lldb",
            "type":"lldb",
            "request":"launch",
            "program":"./x86-build/hello-world",
            "args":[
            ],
            "cwd": "${workspaceRoot}",
        }
    ]
}
' > .vscode/launch.json

echo '{
    "cmake.buildEnvironment": {

    },
    "cmake.buildDirectory": "${workspaceFolder}/${buildKit}"
}
' > .vscode/settings.json 
}

function buildx86(){
    parseArgs $@
    mkdir -p x86-build
    pushd x86-build
    cmake -G Ninja ..
    ninja
    popd
}

function buildarm(){
    parseArgs $@
    mkdir -p arm-build
    pushd arm-build
    source /opt/usr_data/sdk/environment-setup-cortexa72-oe-linux
    cmake \
        -DCMAKE_SYSTEM_PROCESSOR="aarch64" \
        -DUSE_WAYLAND=1 \
        -G Ninja ..
    ninja
    popd
}

function main(){
    parseArgs $@

    # setupVscode
    export SCREEN_HOR_RES=800 
    export SCREEN_VER_RES=480

    if [ "${target}" == "arm" ];then
        buildarm
    else
        buildx86
    fi
}

main $@