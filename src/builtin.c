#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "builtin.h"
#include "cmd.h"
#include <ctype.h>  // Pour isalpha et isalnum
#include "parser.h"  // Si substenv est dans parser.c

// Vérifie si la commande est interne
int is_builtin(const char* cmd) {
    return (strcmp(cmd, "cd") == 0 || 
            strcmp(cmd, "export") == 0 || 
            strcmp(cmd, "unset") == 0 || 
            strcmp(cmd, "exit") == 0);
}

// Exécute une commande interne
int builtin(cmd_t* cmd) {
    if (strcmp(cmd->args[0], "cd") == 0) {
        return cd(cmd->args[1], STDERR_FILENO);
    } else if (strcmp(cmd->args[0], "export") == 0) {
        if (cmd->args[1] == NULL) {
            fprintf(stderr, "export: aucun argument fourni\n");
            return -1;
        }

        char* var = strtok(cmd->args[1], "=");
        char* value = strtok(NULL, "");

        return export(var, value ? value : "", STDERR_FILENO);
    } else if (strcmp(cmd->args[0], "unset") == 0) {
        if (cmd->args[1] == NULL) {
            fprintf(stderr, "unset: aucun argument fourni\n");
            return -1;
        }
        if (unsetenv(cmd->args[1]) != 0) {
            perror("unset");
            return -1;
        }
        return 0;
    } else if (strcmp(cmd->args[0], "exit") == 0) {
        int ret = (cmd->args[1] != NULL) ? atoi(cmd->args[1]) : 0;
        return exit_shell(ret, STDERR_FILENO);
    }

    return 0; // Pas une commande interne
}
// Export function implementation
// Corrected export function implementation
// Implémentation de la fonction export
int export(const char* var, const char* value, int fderr) {
    printf("Exporting: %s = %s\n", var, value);  // Ligne de débogage

    if (var == NULL || *var == '\0') {
        fprintf(stderr, "export: invalid variable name\n");
        return -1;
    }

    // Vérification du nom de la variable
    if (!isalpha((unsigned char)var[0]) && var[0] != '_') {
        fprintf(stderr, "export: invalid variable name\n");
        return -1;
    }

    // Vérification des caractères valides dans le nom de la variable
    for (int i = 1; var[i] != '\0'; i++) {
        if (!isalnum((unsigned char)var[i]) && var[i] != '_') {
            fprintf(stderr, "export: invalid variable name\n");
            return -1;
        }
    }

    // Utilisation de setenv pour définir la variable d'environnement
    if (setenv(var, value, 1) == -1) {
        perror("export: error setting environment variable");
        return -1;
    }

    // Vérification que la variable a bien été définie
    printf("After export, MY_VAR = %s\n", getenv(var)); // Vérification
    return 0;  // Succès
}


// Change le répertoire courant
int cd(const char* path, int fderr) {
    if (path == NULL) {
        path = getenv("HOME");
        if (path == NULL) {
            fprintf(stderr, "cd: variable d'environnement HOME non définie\n");
            return -1;
        }
    }
    if (chdir(path) != 0) {
        perror("cd");
        return -1;
    }
    return 0;
}

// Quitte le minishell
int exit_shell(int ret, int fderr) {
    exit(ret);  // Quitte avec le code de retour
    return 0;   // Jamais atteint
}

