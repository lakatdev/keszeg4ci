#include <instance.h>
#include <interpreter.h>
#include <instructions.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

char interpreter_code_path[INTERPRETER_PATH_MAX];

#define INT_MIN -2147483648
#define INT_MAX 2147483647

void interpreter_halt()
{

}

void interpreter_instance_init(Interpreter_Instance* instance)
{
    instance->depth = 0;
    instance->func_count = 0;
    instance->parsed_line_count = 0;
    instance->stack_pointer = -1;
    instance->execution_position = 0;
}

char interpreter_lowercase(char ch)
{
    if (ch >= 'A' && ch <= 'Z') {
        return ch + ('a' - 'A');
    }
    return ch;
}

int interpreter_ci_strcmp(const char* str1, const char* str2)
{
    while (*str1 && *str2) {
        if (interpreter_lowercase(*str1) != interpreter_lowercase(*str2)) {
            return interpreter_lowercase(*str1) - interpreter_lowercase(*str2);
        }
        str1++;
        str2++;
    }
    return interpreter_lowercase(*str1) - interpreter_lowercase(*str2);
}

Interpreter_VarType interpreter_get_type(const char* type_str)
{
    if (interpreter_ci_strcmp(type_str, "int") == 0) {
        return TYPE_INT;
    }
    else if (interpreter_ci_strcmp(type_str, "float") == 0) {
        return TYPE_FLOAT;
    }
    else if (interpreter_ci_strcmp(type_str, "byte") == 0) {
        return TYPE_BYTE;
    }
    else if (interpreter_ci_strcmp(type_str, "iarray") == 0) {
        return TYPE_IARRAY;
    }
    else if (interpreter_ci_strcmp(type_str, "farray") == 0) {
        return TYPE_FARRAY;
    }
    else if (interpreter_ci_strcmp(type_str, "string") == 0) {
        return TYPE_STRING;
    }
    return -1;
}

int interpreter_load_code(Interpreter_Instance* instance, const char* code)
{
    static char processed_code_buffer[INTERPRETER_MAX_CODE + 1];
    int processed_idx = 0;
    const char *p = code;

    while (*p) {
        if (processed_idx >= INTERPRETER_MAX_CODE) {
            printf("Error: Code buffer full\n");
            interpreter_halt();
            return 1;
        }

        if (*p == '#') {
            while (*p && *p != '\n') {
                p++;
            }
            if (*p == '\n') {
                processed_code_buffer[processed_idx++] = '\n';
                p++;
            }
        }
        else if (*p == ',') {
            processed_code_buffer[processed_idx++] = '\n';
            p++;
        }
        else {
            processed_code_buffer[processed_idx++] = *p;
            p++;
        }
    }
    processed_code_buffer[processed_idx] = '\0';

    instance->parsed_line_count = 0;
    char *line_saveptr;
    char *current_line_segment = strtok_r(processed_code_buffer, "\n", &line_saveptr);

    while (current_line_segment != (void*)0 && instance->parsed_line_count < INTERPRETER_MAX_PARSED_LINES) {
        char *trimmed_line = current_line_segment;

        while (isspace((unsigned char)*trimmed_line)) {
            trimmed_line++;
        }

        if (*trimmed_line != '\0') {
            char *end_ptr = trimmed_line + strlen(trimmed_line) - 1;
            while (end_ptr > trimmed_line && isspace((unsigned char)*end_ptr)) {
                end_ptr--;
            }
            *(end_ptr + 1) = '\0';
        }

        if (strlen(trimmed_line) == 0) {
            current_line_segment = strtok_r((void*)0, "\n", &line_saveptr);
            continue;
        }

        int current_token_count = 0;
        char *token_saveptr;
        char *token = strtok_r(trimmed_line, " \t", &token_saveptr);

        while (token != (void*)0 && current_token_count < INTERPRETER_MAX_TOKENS_PER_LINE) {
            if (strlen(token) >= INTERPRETER_MAX_TOKEN_LENGTH) {
                printf("Error: Token too long on line %d.\n", instance->parsed_line_count + 1);
                interpreter_halt();
                return 1;
            }
            strncpy(instance->parsed_code[instance->parsed_line_count][current_token_count], token, INTERPRETER_MAX_TOKEN_LENGTH - 1);
            instance->parsed_code[instance->parsed_line_count][current_token_count][INTERPRETER_MAX_TOKEN_LENGTH - 1] = '\0'; // Ensure (void*)0-termination
            current_token_count++;
            token = strtok_r((void*)0, " \t", &token_saveptr);
        }
         if (token != (void*)0 && current_token_count >= INTERPRETER_MAX_TOKENS_PER_LINE) {
            printf("Error: Too many tokens on line %d.\n", instance->parsed_line_count + 1);
            interpreter_halt();
            return 1;
        }

        instance->line_token_counts[instance->parsed_line_count] = current_token_count;
        instance->parsed_line_count++;

        current_line_segment = strtok_r((void*)0, "\n", &line_saveptr);
    }

    if (current_line_segment != (void*)0 && instance->parsed_line_count >= INTERPRETER_MAX_PARSED_LINES) {
        printf("Error: Too many lines\n");
        interpreter_halt();
        return 1;
    }

    return 0;
}

void interpreter_dump_tokens(Interpreter_Instance* instance)
{
    printf("\n--- Tokenized Code ---\n");
    for (int i = 0; i < instance->parsed_line_count; i++) {
        printf("Line %d: ", i + 1);
        for (int j = 0; j < instance->line_token_counts[i]; j++) {
            printf("[%s] ", instance->parsed_code[i][j]);
        }
        printf("\n");
    }
    printf("--- End of Tokenized Code ---\n\n");
}

void interpreter_dump_functions(Interpreter_Instance* instance)
{
    printf("\n--- Functions ---\n");
    for (int i = 0; i < instance->func_count; i++) {
        printf("Function %s starts at line %d with %d parameters:\n", instance->functions[i].name, instance->functions[i].start_line, instance->functions[i].param_count);
        for (int j = 0; j < instance->functions[i].param_count; j++) {
            printf("  Param %d: %s (%s", j + 1, instance->functions[i].params[j].name, instance->functions[i].params[j].is_ref ? "ref" : "var");
            switch (instance->functions[i].params[j].type) {
                case TYPE_INT: {
                    printf(" int");
                    break;
                }   
                case TYPE_FLOAT: {
                    printf(" float");
                    break;
                }
                case TYPE_BYTE: {
                    printf(" byte");
                    break;
                }
                case TYPE_IARRAY: {
                    printf(" iarray");
                    break;
                }
                case TYPE_FARRAY: {
                    printf(" farray");
                    break;
                }
                case TYPE_STRING: {
                    printf(" string");
                    break;
                }
                default: {
                    printf(" unknown");
                    break;
                }
            }
            printf(")\n");
        }
    }
    printf("--- End of Functions ---\n\n");
}

int interpreter_parse_functions(Interpreter_Instance* instance)
{
    instance->func_count = 0;
    for (int i = 0; i < instance->parsed_line_count; i++) {
        if (interpreter_ci_strcmp(instance->parsed_code[i][0], "fun") == 0) {
            if (instance->func_count >= INTERPRETER_MAX_FUNCTIONS) {
                printf("Error: Too many functions\n");
                interpreter_halt();
                return 1;
            }
            strncpy(instance->functions[instance->func_count].name, instance->parsed_code[i][1], INTERPRETER_MAX_TOKEN_LENGTH - 1);
            instance->functions[instance->func_count].name[INTERPRETER_MAX_TOKEN_LENGTH - 1] = '\0';
            instance->functions[instance->func_count].start_line = i + 1;

            int param_count = 0;
            for (int j = 2; j < instance->line_token_counts[i]; j += 3) {
                int ref = interpreter_ci_strcmp(instance->parsed_code[i][j], "&") == 0;
                int var = interpreter_ci_strcmp(instance->parsed_code[i][j], "@") == 0;
                if (ref || var) {
                    Interpreter_Parameter param;
                    strcpy(param.name, instance->parsed_code[i][j + 1]);
                    param.name[strlen(instance->parsed_code[i][j + 1])] = '\0';
                    param.is_ref = ref;
                    param.type = interpreter_get_type(instance->parsed_code[i][j + 2]);
                    if (param.type == -1) {
                        printf("Error: Invalid type for parameter %s\n", instance->parsed_code[i][j + 2]);
                        interpreter_halt();
                        return 1;
                    }
                    instance->functions[instance->func_count].params[param_count] = param;
                    param_count++;
                }
            }
            instance->functions[instance->func_count].param_count = param_count;
            instance->func_count++;
        }
    }
    return 0;
}

int is_purely_alphabetic(const char* str)
{
    if (!str || *str == '\0') {
        return 0;
    }
    while (*str) {
        if (!isalpha((unsigned char)*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}

void interpreter_parse_const_escapes(const char* src, char* dest, unsigned long long int max_len)
{
    size_t si = 0, di = 0;
    while (src[si] && di + 1 < max_len) {
        if (src[si] == '\\') {
            char next = src[si + 1];
            switch (next) {
                case 'n': case 'N': dest[di++] = '\n'; si += 2; break;
                case 't': case 'T': dest[di++] = '\t'; si += 2; break;
                case 's': case 'S': dest[di++] = ' ';  si += 2; break;
                case 'h': case 'H': dest[di++] = '#';  si += 2; break;
                case 'c': case 'C': dest[di++] = ',';  si += 2; break;
                case '\\':          dest[di++] = '\\'; si += 2; break;
                case '\0':          si++; break;
                default:            dest[di++] = src[si++]; break;
            }
        } else {
            dest[di++] = src[si++];
        }
    }
    dest[di] = '\0';
}

Interpreter_Value interpreter_get_value_of_token(Interpreter_Instance* instance, const char* token)
{
    Interpreter_Value val;
    val.type = TYPE_INT;
    val.i = 0;

    if (!token || *token == '\0') {
        return val;
    }

    if (is_purely_alphabetic(token)) {
        Interpreter_Value* var_val_ptr = interpreter_get_variable(instance, token);
        if (var_val_ptr != NULL) {
            return *var_val_ptr;
        }
        return val;
    }

    unsigned long long int len = strlen(token);
    char* endptr_byte_num;
    char* endptr_int;

    if (len > 1 && (token[len - 1] == 'b' || token[len - 1] == 'B')) {
        if (len - 1 < INTERPRETER_MAX_TOKEN_LENGTH) {
            char numeric_part[INTERPRETER_MAX_TOKEN_LENGTH];
            strncpy(numeric_part, token, len - 1);
            numeric_part[len - 1] = '\0';

            if (numeric_part[0] != '\0') {
                long temp_long = strtol(numeric_part, &endptr_byte_num, 10);

                if (numeric_part != endptr_byte_num && *endptr_byte_num == '\0' && temp_long >= 0 && temp_long <= 255) {
                    val.type = TYPE_BYTE;
                    val.b = (unsigned char)temp_long;
                    return val;
                }
            }
        }
    }

    int is_float = 0;
    for (unsigned long long int i = 0; i < len; ++i) {
        if (token[i] == '.' || token[i] == 'e' || token[i] == 'E') {
            is_float = 1;
            break;
        }
    }
    if (!is_float && len > 1 && (token[len - 1] == 'f' || token[len - 1] == 'F')) {
        is_float = 1;
    }

    if (is_float) {
        char* endptr_float;
        float temp_float = strtof(token, &endptr_float);
        if (token != endptr_float) {
            if (*endptr_float == '\0' ||
                ((*endptr_float == 'f' || *endptr_float == 'F') && endptr_float == &token[len - 1])) {
                val.type = TYPE_FLOAT;
                val.f = temp_float;
                return val;
            }
        }
    }
    else {
        long temp_int_long = strtol(token, &endptr_int, 10);
        if (token != endptr_int && *endptr_int == '\0') {
            if (temp_int_long >= INT_MIN && temp_int_long <= INT_MAX) {
                val.type = TYPE_INT;
                val.i = (int)temp_int_long;
                return val;
            }
        }
    }

    return val;
}

void interpreter_declare_variable(Interpreter_Instance* instance, const char* name, Interpreter_VarType type)
{
    if (instance->stack_pointer < 0) {
        printf("Error: No active call frame for variable declaration\n");
        interpreter_halt();
        return;
    }

    Interpreter_CallFrame* current_frame = &instance->call_stack[instance->stack_pointer];
    
    for (int i = 0; i < current_frame->local_var_count; i++) {
        if (interpreter_ci_strcmp(current_frame->local_vars[i].name, name) == 0) {
            if ( instance->stack_pointer <= current_frame->local_vars[i].declaration_line) {
                return;
            }
            else {
                printf("Error: Variable %s already declared in current scope\n", name);
                interpreter_halt();
                return;
            }
        }
    }

    for (int i = 0; i < current_frame->ref_count; i++) {
        if (interpreter_ci_strcmp(current_frame->refs[i].name, name) == 0) {
            printf("Error: A reference with the name %s already exists in the current scope\n", name);
            interpreter_halt();
            return;
        }
    }

    if (current_frame->local_var_count >= INTERPRETER_MAX_VARIABLES) {
        printf("Error: Too many variables in current scope\n");
        interpreter_halt();
        return;
    }

    Interpreter_Variable* new_var = &current_frame->local_vars[current_frame->local_var_count];
    strncpy(new_var->name, name, INTERPRETER_MAX_NAME - 1);
    new_var->name[INTERPRETER_MAX_NAME - 1] = '\0';
    new_var->type = type;
    new_var->declaration_line = instance->execution_position;
    new_var->value.type = type;
    
    switch (type) {
        case TYPE_INT:
            new_var->value.i = 0;
            break;
        case TYPE_FLOAT:
            new_var->value.f = 0.0f;
            break;
        case TYPE_BYTE:
            new_var->value.b = 0;
            break;
        case TYPE_STRING:
            new_var->value.string.data[0] = '\0';
            new_var->value.string.size = 0;
            break;
        case TYPE_IARRAY:
            for (int i = 0; i < INTERPRETER_MAX_ARRAY_SIZE; i++) {
                new_var->value.iarray.data[i] = 0;
                new_var->value.iarray.size = 0;
            }
            break;
        case TYPE_FARRAY:
            for (int i = 0; i < INTERPRETER_MAX_ARRAY_SIZE; i++) {
                new_var->value.farray.data[i] = 0.0f;
                new_var->value.farray.size = 0;
            }
            break;
    }

    current_frame->local_var_count++;
}

Interpreter_Value* interpreter_get_variable(Interpreter_Instance* instance, const char* name)
{
    if (instance->stack_pointer < 0) {
        printf("Error: No active call frame for variable access\n");
        interpreter_halt();
        return (void*)0;
    }

    Interpreter_CallFrame* current_frame = &instance->call_stack[instance->stack_pointer];
    for (int i = 0; i < current_frame->ref_count; i++) {
        if (interpreter_ci_strcmp(current_frame->refs[i].name, name) == 0) {
            return current_frame->refs[i].value_ptr;
        }
    }

    for (int i = 0; i < current_frame->local_var_count; i++) {
        if (interpreter_ci_strcmp(current_frame->local_vars[i].name, name) == 0) {
            if (instance->execution_position < current_frame->local_vars[i].declaration_line) {
                printf("Error: Variable %s accessed before declaration (declared on line %d, accessed on line %d)\n", name, current_frame->local_vars[i].declaration_line + 1, instance->execution_position + 1);
                interpreter_halt();
                return (void*)0;
            }
            return &current_frame->local_vars[i].value;
        }
    }

    printf("Error: Variable %s not found in current scope\n", name);
    interpreter_halt();
    return (void*)0;
}

void interpreter_set_variable(Interpreter_Instance* instance, const char* name, Interpreter_Value value)
{
    if (instance->stack_pointer < 0) {
        printf("Error: No active call frame for variable assignment\n");
        interpreter_halt();
        return;
    }

    Interpreter_CallFrame* current_frame = &instance->call_stack[instance->stack_pointer];
    
    for (int i = 0; i < current_frame->ref_count; i++) {
        if (interpreter_ci_strcmp(current_frame->refs[i].name, name) == 0) {
            *(current_frame->refs[i].value_ptr) = value;
            return;
        }
    }
    
    for (int i = 0; i < current_frame->local_var_count; i++) {
        if (interpreter_ci_strcmp(current_frame->local_vars[i].name, name) == 0) {
            if (instance->execution_position < current_frame->local_vars[i].declaration_line) {
                printf("Error: Variable %s assigned before declaration (declared on line %d, accessed on line %d)\n", name, current_frame->local_vars[i].declaration_line + 1, instance->execution_position + 1);
                interpreter_halt();
                return;
            }
            current_frame->local_vars[i].value = value;
            return;
        }
    }

    printf("Error: Variable %s not found in current scope\n", name);
    interpreter_halt();
}

void interpreter_set_reference_parameter(Interpreter_Instance* instance, const char* param_name, const char* var_name)
{
    if (instance->stack_pointer < 0) {
        printf("Error: No active call frame for reference setup\n");
        interpreter_halt();
        return;
    }

    if (instance->stack_pointer == 0) {
        printf("Error: Cannot set reference parameter in main function\n");
        interpreter_halt();
        return;
    }

    Interpreter_CallFrame* caller_frame = &instance->call_stack[instance->stack_pointer - 1];
    Interpreter_Value* var_ptr = (void*)0;

    for (int i = 0; i < caller_frame->local_var_count; i++) {
        if (interpreter_ci_strcmp(caller_frame->local_vars[i].name, var_name) == 0) {
            if (instance->execution_position < caller_frame->local_vars[i].declaration_line) {
                printf("Error: Variable %s (passed by reference) used before declaration (declared on line %d, referenced at call on line %d)\n", var_name, caller_frame->local_vars[i].declaration_line + 1, instance->execution_position + 1);
                interpreter_halt();
                return;
            }
            var_ptr = &caller_frame->local_vars[i].value;
            break;
        }
    }

    if (var_ptr == (void*)0) {
        for (int i = 0; i < caller_frame->ref_count; i++) {
            if (interpreter_ci_strcmp(caller_frame->refs[i].name, var_name) == 0) {
                var_ptr = caller_frame->refs[i].value_ptr;
                break;
            }
        }
    }

    if (var_ptr == (void*)0) {
        printf("Error: Variable %s not found in caller scope\n", var_name);
        interpreter_halt();
        return;
    }

    Interpreter_CallFrame* current_frame = &instance->call_stack[instance->stack_pointer];
    if (current_frame->ref_count >= INTERPRETER_MAX_PARAMETERS) {
        printf("Error: Too many reference parameters\n");
        interpreter_halt();
        return;
    }

    strncpy(current_frame->refs[current_frame->ref_count].name, param_name, INTERPRETER_MAX_NAME - 1);
    current_frame->refs[current_frame->ref_count].name[INTERPRETER_MAX_NAME - 1] = '\0';
    current_frame->refs[current_frame->ref_count].value_ptr = var_ptr;
    current_frame->ref_count++;
}

int interpreter_jump_to_function(Interpreter_Instance* instance, const char* function_name)
{
    for (int i = 0; i < instance->func_count; i++) {
        if (interpreter_ci_strcmp(instance->functions[i].name, function_name) == 0) {

            if (instance->stack_pointer >= INTERPRETER_MAX_STACK - 1) {
                printf("Error: Call stack overflow\n");
                interpreter_halt();
                return -1;
            }
            instance->stack_pointer++;
            instance->call_stack[instance->stack_pointer].return_address = instance->execution_position + 1;
            instance->call_stack[instance->stack_pointer].local_var_count = 0;
            instance->call_stack[instance->stack_pointer].ref_count = 0;

            for (int j = 0; j < instance->functions[i].param_count; j++) {
                if (!instance->functions[i].params[j].is_ref) {
                    interpreter_declare_variable(instance, 
                        instance->functions[i].params[j].name, 
                        instance->functions[i].params[j].type);
                }
            }

            return instance->functions[i].start_line;
        }
    }
    printf("Error: Function %s not found\n", function_name);
    interpreter_halt();
    return -1;
}

int interpreter_call_function(Interpreter_Instance* instance, const char* function_name, char** arg_names, int arg_count)
{
    Interpreter_Function* target_function = (void*)0;
    for (int i = 0; i < instance->func_count; i++) {
        if (interpreter_ci_strcmp(instance->functions[i].name, function_name) == 0) {
            target_function = &instance->functions[i];
            break;
        }
    }

    if (target_function == (void*)0) {
        printf("Error: Function %s not found\n", function_name);
        interpreter_halt();
        return -1;
    }

    if (arg_count != target_function->param_count) {
        printf("Error: Function %s expects %d parameters, got %d\n", function_name, target_function->param_count, arg_count);
        interpreter_halt();
        return -1;
    }

    int function_start = interpreter_jump_to_function(instance, function_name);
    if (function_start == -1) {
        return -1;
    }

    for (int i = 0; i < target_function->param_count; i++) {
        if (target_function->params[i].is_ref) {
            interpreter_set_reference_parameter(instance, target_function->params[i].name, arg_names[i]);
        }
        else {
            instance->stack_pointer--;
            Interpreter_Value* arg_value = interpreter_get_variable(instance, arg_names[i]);
            if (arg_value == (void*)0) {
                printf("Error: Argument variable %s not found\n", arg_names[i]);
                interpreter_halt();
                instance->stack_pointer++;
                return -1;
            }
            Interpreter_Value value_copy = *arg_value;
            instance->stack_pointer++;
            
            interpreter_set_variable(instance, target_function->params[i].name, value_copy);
        }
    }

    instance->execution_position = function_start;
    return function_start;
}

int interpreter_instruction_matches(const Interpreter_Instruction_Definition* def, char** tokens, int token_count)
{
    if (token_count < def->min_token_count) {
        return 0;
    }

    for (int i = 0; i < def->matcher_count; i++) {
        if (def->matchers[i].token_index >= token_count || interpreter_ci_strcmp(tokens[def->matchers[i].token_index], def->matchers[i].token) != 0) {
            return 0;
        }
    }
    return 1;
}

int interpreter_execute(Interpreter_Instance* instance)
{
    instance->execution_position = interpreter_call_function(instance, "main", (char**)0, 0);
    
    if (instance->execution_position == -1) {
        printf("Error: No main function found\n");
        interpreter_halt();
        return 1;
    }

    while (instance->execution_position < instance->parsed_line_count && instance->execution_position >= 0) {
        int token_count = instance->line_token_counts[instance->execution_position];
        
        char* current_line_tokens[INTERPRETER_MAX_TOKENS_PER_LINE]; 
        if (token_count > 0) {
            for (int i = 0; i < token_count; i++) {
                current_line_tokens[i] = instance->parsed_code[instance->execution_position][i];
            }
        }

        if (token_count == 0) {
            instance->execution_position++;
            continue;
        }

        if (interpreter_ci_strcmp(current_line_tokens[0], "ef") == 0) {
            if (instance->stack_pointer <= 0) {
                instance->stack_pointer = -1;
                instance->execution_position = -1;
            }
            else {
                instance->execution_position = instance->call_stack[instance->stack_pointer].return_address;
                instance->stack_pointer--;
            }
            continue; 
        }

        if (interpreter_ci_strcmp(current_line_tokens[0], "fun") == 0) {
            instance->execution_position++;
            continue;
        }

        int instruction_was_handled_and_jumped = 0;
        for (int i = 0; i < g_instruction_definition_count; i++) { 
            if (interpreter_instruction_matches(&g_instruction_definitions[i], current_line_tokens, token_count)) {
                int prev_exec_pos = instance->execution_position;
                g_instruction_definitions[i].execute(instance, current_line_tokens, token_count);
                
                if (instance->execution_position != prev_exec_pos) {
                    instruction_was_handled_and_jumped = 1;
                }
                break; 
            }
        }
        
        if (!instruction_was_handled_and_jumped) {
            instance->execution_position++;
        }
    }
    return 0;
}

int interpreter_find_matching_end(Interpreter_Instance* instance, int start_line)
{
    int depth = 0;
    for (int i = start_line + 1; i < instance->parsed_line_count; ++i) {
        char* first_token = instance->parsed_code[i][0];
        if (interpreter_ci_strcmp(first_token, "if") == 0 || interpreter_ci_strcmp(first_token, "while") == 0) {
            depth++;
        }
        else if (interpreter_ci_strcmp(first_token, "end") == 0) {
            if (depth == 0) return i;
            depth--;
        }
    }
    return -1;
}

int interpreter_compare(Interpreter_Value left, Interpreter_Value right, const char* op)
{
    double l = (left.type == TYPE_FLOAT) ? left.f : (left.type == TYPE_INT) ? left.i : (left.type == TYPE_BYTE) ? left.b : 0;
    double r = (right.type == TYPE_FLOAT) ? right.f : (right.type == TYPE_INT) ? right.i : (right.type == TYPE_BYTE) ? right.b : 0;
    if (strcmp(op, ">") == 0) return l > r;
    if (strcmp(op, "<") == 0) return l < r;
    if (strcmp(op, ">=") == 0) return l >= r;
    if (strcmp(op, "<=") == 0) return l <= r;
    if (strcmp(op, "=") == 0) return l == r;
    return 0;
}
