#include "cmd.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "builtin.h"
#include "parser.h"  // Add this if substenv is declared in utils.h

// Exécution d'une commande interne ou externe

// Executing a command (internal or external)
int exec_cmd(cmd_t* cmd) {
    if (cmd->args[0] == NULL) {
        return -1; // Aucun argument
    }

    // Gestion de la commande echo
    if (strcmp(cmd->args[0], "echo") == 0) {
        for (int i = 1; cmd->args[i] != NULL; i++) {
            if (cmd->args[i][0] == '$') {
                // Récupérer la valeur de la variable d'environnement
                char* env_var = getenv(&cmd->args[i][1]); // Ignore le '$' au début
                if (env_var != NULL) {
                    printf("%s ", env_var);  // Afficher la valeur de la variable
                } else {
                    printf("%s ", cmd->args[i]);  // Si variable non définie, afficher le nom
                }
            } else {
                printf("%s ", cmd->args[i]);  // Afficher l'argument tel quel
            }
        }
        printf("\n");
        return 0;
    }

    // Vérification pour les autres commandes internes
    if (is_builtin(cmd->args[0])) {
        return builtin(cmd); // Exécuter une commande interne
    }

    // Exécution d'une commande externe
    pid_t pid = fork();
    if (pid == 0) {
        // Processus enfant : exécute la commande externe
        execvp(cmd->args[0], cmd->args);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Processus parent : attend la fin du processus enfant
        int status;
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    } else {
        perror("fork");
        return -1;
    }
}

// Initialisation d'une commande
int init_cmd(cmd_t *p) {
    if (p == NULL) {
        return -1;
    }
    memset(p->args, 0, sizeof(p->args)); // Initialiser tous les arguments à NULL
    p->input = stdin; // Par défaut, l'entrée est stdin
    p->output = stdout; // Par défaut, la sortie est stdout
    p->background = 0; // Par défaut, la commande ne s'exécute pas en arrière-plan
    return 0;
}

// Analyse des tokens pour créer une liste de commandes
int parse_cmd(char* tokens[], cmd_t* cmds, size_t max) {
    int cmd_count = 0;
    int arg_index = 0;

    init_cmd(&cmds[cmd_count]);

    for (int i = 0; tokens[i] != NULL; i++) {
        // Handle redirections and other operators first
        if (strcmp(tokens[i], ">") == 0) {
            cmds[cmd_count].output_file = tokens[++i];
        } else if (strcmp(tokens[i], "<") == 0) {
            cmds[cmd_count].input_file = tokens[++i];
        } else if (strcmp(tokens[i], "2>") == 0) {
            cmds[cmd_count].error_file = tokens[++i];
        } else if (strcmp(tokens[i], "&") == 0) {
            cmds[cmd_count].background = 1;
        } else if (strcmp(tokens[i], "|") == 0) {
            cmds[cmd_count].args[arg_index] = NULL;
            cmds[cmd_count].has_pipe = 1;
            cmd_count++;
            if (cmd_count >= max) {
                fprintf(stderr, "Erreur : trop de commandes dans la ligne\n");
                return -1;
            }
            init_cmd(&cmds[cmd_count]);
            cmds[cmd_count - 1].next = &cmds[cmd_count];
            arg_index = 0;
        } else if (strcmp(tokens[i], "export") == 0) {
            // Handle export specifically
            if (tokens[i+1] != NULL) {
                char* var_value = tokens[++i]; // Move to the next token which should be the variable=value string

                // Split the var=value pair
                char* var = strtok(var_value, "=");
                char* value = strtok(NULL, ""); // Remaining part is the value

                // Store the variable and its value in the command args
                cmds[cmd_count].args[0] = "export";
                cmds[cmd_count].args[1] = var;
                cmds[cmd_count].args[2] = value ? value : "";
                cmds[cmd_count].args[3] = NULL; // End the argument list

            } else {
                fprintf(stderr, "export: no arguments provided\n");
                return -1;
            }
        } else {
            cmds[cmd_count].args[arg_index++] = tokens[i];
        }
    }
    cmds[cmd_count].args[arg_index] = NULL;

    return cmd_count + 1;
}

