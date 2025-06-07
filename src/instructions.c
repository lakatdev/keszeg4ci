#include <instructions.h>
#include <interpreter.h>
#include <instance.h>
#include <stdio.h>

#include <string.h>

void interpreter_execute_add(Interpreter_Instance* instance, char** tokens, int token_count)
{
    if (token_count < 5) {
        printf("Error: ADD instruction requires more tokens.\n");
        interpreter_halt();
        return;
    }

    const char* dest_var_name = tokens[0];
    const char* op1_token = tokens[2];
    const char* op2_token = tokens[4];

    Interpreter_Value val1 = interpreter_get_value_of_token(instance, op1_token);
    if (val1.type == -1) {
        printf("Error: Operand1 for ADD instruction is invalid or not found.\n");
        interpreter_halt();
        return;
    }

    Interpreter_Value val2 = interpreter_get_value_of_token(instance, op2_token);
    if (val2.type == -1) {
        printf("Error: Operand2 for ADD instruction is invalid or not found.\n");
        interpreter_halt();
        return;
    }

    Interpreter_Value result;
    memset(&result, 0, sizeof(Interpreter_Value));

    if ((val1.type == TYPE_INT || val1.type == TYPE_BYTE) && (val2.type == TYPE_INT || val2.type == TYPE_BYTE)) {
        result.type = TYPE_INT;
        long long v1 = (val1.type == TYPE_INT) ? val1.i : val1.b;
        long long v2 = (val2.type == TYPE_INT) ? val2.i : val2.b;
        result.i = v1 + v2;
    }
    else if ((val1.type == TYPE_FLOAT || val1.type == TYPE_INT || val1.type == TYPE_BYTE) &&
               (val2.type == TYPE_FLOAT || val2.type == TYPE_INT || val2.type == TYPE_BYTE)) {
        result.type = TYPE_FLOAT;
        float f1 = (val1.type == TYPE_FLOAT) ? val1.f : ((val1.type == TYPE_INT) ? (float)val1.i : (float)val1.b);
        float f2 = (val2.type == TYPE_FLOAT) ? val2.f : ((val2.type == TYPE_INT) ? (float)val2.i : (float)val2.b);
        result.f = f1 + f2;
    }
    else {
        printf("Error: Incompatible types for ADD operation. Only numeric types.");
        interpreter_halt();
        return;
    }
    interpreter_set_variable(instance, dest_var_name, result);
}

void interpreter_execute_add_assign(Interpreter_Instance* instance, char** tokens, int token_count)
{
    if (token_count < 3) {
        printf("Error: ADD_ASSIGN instruction requires 3 tokens.\n");
        interpreter_halt();
        return;
    }

    const char* var_name = tokens[0];
    const char* value_token = tokens[2];

    Interpreter_Value* target_var_ptr = interpreter_get_variable(instance, var_name);
    if (target_var_ptr == NULL) {
         printf("Error: Variable '%s' not declared for ADD_ASSIGN operation.\n", var_name);
         interpreter_halt();
         return;
    }
    Interpreter_Value current_val = *target_var_ptr;

    Interpreter_Value val_to_add = interpreter_get_value_of_token(instance, value_token);
    if (val_to_add.type == -1) {
        printf("Error: Value '%s' for ADD_ASSIGN is invalid or not found.\n", value_token);
        interpreter_halt();
        return;
    }

    Interpreter_Value result;
    memset(&result, 0, sizeof(Interpreter_Value));

    if (current_val.type == TYPE_INT) {
        if (val_to_add.type == TYPE_INT) {
            result.type = TYPE_INT;
            result.i = current_val.i + val_to_add.i;
        }
        else if (val_to_add.type == TYPE_BYTE) {
            result.type = TYPE_INT;
            result.i = current_val.i + val_to_add.b;
        }
        else if (val_to_add.type == TYPE_FLOAT) {
            result.type = TYPE_FLOAT;
            result.f = (float)current_val.i + val_to_add.f;
        }
        else {
            printf("Error: Incompatible types for ADD_ASSIGN.\n");
            interpreter_halt();
            return;
        }
    }
    else if (current_val.type == TYPE_FLOAT) {
        if (val_to_add.type == TYPE_FLOAT) {
            result.type = TYPE_FLOAT;
            result.f = current_val.f + val_to_add.f;
        }
        else if (val_to_add.type == TYPE_INT) {
            result.type = TYPE_FLOAT;
            result.f = current_val.f + (float)val_to_add.i;
        }
        else if (val_to_add.type == TYPE_BYTE) {
            result.type = TYPE_FLOAT;
            result.f = current_val.f + (float)val_to_add.b;
        }
        else {
            printf("Error: Incompatible types for ADD_ASSIGN.\n");
            interpreter_halt();
            return;
        }
    }
    else if (current_val.type == TYPE_BYTE) {
        if (val_to_add.type == TYPE_BYTE) {
            result.type = TYPE_INT;
            result.i = (long long)current_val.b + val_to_add.b;
        }
        else if (val_to_add.type == TYPE_INT) {
            result.type = TYPE_INT;
            result.i = (long long)current_val.b + val_to_add.i;
        }
        else if (val_to_add.type == TYPE_FLOAT) {
            result.type = TYPE_FLOAT;
            result.f = (float)current_val.b + val_to_add.f;
        }
        else {
            printf("Error: Incompatible types for ADD_ASSIGN.\n");
            interpreter_halt();
            return;
        }
    } 
    else { 
        printf("Error: Incompatible types for ADD_ASSIGN.\n");
        interpreter_halt();
        return;
    }

    interpreter_set_variable(instance, var_name, result);
    return;
}

void interpreter_execute_array_get(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_array_get\n");}
void interpreter_execute_array_set(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_array_set\n");}

void interpreter_execute_assign(Interpreter_Instance* instance, char** tokens, int token_count)
{
    if (token_count < 3) {
        printf("Error: Assign needs 3 tokens.\n");
        interpreter_halt();
        return;
    }

    const char* dest_var_name = tokens[0];
    const char* value_token = tokens[2];

    Interpreter_Value value_to_assign = interpreter_get_value_of_token(instance, value_token);

    if (value_to_assign.type == -1) {
        printf("Error: Value for assignment is invalid or not found.\n");
        interpreter_halt();
        return;
    }

    interpreter_set_variable(instance, dest_var_name, value_to_assign);
}

void interpreter_execute_call(Interpreter_Instance* instance, char** tokens, int token_count)
{
    printf("interpreter_execute_call\n");

    if (token_count < 2) {
        printf("Error: CALL instruction requires function name\n");
        interpreter_halt();
        return;
    }

    const char* function_name = tokens[1];
    int arg_count = token_count - 2;
    
    char** arg_names = (void*)0;
    if (arg_count > 0) {
        arg_names = &tokens[2];
    }

    int result = interpreter_call_function(instance, function_name, arg_names, arg_count);
    if (result == -1) {
        return;
    }
}

void interpreter_execute_call_assign(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_call_assign\n");}
void interpreter_execute_cat(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_cat\n");}
void interpreter_execute_clear(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_clear\n");}

void interpreter_execute_declare(Interpreter_Instance* instance, char** tokens, int token_count)
{
    if (token_count < 3) {
        printf("Error: DECLARE instruction requires variable name and type (e.g., @ myVar int).\n");
        interpreter_halt();
        return;
    }

    const char* var_name = tokens[1];
    const char* type_str = tokens[2];

    Interpreter_VarType var_type = interpreter_get_type(type_str);

    if (var_type == -1) {
        printf("Error: Unknown variable type '%s' for variable '%s'.\n", type_str, var_name);
        interpreter_halt();
        return;
    }

    interpreter_declare_variable(instance, var_name, var_type);
}

void interpreter_execute_subtract(Interpreter_Instance* instance, char** tokens, int token_count)
{
    if (token_count < 5) {
        printf("Error: SUBTRACT instruction requires 5 tokens.\n");
        interpreter_halt();
        return;
    }

    const char* dest_var_name = tokens[0];
    const char* op1_token = tokens[2];
    const char* op2_token = tokens[4];

    Interpreter_Value val1 = interpreter_get_value_of_token(instance, op1_token);
    if (val1.type == -1) {
        printf("Error: Operand1 for SUBTRACT instruction is invalid or not found.\n");
        interpreter_halt();
        return;
    }

    Interpreter_Value val2 = interpreter_get_value_of_token(instance, op2_token);
    if (val2.type == -1) {
        printf("Error: Operand2 for SUBTRACT instruction is invalid or not found.\n");
        interpreter_halt();
        return;
    }

    Interpreter_Value result;
    memset(&result, 0, sizeof(Interpreter_Value));

    if ((val1.type == TYPE_INT || val1.type == TYPE_BYTE) && (val2.type == TYPE_INT || val2.type == TYPE_BYTE)) {
        result.type = TYPE_INT;
        long long v1 = (val1.type == TYPE_INT) ? val1.i : val1.b;
        long long v2 = (val2.type == TYPE_INT) ? val2.i : val2.b;
        result.i = v1 - v2;
    }
    else if ((val1.type == TYPE_FLOAT || val1.type == TYPE_INT || val1.type == TYPE_BYTE) &&
               (val2.type == TYPE_FLOAT || val2.type == TYPE_INT || val2.type == TYPE_BYTE)) {
        result.type = TYPE_FLOAT;
        float f1 = (val1.type == TYPE_FLOAT) ? val1.f : ((val1.type == TYPE_INT) ? (float)val1.i : (float)val1.b);
        float f2 = (val2.type == TYPE_FLOAT) ? val2.f : ((val2.type == TYPE_INT) ? (float)val2.i : (float)val2.b);
        result.f = f1 - f2;
    }
    else {
        printf("Error: Incompatible types for SUBTRACT operation. Only numeric types.\n");
        interpreter_halt();
        return;
    }
    interpreter_set_variable(instance, dest_var_name, result);
}

void interpreter_execute_subtract_assign(Interpreter_Instance* instance, char** tokens, int token_count)
{
    if (token_count < 3) {
        printf("Error: SUBTRACT_ASSIGN instruction requires 3 tokens.\n");
        interpreter_halt();
        return;
    }

    const char* var_name = tokens[0];
    const char* value_token = tokens[2];

    Interpreter_Value* target_var_ptr = interpreter_get_variable(instance, var_name);
    if (target_var_ptr == NULL) {
         printf("Error: Variable not declared for SUBTRACT_ASSIGN operation.\n");
         interpreter_halt();
         return;
    }
    Interpreter_Value current_val = *target_var_ptr;
    Interpreter_Value val_to_subtract = interpreter_get_value_of_token(instance, value_token);

    if (val_to_subtract.type == -1) {
        printf("Error: Value for SUBTRACT_ASSIGN is invalid or not found.\n");
        interpreter_halt();
        return;
    }

    Interpreter_Value result;
    memset(&result, 0, sizeof(Interpreter_Value));

    if (current_val.type == TYPE_INT) {
        if (val_to_subtract.type == TYPE_INT) {
            result.type = TYPE_INT;
            result.i = current_val.i - val_to_subtract.i;
        }
        else if (val_to_subtract.type == TYPE_BYTE) {
            result.type = TYPE_INT;
            result.i = current_val.i - val_to_subtract.b;
        }
        else if (val_to_subtract.type == TYPE_FLOAT) {
            result.type = TYPE_FLOAT;
            result.f = (float)current_val.i - val_to_subtract.f;
        }
        else {
            printf("Error: Incompatible types for SUBTRACT_ASSIGN.\n");
            interpreter_halt();
            return;
        }
    }
    else if (current_val.type == TYPE_FLOAT) {
        if (val_to_subtract.type == TYPE_FLOAT) {
            result.type = TYPE_FLOAT;
            result.f = current_val.f - val_to_subtract.f;
        }
        else if (val_to_subtract.type == TYPE_INT) {
            result.type = TYPE_FLOAT;
            result.f = current_val.f - (float)val_to_subtract.i;
        }
        else if (val_to_subtract.type == TYPE_BYTE) {
            result.type = TYPE_FLOAT;
            result.f = current_val.f - (float)val_to_subtract.b;
        }
        else {
            printf("Error: Incompatible types for SUBTRACT_ASSIGN.\n");
            interpreter_halt();
            return;
        }
    }
    else if (current_val.type == TYPE_BYTE) {
        if (val_to_subtract.type == TYPE_BYTE) {
            result.type = TYPE_INT; 
            result.i = (long long)current_val.b - val_to_subtract.b;
        }
        else if (val_to_subtract.type == TYPE_INT) {
            result.type = TYPE_INT;
            result.i = (long long)current_val.b - val_to_subtract.i;
        }
        else if (val_to_subtract.type == TYPE_FLOAT) {
            result.type = TYPE_FLOAT;
            result.f = (float)current_val.b - val_to_subtract.f;
        }
        else {
            printf("Error: Incompatible types for SUBTRACT_ASSIGN.\n");
            interpreter_halt();
            return;
        }
    }
    else {
        printf("Error: Variable has an unsupported type for SUBTRACT_ASSIGN.\n");
        interpreter_halt();
        return;
    }
    interpreter_set_variable(instance, var_name, result);
}

void interpreter_execute_multiply(Interpreter_Instance* instance, char** tokens, int token_count)
{
    if (token_count < 5) {
        printf("Error: MULTIPLY instruction requires 5 tokens.\n");
        interpreter_halt();
        return;
    }

    const char* dest_var_name = tokens[0];
    const char* op1_token = tokens[2];
    const char* op2_token = tokens[4];

    Interpreter_Value val1 = interpreter_get_value_of_token(instance, op1_token);
    if (val1.type == -1) {
        printf("Error: Operand1 for MULTIPLY instruction is invalid or not found.\n");
        interpreter_halt();
        return;
    }

    Interpreter_Value val2 = interpreter_get_value_of_token(instance, op2_token);
    if (val2.type == -1) {
        printf("Error: Operand2 for MULTIPLY instruction is invalid or not found.\n");
        interpreter_halt();
        return;
    }

    Interpreter_Value result;
    memset(&result, 0, sizeof(Interpreter_Value));

    if ((val1.type == TYPE_INT || val1.type == TYPE_BYTE) && (val2.type == TYPE_INT || val2.type == TYPE_BYTE)) {
        result.type = TYPE_INT;
        long long v1 = (val1.type == TYPE_INT) ? val1.i : val1.b;
        long long v2 = (val2.type == TYPE_INT) ? val2.i : val2.b;
        result.i = v1 * v2;
    }
    else if ((val1.type == TYPE_FLOAT || val1.type == TYPE_INT || val1.type == TYPE_BYTE) &&
               (val2.type == TYPE_FLOAT || val2.type == TYPE_INT || val2.type == TYPE_BYTE)) {
        result.type = TYPE_FLOAT;
        float f1 = (val1.type == TYPE_FLOAT) ? val1.f : ((val1.type == TYPE_INT) ? (float)val1.i : (float)val1.b);
        float f2 = (val2.type == TYPE_FLOAT) ? val2.f : ((val2.type == TYPE_INT) ? (float)val2.i : (float)val2.b);
        result.f = f1 * f2;
    }
    else {
        printf("Error: Incompatible types for MULTIPLY operation. Only numeric types.\n");
        interpreter_halt();
        return;
    }
    interpreter_set_variable(instance, dest_var_name, result);
}

void interpreter_execute_multiply_assign(Interpreter_Instance* instance, char** tokens, int token_count)
{
    if (token_count < 3) {
        printf("Error: MULTIPLY_ASSIGN instruction requires 3 tokens.\n");
        interpreter_halt();
        return;
    }

    const char* var_name = tokens[0];
    const char* value_token = tokens[2];

    Interpreter_Value* target_var_ptr = interpreter_get_variable(instance, var_name);
    if (target_var_ptr == NULL) {
         printf("Error: Variable not declared for MULTIPLY_ASSIGN operation.\n");
         interpreter_halt();
         return;
    }
    Interpreter_Value current_val = *target_var_ptr;
    Interpreter_Value val_to_multiply = interpreter_get_value_of_token(instance, value_token);

    if (val_to_multiply.type == -1) {
        printf("Error: Value for MULTIPLY_ASSIGN is invalid or not found.\n");
        interpreter_halt();
        return;
    }

    Interpreter_Value result;
    memset(&result, 0, sizeof(Interpreter_Value));

    if (current_val.type == TYPE_INT) {
        if (val_to_multiply.type == TYPE_INT) {
            result.type = TYPE_INT;
            result.i = current_val.i * val_to_multiply.i;
        }
        else if (val_to_multiply.type == TYPE_BYTE) {
            result.type = TYPE_INT;
            result.i = current_val.i * val_to_multiply.b;
        }
        else if (val_to_multiply.type == TYPE_FLOAT) {
            result.type = TYPE_FLOAT;
            result.f = (float)current_val.i * val_to_multiply.f;
        }
        else {
            printf("Error: Incompatible types for MULTIPLY_ASSIGN.\n");
            interpreter_halt();
            return;
        }
    }
    else if (current_val.type == TYPE_FLOAT) {
        if (val_to_multiply.type == TYPE_FLOAT) {
            result.type = TYPE_FLOAT;
            result.f = current_val.f * val_to_multiply.f;
        }
        else if (val_to_multiply.type == TYPE_INT) {
            result.type = TYPE_FLOAT;
            result.f = current_val.f * (float)val_to_multiply.i;
        }
        else if (val_to_multiply.type == TYPE_BYTE) {
            result.type = TYPE_FLOAT;
            result.f = current_val.f * (float)val_to_multiply.b;
        }
        else {
            printf("Error: Incompatible types for MULTIPLY_ASSIGN.\n");
            interpreter_halt();
            return;
        }
    }
    else if (current_val.type == TYPE_BYTE) {
        if (val_to_multiply.type == TYPE_BYTE) {
            result.type = TYPE_INT; 
            result.i = (long long)current_val.b * val_to_multiply.b;
        }
        else if (val_to_multiply.type == TYPE_INT) {
            result.type = TYPE_INT;
            result.i = (long long)current_val.b * val_to_multiply.i;
        }
        else if (val_to_multiply.type == TYPE_FLOAT) {
            result.type = TYPE_FLOAT;
            result.f = (float)current_val.b * val_to_multiply.f;
        }
        else {
            printf("Error: Incompatible types for MULTIPLY_ASSIGN.\n");
            interpreter_halt();
            return;
        }
    }
    else {
        printf("Error: Variable has an unsupported type for MULTIPLY_ASSIGN.\n");
        interpreter_halt();
        return;
    }
    interpreter_set_variable(instance, var_name, result);
}

void interpreter_execute_divide(Interpreter_Instance* instance, char** tokens, int token_count)
{
    if (token_count < 5) {
        printf("Error: DIVIDE instruction requires 5 tokens.\n");
        interpreter_halt();
        return;
    }

    const char* dest_var_name = tokens[0];
    const char* op1_token = tokens[2];
    const char* op2_token = tokens[4];

    Interpreter_Value val1 = interpreter_get_value_of_token(instance, op1_token);
    if (val1.type == -1) {
        printf("Error: Operand1 for DIVIDE instruction is invalid or not found.\n");
        interpreter_halt();
        return;
    }

    Interpreter_Value val2 = interpreter_get_value_of_token(instance, op2_token);
    if (val2.type == -1) {
        printf("Error: Operand2 for DIVIDE instruction is invalid or not found.\n");
        interpreter_halt();
        return;
    }

    Interpreter_Value result;
    memset(&result, 0, sizeof(Interpreter_Value));

    if ((val1.type == TYPE_INT || val1.type == TYPE_BYTE) && (val2.type == TYPE_INT || val2.type == TYPE_BYTE)) {
        result.type = TYPE_INT; 
        long long v1 = (val1.type == TYPE_INT) ? val1.i : val1.b;
        long long v2 = (val2.type == TYPE_INT) ? val2.i : val2.b;
        if (v2 == 0) {
            printf("Error: Division by zero in DIVIDE operation.\n");
            interpreter_halt();
            return;
        }
        result.i = v1 / v2;
    }
    else if ((val1.type == TYPE_FLOAT || val1.type == TYPE_INT || val1.type == TYPE_BYTE) &&
               (val2.type == TYPE_FLOAT || val2.type == TYPE_INT || val2.type == TYPE_BYTE)) {
        result.type = TYPE_FLOAT;
        float f1 = (val1.type == TYPE_FLOAT) ? val1.f : ((val1.type == TYPE_INT) ? (float)val1.i : (float)val1.b);
        float f2 = (val2.type == TYPE_FLOAT) ? val2.f : ((val2.type == TYPE_INT) ? (float)val2.i : (float)val2.b);
        if (f2 == 0.0f) {
            printf("Error: Division by zero in DIVIDE operation.\n");
            interpreter_halt();
            return;
        }
        result.f = f1 / f2;
    }
    else {
        printf("Error: Incompatible types for DIVIDE operation. Only numeric types.\n");
        interpreter_halt();
        return;
    }
    interpreter_set_variable(instance, dest_var_name, result);
}

void interpreter_execute_divide_assign(Interpreter_Instance* instance, char** tokens, int token_count)
{
    if (token_count < 3) {
        printf("Error: DIVIDE_ASSIGN instruction requires 3 tokens.\n");
        interpreter_halt();
        return;
    }

    const char* var_name = tokens[0];
    const char* value_token = tokens[2];

    Interpreter_Value* target_var_ptr = interpreter_get_variable(instance, var_name);
    if (target_var_ptr == NULL) {
         printf("Error: Variable not declared for DIVIDE_ASSIGN operation.\n");
         interpreter_halt();
         return;
    }
    Interpreter_Value current_val = *target_var_ptr;
    Interpreter_Value val_to_divide = interpreter_get_value_of_token(instance, value_token);

    if (val_to_divide.type == -1) {
        printf("Error: Value for DIVIDE_ASSIGN is invalid or not found.\n");
        interpreter_halt();
        return;
    }

    Interpreter_Value result;
    memset(&result, 0, sizeof(Interpreter_Value));

    if (current_val.type == TYPE_INT) {
        long long divisor_i = 0;
        if (val_to_divide.type == TYPE_INT) {
            divisor_i = val_to_divide.i;
        }
        else if (val_to_divide.type == TYPE_BYTE) {
            divisor_i = val_to_divide.b;
        }
        else if (val_to_divide.type == TYPE_FLOAT) { 
            if (val_to_divide.f == 0.0f) {
                printf("Error: Division by zero in DIVIDE_ASSIGN.\n");
                interpreter_halt(); return;
            }
            result.type = TYPE_FLOAT;
            result.f = (float)current_val.i / val_to_divide.f;
            interpreter_set_variable(instance, var_name, result);
            return;
        }
        else {
            printf("Error: Incompatible types for DIVIDE_ASSIGN.\n");
            interpreter_halt(); return;
        }
        
        if (divisor_i == 0) {
            printf("Error: Division by zero in DIVIDE_ASSIGN.\n");
            interpreter_halt(); return;
        }
        result.type = TYPE_INT; 
        result.i = current_val.i / divisor_i;

    }
    else if (current_val.type == TYPE_FLOAT) {
        float divisor_f = 0.0f;
        if (val_to_divide.type == TYPE_FLOAT) {
            divisor_f = val_to_divide.f;
        }
        else if (val_to_divide.type == TYPE_INT) {
            divisor_f = (float)val_to_divide.i;
        }
        else if (val_to_divide.type == TYPE_BYTE) {
            divisor_f = (float)val_to_divide.b;
        }
        else {
            printf("Error: Incompatible types for DIVIDE_ASSIGN.\n");
            interpreter_halt(); return;
        }

        if (divisor_f == 0.0f) {
            printf("Error: Division by zero in DIVIDE_ASSIGN.\n");
            interpreter_halt(); return;
        }
        result.type = TYPE_FLOAT;
        result.f = current_val.f / divisor_f;

    }
    else if (current_val.type == TYPE_BYTE) {
        long long divisor_b_as_i = 0;
         if (val_to_divide.type == TYPE_BYTE) {
            divisor_b_as_i = val_to_divide.b;
         }
        else if (val_to_divide.type == TYPE_INT) {
            divisor_b_as_i = val_to_divide.i;
        }
        else if (val_to_divide.type == TYPE_FLOAT) { 
            if (val_to_divide.f == 0.0f) {
                printf("Error: Division by zero in DIVIDE_ASSIGN.\n");
                interpreter_halt(); return;
            }
            result.type = TYPE_FLOAT;
            result.f = (float)current_val.b / val_to_divide.f;
            interpreter_set_variable(instance, var_name, result);
            return;
        }
        else {
            printf("Error: Incompatible types for DIVIDE_ASSIGN.\n");
            interpreter_halt(); return;
        }

        if (divisor_b_as_i == 0) {
            printf("Error: Division by zero in DIVIDE_ASSIGN.\n");
            interpreter_halt(); return;
        }
        result.type = TYPE_INT; 
        result.i = (long long)current_val.b / divisor_b_as_i;
    }
    else {
        printf("Error: Variable has an unsupported type for DIVIDE_ASSIGN.\n");
        interpreter_halt();
        return;
    }
    interpreter_set_variable(instance, var_name, result);
}

void interpreter_execute_mod(Interpreter_Instance* instance, char** tokens, int token_count)
{
    if (token_count < 5) {
        printf("Error: MOD instruction requires 5 tokens.\n");
        interpreter_halt();
        return;
    }

    const char* dest_var_name = tokens[0];
    const char* op1_token = tokens[2];
    const char* op2_token = tokens[4];

    Interpreter_Value val1 = interpreter_get_value_of_token(instance, op1_token);
    if (val1.type == -1) {
        printf("Error: Operand1 for MOD instruction is invalid or not found.\n");
        interpreter_halt();
        return;
    }

    Interpreter_Value val2 = interpreter_get_value_of_token(instance, op2_token);
    if (val2.type == -1) {
        printf("Error: Operand2 for MOD instruction is invalid or not found.\n");
        interpreter_halt();
        return;
    }

    Interpreter_Value result;
    memset(&result, 0, sizeof(Interpreter_Value));

    if ((val1.type == TYPE_INT || val1.type == TYPE_BYTE) && (val2.type == TYPE_INT || val2.type == TYPE_BYTE)) {
        result.type = TYPE_INT;
        long long v1 = (val1.type == TYPE_INT) ? val1.i : val1.b;
        long long v2 = (val2.type == TYPE_INT) ? val2.i : val2.b;
        if (v2 == 0) {
            printf("Error: Modulo by zero in MOD operation.\n");
            interpreter_halt();
            return;
        }
        result.i = v1 % v2;
    }
    else {
        printf("Error: Incompatible types for MOD operation. Only integer types.\n");
        interpreter_halt();
        return;
    }
    interpreter_set_variable(instance, dest_var_name, result);
}

void interpreter_execute_mod_assign(Interpreter_Instance* instance, char** tokens, int token_count)
{
    if (token_count < 3) {
        printf("Error: MOD_ASSIGN instruction requires 3 tokens.\n");
        interpreter_halt();
        return;
    }

    const char* var_name = tokens[0];
    const char* value_token = tokens[2];

    Interpreter_Value* target_var_ptr = interpreter_get_variable(instance, var_name);
    if (target_var_ptr == NULL) {
         printf("Error: Variable not declared for MOD_ASSIGN operation.\n");
         interpreter_halt();
         return;
    }
    Interpreter_Value current_val = *target_var_ptr;
    Interpreter_Value val_to_mod = interpreter_get_value_of_token(instance, value_token);

    if (val_to_mod.type == -1) {
        printf("Error: Value for MOD_ASSIGN is invalid or not found.\n");
        interpreter_halt();
        return;
    }

    Interpreter_Value result;
    memset(&result, 0, sizeof(Interpreter_Value));

    if ((current_val.type == TYPE_INT || current_val.type == TYPE_BYTE) &&
        (val_to_mod.type == TYPE_INT || val_to_mod.type == TYPE_BYTE)) {
        
        long long current_v = (current_val.type == TYPE_INT) ? current_val.i : current_val.b;
        long long val_v = (val_to_mod.type == TYPE_INT) ? val_to_mod.i : val_to_mod.b;

        if (val_v == 0) {
            printf("Error: Modulo by zero in MOD_ASSIGN.\n");
            interpreter_halt();
            return;
        }
        result.type = TYPE_INT; 
        result.i = current_v % val_v;
    }
    else {
        printf("Error: Incompatible types for MOD_ASSIGN. Both variable and value must be integer types.\n");
        interpreter_halt();
        return;
    }
    interpreter_set_variable(instance, var_name, result);
}

void interpreter_execute_end(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_end\n");}
void interpreter_execute_exec(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_exec\n");}
void interpreter_execute_free(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_free\n");}
void interpreter_execute_if(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_if\n");}
void interpreter_execute_input(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_input\n");}
void interpreter_execute_load(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_load\n");}
void interpreter_execute_print(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_print\n");}
void interpreter_execute_println(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_println\n");}
void interpreter_execute_random(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_random\n");}
void interpreter_execute_return(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_return\n");}
void interpreter_execute_save(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_save\n");}
void interpreter_execute_sizeof(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_sizeof\n");}
void interpreter_execute_sleep(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_sleep\n");}
void interpreter_execute_while(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_while\n");}

Interpreter_Instruction_AttributeMatcher declare_matchers[] = { {"@", 0} };
Interpreter_Instruction_AttributeMatcher assign_matchers[] = { {"=", 1} };
Interpreter_Instruction_AttributeMatcher add_matchers[] = { {"=", 1}, {"+", 3} };
Interpreter_Instruction_AttributeMatcher add_assign_matchers[] = { {"+=", 1} };
Interpreter_Instruction_AttributeMatcher array_get_matchers[] = { {"<-", 1}, {":", 3} };
Interpreter_Instruction_AttributeMatcher array_set_matchers[] = { {":", 1}, {"<-", 3} };
Interpreter_Instruction_AttributeMatcher call_matchers[] = { {"call", 0} };
Interpreter_Instruction_AttributeMatcher call_assign_matchers[] = { {"<-", 1}, {"call", 2} };
Interpreter_Instruction_AttributeMatcher cat_matchers[] = { {"cat", 0} };
Interpreter_Instruction_AttributeMatcher clear_matchers[] = { {"clear", 0} };
Interpreter_Instruction_AttributeMatcher divide_matchers[] = { {"=", 1}, {"/", 3} };
Interpreter_Instruction_AttributeMatcher divide_assign_matchers[] = { {"/=", 1} };
Interpreter_Instruction_AttributeMatcher end_matchers[] = { {"end", 0} };
Interpreter_Instruction_AttributeMatcher exec_matchers[] = { {"exec", 0} };
Interpreter_Instruction_AttributeMatcher free_matchers[] = { {"free", 0} };
Interpreter_Instruction_AttributeMatcher if_matchers[] = { {"if", 0} };
Interpreter_Instruction_AttributeMatcher input_matchers[] = { {"input", 0} };
Interpreter_Instruction_AttributeMatcher load_matchers[] = { {"load", 0} };
Interpreter_Instruction_AttributeMatcher mod_matchers[] = { {"=", 1}, {"%", 3} };
Interpreter_Instruction_AttributeMatcher mod_assign_matchers[] = { {"%=", 1} };
Interpreter_Instruction_AttributeMatcher multiply_matchers[] = { {"=", 1}, {"*", 3} };
Interpreter_Instruction_AttributeMatcher multiply_assign_matchers[] = { {"*=", 1} };
Interpreter_Instruction_AttributeMatcher print_matchers[] = { {"print", 0} };
Interpreter_Instruction_AttributeMatcher println_matchers[] = { {"println", 0} };
Interpreter_Instruction_AttributeMatcher random_matchers[] = { {"random", 0} };
Interpreter_Instruction_AttributeMatcher return_matchers[] = { {"return", 0} };
Interpreter_Instruction_AttributeMatcher save_matchers[] = { {"save", 0} };
Interpreter_Instruction_AttributeMatcher sizeof_matchers[] = { {"sizeof", 1} };
Interpreter_Instruction_AttributeMatcher sleep_matchers[] = { {"sleep", 0} };
Interpreter_Instruction_AttributeMatcher subtract_matchers[] = { {"=", 1}, {"-", 3} };
Interpreter_Instruction_AttributeMatcher subtract_assign_matchers[] = { {"-=", 1} };
Interpreter_Instruction_AttributeMatcher while_matchers[] = { {"while", 0} };

const Interpreter_Instruction_Definition g_instruction_definitions[] = {
    {"ADD", add_matchers, sizeof(add_matchers)/sizeof(Interpreter_Instruction_AttributeMatcher), 5, interpreter_execute_add},
    {"ARRAY_GET", array_get_matchers, sizeof(array_get_matchers)/sizeof(Interpreter_Instruction_AttributeMatcher), 5, interpreter_execute_array_get},
    {"ARRAY_SET", array_set_matchers, sizeof(array_set_matchers)/sizeof(Interpreter_Instruction_AttributeMatcher), 5, interpreter_execute_array_set},
    {"CALL_ASSIGN", call_assign_matchers, sizeof(call_assign_matchers)/sizeof(Interpreter_Instruction_AttributeMatcher), 4, interpreter_execute_call_assign},
    {"DIVIDE", divide_matchers, sizeof(divide_matchers)/sizeof(Interpreter_Instruction_AttributeMatcher), 5, interpreter_execute_divide},
    {"MOD", mod_matchers, sizeof(mod_matchers)/sizeof(Interpreter_Instruction_AttributeMatcher), 5, interpreter_execute_mod},
    {"MULTIPLY", multiply_matchers, sizeof(multiply_matchers)/sizeof(Interpreter_Instruction_AttributeMatcher), 5, interpreter_execute_multiply},
    {"SUBTRACT", subtract_matchers, sizeof(subtract_matchers)/sizeof(Interpreter_Instruction_AttributeMatcher), 5, interpreter_execute_subtract},
    {"DECLARE", declare_matchers, sizeof(declare_matchers)/sizeof(Interpreter_Instruction_AttributeMatcher), 3, interpreter_execute_declare},
    {"ASSIGN", assign_matchers, sizeof(assign_matchers)/sizeof(Interpreter_Instruction_AttributeMatcher), 3, interpreter_execute_assign},
    {"ADD_ASSIGN", add_assign_matchers, sizeof(add_assign_matchers)/sizeof(Interpreter_Instruction_AttributeMatcher), 3, interpreter_execute_add_assign},
    {"CALL", call_matchers, sizeof(call_matchers)/sizeof(Interpreter_Instruction_AttributeMatcher), 2, interpreter_execute_call},
    {"CAT", cat_matchers, sizeof(cat_matchers)/sizeof(Interpreter_Instruction_AttributeMatcher), 4, interpreter_execute_cat},
    {"CLEAR", clear_matchers, sizeof(clear_matchers)/sizeof(Interpreter_Instruction_AttributeMatcher), 1, interpreter_execute_clear},
    {"DIVIDE_ASSIGN", divide_assign_matchers, sizeof(divide_assign_matchers)/sizeof(Interpreter_Instruction_AttributeMatcher), 3, interpreter_execute_divide_assign},
    {"END", end_matchers, sizeof(end_matchers)/sizeof(Interpreter_Instruction_AttributeMatcher), 1, interpreter_execute_end},
    {"EXEC", exec_matchers, sizeof(exec_matchers)/sizeof(Interpreter_Instruction_AttributeMatcher), 3, interpreter_execute_exec},
    {"FREE", free_matchers, sizeof(free_matchers)/sizeof(Interpreter_Instruction_AttributeMatcher), 2, interpreter_execute_free},
    {"IF", if_matchers, sizeof(if_matchers)/sizeof(Interpreter_Instruction_AttributeMatcher), 4, interpreter_execute_if},
    {"INPUT", input_matchers, sizeof(input_matchers)/sizeof(Interpreter_Instruction_AttributeMatcher), 3, interpreter_execute_input},
    {"LOAD", load_matchers, sizeof(load_matchers)/sizeof(Interpreter_Instruction_AttributeMatcher), 4, interpreter_execute_load},
    {"MOD_ASSIGN", mod_assign_matchers, sizeof(mod_assign_matchers)/sizeof(Interpreter_Instruction_AttributeMatcher), 3, interpreter_execute_mod_assign},
    {"MULTIPLY_ASSIGN", multiply_assign_matchers, sizeof(multiply_assign_matchers)/sizeof(Interpreter_Instruction_AttributeMatcher), 3, interpreter_execute_multiply_assign},
    {"PRINT", print_matchers, sizeof(print_matchers)/sizeof(Interpreter_Instruction_AttributeMatcher), 3, interpreter_execute_print},
    {"PRINTLN", println_matchers, sizeof(println_matchers)/sizeof(Interpreter_Instruction_AttributeMatcher), 3, interpreter_execute_println},
    {"RANDOM", random_matchers, sizeof(random_matchers)/sizeof(Interpreter_Instruction_AttributeMatcher), 3, interpreter_execute_random},
    {"RETURN", return_matchers, sizeof(return_matchers)/sizeof(Interpreter_Instruction_AttributeMatcher), 1, interpreter_execute_return},
    {"SAVE", save_matchers, sizeof(save_matchers)/sizeof(Interpreter_Instruction_AttributeMatcher), 4, interpreter_execute_save},
    {"SIZEOF", sizeof_matchers, sizeof(sizeof_matchers)/sizeof(Interpreter_Instruction_AttributeMatcher), 3, interpreter_execute_sizeof},
    {"SLEEP", sleep_matchers, sizeof(sleep_matchers)/sizeof(Interpreter_Instruction_AttributeMatcher), 2, interpreter_execute_sleep},
    {"SUBTRACT_ASSIGN", subtract_assign_matchers, sizeof(subtract_assign_matchers)/sizeof(Interpreter_Instruction_AttributeMatcher), 3, interpreter_execute_subtract_assign},
    {"WHILE", while_matchers, sizeof(while_matchers)/sizeof(Interpreter_Instruction_AttributeMatcher), 4, interpreter_execute_while}
};
const int g_instruction_definition_count = sizeof(g_instruction_definitions) / sizeof(Interpreter_Instruction_Definition);
