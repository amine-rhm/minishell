#ifndef _CMD_H
#define _CMD_H

#include <stdio.h>

#define MAX_ARGS 64

// Constantes explicites pour les opérateurs logiques
#define LOGICAL_AND 1
#define LOGICAL_OR  2
#define LOGICAL_NONE 0
typedef struct cmd {
    char* args[MAX_ARGS];
    char* input_file;
    char* output_file;
    char* error_file;
    FILE* input;
    FILE* output;
    int background;
    int has_pipe;
    struct cmd* next;
    int logical_op;
    int append;  // Ajoutez ce membre pour l'option "append"
} cmd_t;

// Déclaration des fonctions
int init_cmd(cmd_t* p);
int exec_cmd(cmd_t* p);
int parse_cmd(char* tokens[], cmd_t* cmds, size_t max);

#endif /* _CMD_H */

