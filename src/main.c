#include <stdio.h>
#include <interpreter.h>
#include <instance.h>
#include <config.h>

int main(int argc, char* argv[])
{
    printf("Keszeg4ci\n");
    static char code[INTERPRETER_MAX_CODE];

    unsigned long long int code_size = 0;
    static Interpreter_Instance instance;
    interpreter_instance_init(&instance);

    if (argc < 2) {
        printf("Usage: %s <source-file>\n", argv[0]);
        return 1;
    }

    FILE* f = fopen(argv[1], "r");
    if (!f) {
        printf("Failed to open source file");
        return 1;
    }
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (fsize > INTERPRETER_MAX_CODE) {
        printf("Source file is too large");
        fclose(f);
        return 1;
    }
    code_size = fsize;

    fread(code, 1, fsize, f);
    code[fsize] = 0;
    fclose(f);

    if (interpreter_load_code(&instance, code) != 0) {
        printf("Failed to load code.\n");
        return 1;
    }

    if (interpreter_parse_functions(&instance) != 0) {
        printf("Failed to parse functions.\n");
        return 1;
    }

    interpreter_dump_tokens(&instance);
    interpreter_dump_functions(&instance);

    if (interpreter_execute(&instance) != 0) {
        printf("Failed to execute code.\n");
        return 1;
    }
    return 0;
}
