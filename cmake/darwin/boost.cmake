enable_language(ASM)

set(BOOST_CONTEXT_PLATFORM_FILES
        libs/context/src/posix/stack_traits.cpp
        libs/context/src/asm/jump_x86_64_sysv_macho_gas.S
        libs/context/src/asm/make_x86_64_sysv_macho_gas.S
        libs/context/src/asm/ontop_x86_64_sysv_macho_gas.S)

set(PLATFORM_COMPILER_FLAGS)
