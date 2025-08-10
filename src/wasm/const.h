#pragma once

#define GENERATE_WASM_CONST_ENUM(ENUM, CODE) WasmConst_##ENUM,
#define GENERATE_WASM_CONST_STRING(ENUM, CODE) #ENUM,
#define GENERATE_WASM_CONST_CODE(ENUM, CODE) CODE,

#define FOREACH_WASM_CONST(WASM_CONST)                                         \
  WASM_CONST(i32, 0x7f)                                                        \
  WASM_CONST(i64, 0x7e)                                                        \
  WASM_CONST(f32, 0x7d)                                                        \
  WASM_CONST(f64, 0x7c)                                                        \
  WASM_CONST(v128, 0x7b)                                                       \
  WASM_CONST(funcref, 0x70)                                                    \
  WASM_CONST(externref, 0x6f)                                                  \
  WASM_CONST(functiontype, 0x60)                                               \
  WASM_CONST(end, 0x0b)

typedef enum WasmConst {
  FOREACH_WASM_CONST(GENERATE_WASM_CONST_ENUM) WasmConstCount
} WasmConst;

static const char *WASM_CONST_STRING[] = {
    FOREACH_WASM_CONST(GENERATE_WASM_CONST_STRING)};

static const unsigned char WASM_CONST_CODE[] = {
    FOREACH_WASM_CONST(GENERATE_WASM_CONST_CODE)};