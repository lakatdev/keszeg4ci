#include <instructions.h>
#include <interpreter.h>
#include <instance.h>
#include <stdio.h>

void interpreter_execute_add(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_add\n");}
void interpreter_execute_add_assign(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_add_assign\n");}
void interpreter_execute_array_get(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_array_get\n");}
void interpreter_execute_array_set(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_array_set\n");}
void interpreter_execute_assign(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_assign\n");}

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
void interpreter_execute_declare(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_declare\n");}
void interpreter_execute_divide(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_divide\n");}
void interpreter_execute_divide_assign(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_divide_assign\n");}
void interpreter_execute_end(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_end\n");}
void interpreter_execute_exec(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_exec\n");}
void interpreter_execute_free(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_free\n");}
void interpreter_execute_if(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_if\n");}
void interpreter_execute_input(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_input\n");}
void interpreter_execute_load(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_load\n");}
void interpreter_execute_mod(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_mod\n");}
void interpreter_execute_mod_assign(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_mod_assign\n");}
void interpreter_execute_multiply(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_multiply\n");}
void interpreter_execute_multiply_assign(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_multiply_assign\n");}
void interpreter_execute_print(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_print\n");}
void interpreter_execute_println(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_println\n");}
void interpreter_execute_random(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_random\n");}
void interpreter_execute_return(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_return\n");}
void interpreter_execute_save(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_save\n");}
void interpreter_execute_sizeof(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_sizeof\n");}
void interpreter_execute_sleep(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_sleep\n");}
void interpreter_execute_subtract(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_subtract\n");}
void interpreter_execute_subtract_assign(Interpreter_Instance* instance, char** tokens, int token_count){ printf("interpreter_execute_subtract_assign\n");}
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
