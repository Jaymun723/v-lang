#pragma once
#include "ast/program.h"
#include "func_mapper.h"
#include <stdbool.h>

bool validateIfStatement(FuncMapper *fm, AstStatement *stmt);
bool validateStmtList(FuncMapper *fm, AstStatementList *stmtList);
bool validate(FuncMapper *fm, AstProgram *program);