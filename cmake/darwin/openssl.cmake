message(STATUS "Configuring OpenSSL")

SET(OPENSSL_ROOT_DIR ${WASM_DEMO_EXTERNALS}/openssl-1.1.0f-darwin)
SET(OPENSSL_USE_STATIC_LIBS TRUE)

message(STATUS "OPENSSL_ROOT_DIR: ${OPENSSL_ROOT_DIR}")

find_package(OpenSSL REQUIRED)
