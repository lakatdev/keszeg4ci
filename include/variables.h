#ifndef INTERPRETER_VARIABLES_H
#define INTERPRETER_VARIABLES_H

#include <types.h>
#include <config.h>

typedef struct {
    char name[INTERPRETER_MAX_NAME];
    Interpreter_VarType type;
    Interpreter_Value value;
} Interpreter_Variable;

typedef struct {
    char name[INTERPRETER_MAX_NAME];
    Interpreter_VarType type;
    int is_ref;
} Interpreter_Parameter;

#endif
