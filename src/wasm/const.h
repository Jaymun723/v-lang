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
  WASM_CONST(end, 0x0b)                                                        \
  WASM_CONST(i32_const, 0x41)                                                  \
  WASM_CONST(i64_const, 0x42)                                                  \
  WASM_CONST(f32_const, 0x43)                                                  \
  WASM_CONST(f64_const, 0x44)                                                  \
  WASM_CONST(i32_neg, 0x47)                                                    \
  WASM_CONST(i32_add, 0x6a)                                                    \
  WASM_CONST(i32_sub, 0x6b)                                                    \
  WASM_CONST(i32_mul, 0x6c)                                                    \
  WASM_CONST(i32_div_s, 0x6d)                                                  \
  WASM_CONST(i32_rem_s, 0x6f)                                                  \
  WASM_CONST(call, 0x10)

typedef enum WasmConst {
  FOREACH_WASM_CONST(GENERATE_WASM_CONST_ENUM) WasmConstCount
} WasmConst;

extern const char *WASM_CONST_STRING[];

extern const unsigned char WASM_CONST_CODE[];