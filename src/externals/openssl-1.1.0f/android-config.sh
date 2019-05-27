#!/usr/bin/env bash

set -u
set -e

function finish {
    popd
}
trap finish EXIT

pushd "$( dirname "${BASH_SOURCE[0]}" )"

[ -x "$(which perl)" ] || { echo "perl not found"; exit 1; }

if [[ -z "${ANDROID_ABI+notempty}" ]]; then
    ANDROID_ABI=x86
    echo "Set ANDROID_ABI to default (${ANDROID_ABI})"
fi

if [[ -z "${PREFIX+notempty}" ]]; then
    PREFIX="$(pwd)/build/${ANDROID_ABI}"
fi

case "${ANDROID_ABI}" in
    armeabi|armeabi-v7a)
        CONFIG_NAME="android-armeabi-clang"
        ;;
    arm64-v8a)
        CONFIG_NAME="android64-aarch64-clang"
        ;;
    x86)
        CONFIG_NAME="android-x86-clang"
        ;;
    x86_64)
        CONFIG_NAME="android64-x86_64-clang"
        ;;
    mips)
        CONFIG_NAME="android-mips-clang"
        ;;
    mips64)
        CONFIG_NAME="android64-mips64-clang"
        ;;
esac

# # shellcheck disable=SC1091
# source ../../../android-config.sh || { echo "config failed"; exit 1; }

if [[ -z "${ANDROID_TOOLCHAIN_ROOT+notempty}" ]]; then
    echo "Android toolchain not set."
    exit 1
fi

if [[ ! -d "${ANDROID_TOOLCHAIN_ROOT}/bin" ]]; then
    echo "Android toolchain not found."
    exit 1
fi

echo "ANDROID_TOOLCHAIN_ROOT: ${ANDROID_TOOLCHAIN_ROOT}"
echo "ANDROID_SYSROOT: ${ANDROID_SYSROOT}"

export PATH=${ANDROID_TOOLCHAIN_ROOT}/bin:$PATH
export CROSS_SYSROOT="${ANDROID_SYSROOT}"
export CROSS_COMPILE="${ANDROID_TOOLCHAIN_PREFIX}" #${ANDROID_TOOLCHAIN_ROOT}/bin/

CONFIG_PARAMS="no-shared no-hw no-comp no-unit-test no-ssl3 no-weak-ssl-ciphers" # no-engine
perl Configure ${CONFIG_NAME} ${CONFIG_PARAMS} --prefix="${PREFIX}"
