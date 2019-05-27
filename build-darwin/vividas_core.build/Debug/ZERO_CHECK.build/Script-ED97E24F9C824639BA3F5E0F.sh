#!/bin/sh
make -C /Users/yamil.rivera/Repos/wasm_demo/build-darwin -f /Users/yamil.rivera/Repos/wasm_demo/build-darwin/CMakeScripts/ZERO_CHECK_cmakeRulesBuildPhase.make$CONFIGURATION OBJDIR=$(basename "$OBJECT_FILE_DIR_normal") all
