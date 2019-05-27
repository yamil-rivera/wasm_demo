#!/bin/sh
make -C /Users/yamil.rivera/Repos/wasm_demo/build-darwin -f /Users/yamil.rivera/Repos/wasm_demo/build-darwin/CMakeScripts/ALL_BUILD_cmakeRulesBuildPhase.make$CONFIGURATION OBJDIR=$(basename "$OBJECT_FILE_DIR_normal") all
