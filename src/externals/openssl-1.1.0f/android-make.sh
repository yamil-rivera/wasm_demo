#!/usr/bin/env bash

set -u
set -e

function finish {
    popd
}
trap finish EXIT

pushd "$( dirname "${BASH_SOURCE[0]}" )"

if [[ -z "${ANDROID_ABI+notempty}" ]]; then
    ANDROID_ABI=x86
fi

# shellcheck disable=SC1091
source ../../../android-config.sh || { echo "config failed"; exit 1; }

echo "ANDROID_TOOLCHAIN_ROOT: ${ANDROID_TOOLCHAIN_ROOT}"
if [[ ! -d "${ANDROID_TOOLCHAIN_ROOT}/bin" ]]; then
    echo "Android toolchain not found."
    exit 1
fi

echo "ANDROID_SYSROOT: ${ANDROID_SYSROOT}"
if [[ ! -d "${ANDROID_SYSROOT}/usr" ]]; then
    echo "Android sysroot not found."
    exit 1
fi

export CROSS_SYSROOT="${ANDROID_SYSROOT}"

export PATH=${ANDROID_TOOLCHAIN_ROOT}/bin:$PATH

make "$@"

