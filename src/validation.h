#pragma once
#include "ast/program.h"
#include "func_mapper.h"
#include <stdbool.h>

bool validate(FuncMapper *fm, AstProgram *program);